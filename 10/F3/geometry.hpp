#include <iostream>
#include "camera.hpp"
#include <vector>

#define RED 0
#define GRN 1
#define BLU 2

#define AMBI 0
#define DIFF 1
#define SPEC 2
#define REFL 3

constexpr double epsilon = std::numeric_limits<double>::epsilon();
extern Vec* lookAtVec;
extern Line lookAt;
extern int recLevel;

class Ray {
public:
    Vec* start;
    Vec* dir;
    bool alloc = false;
    Ray (Vec* start, Vec* dir) {
        this->start = start, this->dir = dir;
        this->dir->normalize();
        this->alloc = false;
    };
    Ray (double startX, double startY, double startZ, double dirX, double dirY, double dirZ) {
        Vec* s = new Vec (startX, startY, startZ);
        Vec* d = new Vec (dirX, dirY, dirZ);
        this->start = s, this->dir = d;
        this->dir->normalize();
        this->alloc = true;
    };
    ~Ray() {
        if (alloc) {
            delete start;
            delete dir;
        };
    };
};

class Light {
public: 
    Vec* ref_point;
    double color[3];

    ~Light() {
        delete ref_point;
    };

    void draw() {
        glColor3f (color[RED], color[GRN], color[BLU]);
        glBegin (GL_POINTS); {
            glVertex3f (ref_point->x, ref_point->y, ref_point->z);
        } glEnd();
    };

    void setColor(double r, double g, double b) {
        color[RED] = r, color[GRN] = g, color[BLU] = b;
    };
};


class PointLight : public Light {
public: 
    PointLight (Vec* light_pos) {
        this->ref_point = light_pos;
    };
};

class SpotLight : public Light {
public: 
    Vec* light_dir;
    double cuttoff_angle;

    SpotLight (Vec* light_pos, Vec* light_dir, double cutoff_angle ) {
        this->ref_point = light_pos;
        this->light_dir = light_dir;
        this->light_dir->normalize();
        this->cuttoff_angle = cutoff_angle;
    };

    ~SpotLight() {
        delete light_dir;
    };
};

class Object;

extern vector<PointLight*> pointLights;
extern vector<SpotLight*> spotLights;
extern vector<Object*> objects;

class Object {
public: 
    Vec* ref_point;
    double height, width, length;
    double color[3];
    double coeff[4];
    int shine;

    // Object() ;
    ~Object() {
        delete ref_point;
    };
    
    virtual void draw() {};

    void setColor(double r, double g, double b) {
        color[RED] = r, color[GRN] = g, color[BLU] = b;
    };

    void setShine (int shine) {
        this->shine = shine;
    };

    void setCoeff (double ambi, double diff, double spec, double refl) {
        coeff[AMBI] = ambi, coeff[DIFF] = diff, coeff[SPEC] = spec, coeff[REFL] = refl;
    };

    virtual double intersect (Ray* R, double* color, int level) {
        return -1.0;
    };

    virtual Vec* getNormal (Vec* intersectionPoint) {
        return nullptr;
    };

    virtual double* getColor (Vec* intersectionPoint) {
        return this->color;
    };

    bool obscuredByOther (Ray* R) {
        Object* nearest = nullptr;
        double tMin = INFINITY;
        for (int k=0; k<objects.size(); k++) {
            double t = objects[k]->intersect(R, nullptr, 0);
            if (t>0 && t<tMin) {
                // cout << "k = " << k << " t = " << t << endl;
                nearest = objects[k], tMin = t;
            };
        };
        if (nearest == this) return false;
        return true;
    };

