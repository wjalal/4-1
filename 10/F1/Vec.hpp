#include <ctime>
#include <math.h>
#ifdef __linux__
    #include <GL/glut.h>
#elif WIN32
    #include <windows.h>
    #include <glut.h>
#endif


using namespace std;
bool drawaxes = true, drawUpArrow = true, drawboard = true;

class Point {
    public:
    double x, y, z;

    Point (double x, double y, double z) {
        this->x = x;
        this->y = y;
        this->z = z;
    };
};

class Line {
    public: 
    Point *p1, *p2;

    Line (Point* p1, Point* p2) {
        this->p1 = p1;
        this->p2 = p2;
    };

    Line (double x1, double y1, double z1, double x2, double y2, double z2) {
        this->p1 = new Point (x1, y1, z1);
        this->p2 = new Point (x2, y2, z2);
    };
};

class Vec {
    public:
    double x, y, z, D, cosTx, cosTy, cosTz;

    void updateCalc() {
        D = sqrt (x*x + y*y + z*z);
        // cout << D << endl;
        cosTx = x/D, cosTy = y/D, cosTz = z/D;
    };

    Vec (double x, double y, double z) {
        this->x = x, this->y = y, this->z = z;
        updateCalc();
    };

    Vec (Point* P) {
        this->x = P->x, this->y = P->y, this->z = P->z;
        updateCalc();       
    };
    Vec (Line* L) {
        this->x = L->p2->x - L->p1->x;
        this->y = L->p2->y - L->p1->y;
        this->z = L->p2->z - L->p1->z;
        updateCalc();
    };

    Vec* cross (Vec* L) {
        return new Vec (this->y * L->z - this->z * L->y,
                        this->z * L->x - this->x * L->z,
                        this->x * L->y - this->y * L->x);
    };

    Vec operator + (const Vec& b) {
        Vec sum(this->x, this->y, this->z);
        sum.x += b.x, sum.y += b.y, sum.z += b.z;
        return sum;
    };

    Vec operator * (double b) {
        Vec sum(this->x, this->y, this->z);
        sum.x *= b, sum.y *= b, sum.z *= b;
        return sum;
    };

    double operator * (const Vec& b) {
        return b.x*this->x + b.y*this->y + b.z*this->z;
    };

    void normalize() {
        this->x /= D, this->y /= D; this->z /= D;
        updateCalc();
    };
};

void rotatePoint (Point* P, Vec* v, double A) {
    Vec *u, p(P);
    u = v->cross(&p);
    Vec rot = p*cos(A) + *(v->cross(&p)) * sin(A) + ((*v)*(*v * p))*(1-cos(A));
    P->x = rot.x, P->y = rot.y, P->z = rot.z;
};
