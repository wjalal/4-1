#include <fstream>
#include "H3dMat.hpp"
#include <string>
#include <stack>
#include <vector>
#include <tuple>
#include "bitmap_image.hpp"

using namespace std;

const double zMax = 1.0;
ofstream stage1, stage2, stage3, zBufferFile;
Point eye, look;
Vec up;
int screenWidth, screenHeight;
double fovY, aR, near, far, dx, dy, topY, leftX;
H3dMat *M = new H3dMat(), *V = new H3dMat(), *P = new H3dMat();
stack<H3dMat*> S; 
vector<vector<Point*>> triangles;
double **zBuffer, **imageBuffer;

void readScene (ifstream& scene) {
    scene >> eye.x >> eye.y >> eye.z;
    scene >> look.x >> look.y >> look.z;
    scene >> up.x >> up.y >> up.z;
    up.updateCalc();
    Vec* l = new Vec (new Line(&eye, &look));
    l->normalize();
    Vec* r = l->cross(&up);
    r->normalize();
    Vec* u = r->cross(l);
    H3dMat T, R;
    T.mat[0][3] = -eye.x, T.mat[1][3] = -eye.y, T.mat[2][3] = -eye.z; 
    R.mat[0][0] = r->x, R.mat[0][1] = r->y, R.mat[0][2] = r->z;
    R.mat[1][0] = u->x, R.mat[1][1] = u->y, R.mat[1][2] = u->z;
    R.mat[2][0] = -l->x, R.mat[2][1] = -l->y, R.mat[2][2] = -l->z;
    V = (H3dMat*)(R.product(&T));

    scene >> fovY >> aR >> near >> far;
    fovY *= (M_PI / 180.0);
    double fovX = fovY * aR;
    double tt = near * tan(fovY/2), rr = near * tan(fovX/2);
    P->mat[0][0] = 1.0 * near / rr, P->mat[1][1] = 1.0 * near / tt;
    P->mat[2][2] = -1.0*(far+near)/(far-near);
    P->mat[3][2] = -1, P->mat[3][3] = 0;
    P->mat[2][3] = -2.0*(far*near)/(far-near);
    string cmd;
    double x, y, z;
    while (true) {
        scene >> cmd;
        if (cmd == "triangle") {
            Point p[3];
            vector<Point*> trngl;
            for (int i=0; i<3; i++) {
                scene >> p[i].x >> p[i].y >> p[i].z;
                H3dPoint* Q = new H3dPoint(&p[i]);
                Q = (H3dPoint*)(M->product(Q));
                Q->print(stage1);
                Q = (H3dPoint*)(V->product(Q));
                Q->print(stage2);
                Q = (H3dPoint*)(P->product(Q));
                Q->mat[0][0] /= Q->mat[3][0], Q->mat[1][0] /= Q->mat[3][0], Q->mat[2][0] /= Q->mat[3][0];
                Q->mat[3][0] = 1;
                Q->print(stage3);
                trngl.push_back(new Point(Q->mat[0][0], Q->mat[1][0], Q->mat[2][0]));
            };
            stage1 << endl; stage2 << endl; stage3 << endl;
            triangles.push_back(trngl);
        } else if (cmd == "translate") {
            scene >> x >> y >> z;
            H3dMat T;
            T.mat[0][3] = x, T.mat[1][3] = y, T.mat[2][3] = z; 
            M = (H3dMat*)(M->product(&T));
        } else if (cmd == "scale") {
            scene >> x >> y >> z;
            H3dMat T;
            T.mat[0][0] = x, T.mat[1][1] = y, T.mat[2][2] = z; 
            M = (H3dMat*)(M->product(&T));
        } else if (cmd == "rotate") {
            double A;
            scene >> A >> x >> y >> z;
            A *= (M_PI / 180.0);
            Vec a(x,y,z); a.normalize();
            Point i(1,0,0), j(0,1,0), k(0,0,1);
            rotatePoint(&i,&a,A), rotatePoint(&j,&a,A), rotatePoint(&k,&a,A);
            H3dMat T;
            T.mat[0][0] = i.x, T.mat[1][0] = i.y, T.mat[2][0] = i.z;
            T.mat[0][1] = j.x, T.mat[1][1] = j.y, T.mat[2][1] = j.z;
            T.mat[0][2] = k.x, T.mat[1][2] = k.y, T.mat[2][2] = k.z;
            M = (H3dMat*)(M->product(&T));
        } else if (cmd == "push") {
            S.push(M);
            M = new H3dMat();
            M->setVals (S.top()->mat);
        } else if (cmd == "pop") {
            M = S.top();
            S.pop();
        } else if (cmd == "end") {
            break;
        };
    };
};