    Ray* calcPhong (Ray* R, double* color, int level, double t) {
        Vec* intersectionPoint = new Vec(0,0,0);
        *intersectionPoint = (*R->start) + (*R->dir)*t;       
        for (int c=0; c<3; c++) color[c] = getColor(intersectionPoint)[c] * coeff[AMBI];

        Vec* normal = getNormal(intersectionPoint);
        for (int i=0; i<pointLights.size(); i++) {
            Vec *lDir = new Vec (0,0,0), *refl = new Vec(0,0,0);
            *lDir = *intersectionPoint + (*pointLights[i]->ref_point)*(-1);
            lDir->normalize();
            Ray* R1 = new Ray (pointLights[i]->ref_point, lDir);
            if (obscuredByOther(R1)) {
                delete lDir; delete R1; delete refl;
                continue;
            };
            *lDir = (*lDir) * (-1);
            double lambert = (*lDir) * (*normal);
            *refl = (*lDir) + (*normal) * (-2*lambert);
            double phong = (*refl * (*R->dir));
            lambert = max(0.0, lambert), phong = max(0.0, phong);
            for (int c=0; c<3; c++) 
                color[c] += pointLights[i]->color[c] * (lambert*coeff[DIFF] + pow(phong, shine)*coeff[SPEC]) * getColor(intersectionPoint)[c];
            delete lDir; delete R1; delete refl;
        };
        for (int i=0; i<spotLights.size(); i++) {
            Vec* lDir = new Vec (0,0,0), *refl = new Vec(0,0,0);
            *lDir = *intersectionPoint + (*spotLights[i]->ref_point)*(-1);
            lDir->normalize();
            double beta = acos ((*lDir)*(*spotLights[i]->light_dir)) * 180.0/M_PI;
            // cout << beta << endl;
            if (beta > spotLights[i]->cuttoff_angle) {
                delete lDir; delete refl;
                continue;
            };
            Ray* R1 = new Ray (spotLights[i]->ref_point, lDir);
            if (obscuredByOther(R1)) {
                delete lDir; delete R1; delete refl;
                continue;
            };
            *lDir = (*lDir) * (-1);
            double lambert = (*R1->dir) * (*normal);
            *refl = (*R1->dir) + (*normal) * (-2*lambert);
            double phong = (*refl * (*R->dir));
            lambert = max(0.0, lambert), phong = max(0.0, phong);
            for (int c=0; c<3; c++) 
                color[c] += spotLights[i]->color[c] * (lambert*coeff[DIFF] + pow(phong, shine)*coeff[SPEC]) * getColor(intersectionPoint)[c];
            delete lDir; delete R1; delete refl;
        };
        // intersectionPoint = intersectionPoint + (*R->dir)*0.0001;
        Vec* ref = new Vec(0,0,0);
        *ref = (*R->dir) + (*normal) * (-2.0*((*R->dir)*(*normal)));
        delete normal;
        return new Ray (intersectionPoint, ref);
    };

    void reflect (Ray* rRef, int level, double* color) {
        Object* nearest = nullptr;
        double tMin = INFINITY;
        for (int k=0; k<objects.size(); k++) {
            if (objects[k] == this) continue;
            double t = objects[k]->intersect(rRef, nullptr, 0);
            if (t>0 && t<tMin) {
                // cout << "k = " << k << " t = " << t << endl;
                nearest = objects[k], tMin = t;
            };
        };
        if (tMin > 0 && nearest) {
            double* colRef = new double[3];
            nearest->intersect (rRef, colRef, level+1);
            for (int c=0; c<3; c++) color[c] += colRef[c] * coeff[REFL];
            delete[] colRef;
        };
        delete rRef->start; delete rRef->dir;
        delete rRef;
    };
};

class Sphere : public Object {
    int nSec= 20, nStack = 10;
    Point ***upVerts, ***downVerts, **eqVerts, *nPole, *sPole;
public: 
    Sphere (Vec* center, double radius) {
        this->ref_point = center;
        this->length = radius;
        double r = this->length;
        nPole = new Point (0, 0, r);
        sPole = new Point (0, 0, -r);
        double dTheta = 2 * M_PI / nSec;
        //start from pos x-axis CCW
        eqVerts = new Point* [nSec];
        upVerts = new Point** [nStack];
        downVerts = new Point** [nStack];
        for (int i=0; i<nSec; i++)
            eqVerts[i] = new Point (r*cos(dTheta*i), r*sin(dTheta*i), 0);
        for (int i=1; i<nStack; i++) {
            upVerts[i] = new Point* [nSec];
            downVerts[i] = new Point* [nSec];
            double phi = M_PI/2 * i/nStack;
            double R = r * cos(phi);
            for (int j=0; j<nSec; j++) {
                upVerts[i][j] = new Point (R*cos(dTheta*j), R*sin(dTheta*j), r*sin(phi));
                downVerts[i][j] = new Point (R*cos(dTheta*j), R*sin(dTheta*j), -r*sin(phi));
            };
        };
    };

    ~Sphere() {
        delete nPole; delete sPole;
        for (int i=0; i<nSec; i++) delete eqVerts[i];
        delete[] eqVerts;
        for (int i=1; i<nStack; i++) {
            for (int j=0; j<nSec; j++) {
                delete upVerts[i][j];
                delete downVerts[i][j];
            };
            delete[] upVerts[i];
            delete[] downVerts[i];
        };
        delete[] upVerts;
        delete[] downVerts;
    };

    void draw() {
        glColor3f (color[RED], color[GRN], color[BLU]);
        glPushMatrix();
        glTranslatef (ref_point->x, ref_point->y, ref_point->z);
        glBegin (GL_QUADS); {
            for (int k=1; k < nStack-1; k++) {
                for (int i=0; i < nSec; i++) {
                    glVertex3f (upVerts[k+1][i]->x, upVerts[k+1][i]->y, upVerts[k+1][i]->z);
                    glVertex3f (upVerts[k][i]->x, upVerts[k][i]->y, upVerts[k][i]->z);
                    glVertex3f (upVerts[k][(i+1)%nSec]->x, upVerts[k][(i+1)%nSec]->y, upVerts[k][(i+1)%nSec]->z);
                    glVertex3f (upVerts[k+1][(i+1)%nSec]->x, upVerts[k+1][(i+1)%nSec]->y, upVerts[k+1][(i+1)%nSec]->z);
                    
                    glVertex3f (downVerts[k+1][i]->x, downVerts[k+1][i]->y, downVerts[k+1][i]->z);
                    glVertex3f (downVerts[k][i]->x, downVerts[k][i]->y, downVerts[k][i]->z);
                    glVertex3f (downVerts[k][(i+1)%nSec]->x, downVerts[k][(i+1)%nSec]->y, downVerts[k][(i+1)%nSec]->z);
                    glVertex3f (downVerts[k+1][(i+1)%nSec]->x, downVerts[k+1][(i+1)%nSec]->y, downVerts[k+1][(i+1)%nSec]->z);
                };
            };
            for (int i=0; i < nSec; i++) {
                glVertex3f (upVerts[1][i]->x, upVerts[1][i]->y, upVerts[1][i]->z);
                glVertex3f (eqVerts[i]->x, eqVerts[i]->y, eqVerts[i]->z);
                glVertex3f (eqVerts[(i+1)%nSec]->x, eqVerts[(i+1)%nSec]->y, eqVerts[(i+1)%nSec]->z);
                glVertex3f (upVerts[1][(i+1)%nSec]->x, upVerts[1][(i+1)%nSec]->y, upVerts[1][(i+1)%nSec]->z);
                
                glVertex3f (downVerts[1][i]->x, downVerts[1][i]->y, downVerts[1][i]->z);
                glVertex3f (eqVerts[i]->x, eqVerts[i]->y, eqVerts[i]->z);
                glVertex3f (eqVerts[(i+1)%nSec]->x, eqVerts[(i+1)%nSec]->y, eqVerts[(i+1)%nSec]->z);
                glVertex3f (downVerts[1][(i+1)%nSec]->x, downVerts[1][(i+1)%nSec]->y, downVerts[1][(i+1)%nSec]->z);
            };

        } glEnd();
        glBegin (GL_TRIANGLES); {
            for (int i=0; i < nSec; i++) {
                glVertex3f (nPole->x, nPole->y, nPole->z);
                glVertex3f (upVerts[nStack-1][i]->x, upVerts[nStack-1][i]->y, upVerts[nStack-1][i]->z);
                glVertex3f (upVerts[nStack-1][(i+1)%nSec]->x, upVerts[nStack-1][(i+1)%nSec]->y, upVerts[nStack-1][(i+1)%nSec]->z);

                glVertex3f (sPole->x, sPole->y, sPole->z);
                glVertex3f (downVerts[nStack-1][i]->x, downVerts[nStack-1][i]->y, downVerts[nStack-1][i]->z);
                glVertex3f (downVerts[nStack-1][(i+1)%nSec]->x, downVerts[nStack-1][(i+1)%nSec]->y, downVerts[nStack-1][(i+1)%nSec]->z);
            };
        } glEnd();
        glPopMatrix();
    };

    Vec* getNormal (Vec* intersectionPoint) {
        Vec* normal = new Vec (0,0,0);
        *normal = (*intersectionPoint) + (*ref_point)*(-1);
        normal->normalize();
        return normal;
    };

    double intersect (Ray* R, double* color, int level) {
        Vec* R0 = new Vec(0,0,0);
        *R0 = (*R->start) + (*ref_point)*(-1);
        double t = -1, r = this->length;
        double tP = -(*R0 * (*R->dir));
        double dSq = (*R0 * *R0) - tP*tP;
        double t_Sq = r*r - dSq;
        bool originOutside = (R0->x*R0->x + R0->y*R0->y + R0->z*R0->z > r*r);
        if (!originOutside || t_Sq > 0) {
            if (originOutside) t = tP - sqrt(t_Sq);
            else t = tP + sqrt(t_Sq);
        };
        delete R0;
        if (level == 0) return t;

        if (t != -1) {
            Ray* rRef = calcPhong (R, color, level, t);
            if (level >= recLevel) {
                delete rRef->start; delete rRef->dir;
                delete rRef;
                return t;
            };
            reflect (rRef, level, color);
        };
        return t;
    };
};