static unsigned long int g_seed = 1;
inline int randomCol() {
    g_seed = (214013 * g_seed + 2531011);
    return (g_seed >> 16) & 0x7FFF;
};

void zBufferScan() {
    dx = 2.0/screenWidth, dy = 2.0/screenHeight;
    topY = 1.0 - dy/2, leftX = -1.0 + dx/2;
    
    zBuffer = new double*[screenHeight];
    for (int i=0; i<screenHeight; i++) {
        zBuffer[i] = new double[screenWidth];
        for (int j=0; j<screenWidth; j++)
            zBuffer[i][j] = 2.0;
    };

    bitmap_image image (screenWidth, screenHeight);
    image.set_all_channels (0, 0, 0);

    for (int t=0; t<triangles.size(); t++) {
        int colR = random(), colG = random(), colB = random();
        int topLine , bottomLine;
        double epsilon = 1.0/screenHeight;

        double maxY = max(triangles[t][0]->y, max(triangles[t][1]->y, triangles[t][2]->y));
        if (maxY > topY) topLine = 0;
        else topLine = round((topY-maxY)/2.0 * screenHeight);

        double minY = min(triangles[t][0]->y, min(triangles[t][1]->y, triangles[t][2]->y));
        if (minY < -topY) bottomLine = screenHeight-1;
        else bottomLine = round((topY-minY)/2.0 * screenHeight);
        // cout << topLine << " " << bottomLine << endl;

        int k; Point p[3];
        if (fabs(triangles[t][0]->y - triangles[t][1]->y) < epsilon) k = 2;
        else if (fabs(triangles[t][1]->y - triangles[t][2]->y) < epsilon) k = 0;
        else if (fabs(triangles[t][2]->y - triangles[t][0]->y) < epsilon) k = 1;
        for (int ii=0; ii<3; ii++) p[ii] = *triangles[t][(k+ii)%3];
        if (p[1].x > p[2].x) swap(p[1], p[2]);

        // cout << fixed << setprecision(7);
        // cout << p[0].x << " " << p[0].y << " " << p[0].z << endl 
        //      << p[1].x << " " << p[1].y << " " << p[1].z << endl 
        //      << p[2].x << " " << p[2].y << " " << p[2].z << endl << endl;

        for (int r = topLine; r < bottomLine; r++) {
            double yp = topY - dy*r;
            double xa = p[0].x - (p[0].x - p[1].x) / (p[0].y - p[1].y) * (p[0].y - yp);
            double za = p[0].z - (p[0].z - p[1].z) / (p[0].y - p[1].y) * (p[0].y - yp);
            double xb = p[0].x - (p[0].x - p[2].x) / (p[0].y - p[2].y) * (p[0].y - yp);
            double zb = p[0].z - (p[0].z - p[2].z) / (p[0].y - p[2].y) * (p[0].y - yp);

            // if (t==3) {
            //     cout << "xa  = " << xa << endl;
            //     cout << "za  = " << za << endl;
            //     cout << "xb  = " << xb << endl;
            //     cout << "zb  = " << zb << endl << endl;
            // };

            int leftPoint, rightPoint;
            if (xa < leftX) leftPoint = 0;
            else leftPoint = round((xa-leftX)/2.0 * screenWidth);
            if (xb > -leftX) rightPoint = screenWidth-1;
            else rightPoint = round((xb-leftX)/2.0 * screenWidth);
            // cout << leftPoint << " " << rightPoint << endl;

            for (int c = leftPoint; c < rightPoint; c++) {
                double xp = leftX + dx*c;
                double zp = za + (xp-xa)/(xa-xb)*(za-zb);
                if (zp < zBuffer[r][c] && fabs(zp) < zMax) {
                    zBuffer[r][c] = zp;
                    image.set_pixel (c, r, colR, colG, colB);
                };
                // zBufferFile << zp << " "; 
            };
            // zBufferFile << endl;
        };
    };

    zBufferFile << fixed << setprecision(6);
    for (int r=0; r<screenHeight; r++) {
        for (int c=0; c<screenWidth; c++) {
            if (zBuffer[r][c] < zMax) 
                zBufferFile << zBuffer[r][c] << "\t"; 
        };
        zBufferFile << endl;
    };

    image.save_image ("out.bmp");
};

int main() {
    ifstream sceneFile, configFile;
    sceneFile.open("scene.txt");
    configFile.open("config.txt");
    stage1.open("stage1.txt");
    stage2.open("stage2.txt");
    stage3.open("stage3.txt");
    zBufferFile.open("z_buffer.txt");
    readScene(sceneFile);
    
    configFile >> screenWidth >> screenHeight;
    zBufferScan();

    return 0;

};