class Triangle : public Object {
public:

    Vec *p1, *p2, *p3;
    Triangle (Vec* p1, Vec* p2, Vec* p3) {
        ref_point = p1;
        this->p1 = p1, this->p2 = p2, this->p3 = p3;
    };

    ~Triangle() {
        delete p2; delete p3;
    };

    void draw() {
        glColor3f (color[RED], color[GRN], color[BLU]);
        glBegin (GL_TRIANGLES); {
            glVertex3f (p1->x, p1->y, p1->z);
            glVertex3f (p2->x, p2->y, p2->z);
            glVertex3f (p3->x, p3->y, p3->z);
        } glEnd();
    };

    Vec* getNormal (Vec* intersectionPoint) {
        Vec edge1(0,0,0), edge2(0,0,0), *normal = new Vec (0,0,0);
        edge1 = (*p2) + (*p1)*(-1);
        edge2 = (*p3) + (*p1)*(-1);
        normal = edge1.cross(&edge2);
        normal->normalize();
        return normal;
    };

    double intersect (Ray* R, double* color, int level) {
        double t = -1;
        Vec *edge1 = new Vec(0,0,0), *edge2 = new Vec(0,0,0), *Rxe2, *S = new Vec(0,0,0), *S_;
        *edge1 = (*p2) + (*p1)*(-1);
        *edge2 = (*p3) + (*p1)*(-1);
        Rxe2 = R->dir->cross(edge2);
        double det = *edge1 * *Rxe2;
        if (det > -epsilon && det < epsilon) {
            delete edge1; delete edge2; delete Rxe2; delete S;
            return -1;    
        };
        double inv_det = 1.0 / det;
        *S = (*R->start) + (*p1)*(-1);
        double u = inv_det * (*S * *Rxe2);
        if (u < 0 || u > 1) {
            delete edge1; delete edge2; delete Rxe2; delete S;
            return -1;
        };
        S_ = S->cross(edge1);
        double v = inv_det * ((*R->dir) * *S_);
        if (v < 0 || u + v > 1) {
            delete edge1; delete edge2; delete Rxe2; delete S; delete S_;
            return -1;
        };
        double tMin = inv_det * (*edge2 * *S_);
        if (tMin > epsilon) t = tMin;
        delete edge1; delete edge2; delete Rxe2; delete S; delete S_;

        if (level == 0) return t;

        if (t != -1) {
            Ray* rRef = calcPhong (R, color, level, t);
            if (level >= recLevel) {
                delete rRef->start; delete rRef->dir;
                delete rRef;
                return t;
            };
            reflect (rRef, level, color);
        };     
        return t;
    };
};

class General : public Object {
    bool checkBound (double x, double y, double z) {
        if (length>0 && (ref_point->x > x || x > ref_point->x + length)) return false;
        if (width>0 && (ref_point->y > y || y > ref_point->y + width)) return false;
        if (height>0 && (ref_point->z > z || z > ref_point->z + height)) return false;
        return true;
    };
public: 
    double A, B, C, D, E, F, G, H, I, J;

    General (Vec* ref_point, double length, double width, double height, 
             double a, double c, double b, double d, 
             double f, double e, double g, double i, double h, double j) {
        this->ref_point = ref_point;
        this->length = length, this->width = width, this->height = height;
        A=a, B=b, C=c, D=d, E=e, F=f, G=g, H=h, I=i, J=j;
    };

    double surface (double x, double y, double z) {
        return A*x*x + B*y*y + C*z*z + D*x*y + E*x*z + F*y*z + G*x + H*y + I*z + J;
    };

    void draw() {
        // glColor3f (color[RED], color[GRN], color[BLU]);
        // glBegin (GL_POINTS); {
        //     for (double i = -100; i <= 100; i+=0.0001) {
        //         for (double j = -100; j <= 100; j+=0.0001) {
        //             for (double k = -100; k <= 100; k+=0.0001) {
        //                 if (fabs(surface(i, j, k)) < 1) glVertex3f(i, j, k);
        //             };
        //         };
        //     };
        // } glEnd();
    };

    Vec* getNormal (Vec* intersectionPoint) {
        double xi = intersectionPoint->x, yi = intersectionPoint->y, zi = intersectionPoint->z;
        double xn = 2*A*xi + D*yi + E*zi + G;
        double yn = 2*B*yi + D*xi + F*zi + H;
        double zn = 2*C*zi + E*xi + F*yi + I;
        Vec *normal = new Vec (xn,yn,zn);
        normal->normalize();
        return normal;
    };

    double intersect (Ray* R, double* color, int level) {
        double t = -1, xd = R->dir->x, yd = R->dir->y, zd = R->dir->z;
        double xr = R->start->x, yr = R->start->y, zr = R->start->z;
        double a = A*xd*xd + B*yd*yd + C*zd*zd + D*xd*yd + E*xd*zd + F*yd*zd;
        double b = 2*A*xr*xd + 2*B*yr*yd + 2*C*zr*zd + D*(xr*yd + yr*xd) + E*xr*zd + F*(yr*zd + yd*zr) + G*xd + H*yd + I*zd;
        double c = A*xr*xr + B*yr*yr + C*zr*zr + D*xr*yr + E*xr*zr + F*yr*zr + G*xr + H*yr + I*zr + J;
        double t0 = (-b - sqrt(b*b - 4*a*c)) / (2*a), t1 = (-b + sqrt(b*b - 4*a*c)) / (2*a);
        if (t0 > 0 && t1 < 0) t = t0;
        else if (t1 > 0 && t0 < 0) t = t1;
        else if (t1 > 0 && t0 > 0) {
            Vec* P = new Vec(0,0,0);
            *P = (*R->start) + (*R->dir)*t0;
            bool t0b = checkBound(P->x, P->y, P->z);
            *P = (*R->start) + (*R->dir)*t1;
            bool t1b = checkBound(P->x, P->y, P->z);
            if (t0b && !t1b) t = t0;
            else if (!t0b && t1b) t = t1;
            else if (t0b && t1b) t = min(t0,t1);
            delete P;
        };
        if (level == 0) return t;

        if (t != -1) {
            Ray* rRef = calcPhong (R, color, level, t);
            if (level >= recLevel) {
                delete rRef->start; delete rRef->dir;
                delete rRef;
                return t;
            };
            reflect (rRef, level, color);
        };
        return t;
    };
};   


class Floor : public Object {
public:
    double floorWidth;
    double colBlack[3] = {0,0,0}, colWhite[3] = {1,1,1};
    Floor (double floorWidth, double tileWidth) {
        ref_point = new Vec (0, 0, 0);
        this->length = tileWidth;
        this->floorWidth = floorWidth;
        setColor (1, 0, 1);
        setCoeff (0.4, 0.3, 0.2, 0.3);
        setShine (20);
    };

    void draw() {
        glPushMatrix();
        glTranslatef(ref_point->x, ref_point->y, ref_point->z);
        glBegin(GL_QUADS); {
            for (double i = -floorWidth/2; i<+floorWidth/2; i+=length) {
                for (double j = -floorWidth/2; j<+floorWidth/2; j+=length) {
                    if ((int)(i+j)%(2*(int)length)) glColor3f(0, 0, 0);
                    else glColor3f(1, 1, 1);
                    glVertex3f(i, j, 0);
                    glVertex3f(i+length, j, 0);
                    glVertex3f(i+length, j+length, 0);
                    glVertex3f(i, j+length, 0);
                };
            };
        } glEnd();
        glPopMatrix();
    };

    double* getColor (Vec* intersectionPoint) {
        double x = intersectionPoint->x + floorWidth/2.0, y = intersectionPoint->y + floorWidth/2.0;
        int i = x/length, j = y/length;
        if ((i+j)%2) return colBlack;
        return colWhite;
    }; 

    Vec* getNormal (Vec* intersectionPoint = nullptr) {
        Vec* normal;
        if (intersectionPoint->z >= -0.000001) normal = new Vec (0,0,1);
        else normal = new Vec (0,0,-1);
        return normal;
    };

    double intersect (Ray* R, double* color, int level) {
        double t = -1;
        Vec *n = getNormal(R->start), *P = new Vec(0,0,0);
        double t_ = -(*n * (*R->start))/(*n * (*R->dir));
        *P = (*R->start) + (*R->dir)*t_;
        if (P->x < -floorWidth/2.0 || P->x > floorWidth/2.0) {
            delete n; delete P;
            return t;
        };
        if (P->y < -floorWidth/2.0 || P->y > floorWidth/2.0) {
            delete n; delete P;
            return t;
        };
        t = t_;
        delete n; delete P;
        if (level == 0) return t;

        if (t != -1) {
            Ray* rRef = calcPhong (R, color, level, t);
            if (level >= recLevel) {
                delete rRef->start; delete rRef->dir;
                delete rRef;
                return t;
            };
            reflect (rRef, level, color);
        };
        // cout << color[RED] << " " << color[GRN] << " " << color[BLU] << endl;
        return t;
    };
};


