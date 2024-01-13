#include "camera.hpp"
#include <iostream>
using namespace std;

class Sphere {
    public:
    int nSec, nStack;
    Point ***upVerts, ***downVerts, **eqVerts, *nPole, *sPole;
    double r;
    Sphere (double r, int nSec, int nStack) {
        cout << "hellosc1" << endl;
        this->r = r, this->nSec = nSec, this->nStack = nStack;
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
        cout << "hellosc2" << endl;
    };

    void rotate (Vec* v, double a) {
        double A = a * M_PI/180.0;
        rotatePoint (nPole, v, A), rotatePoint (sPole, v, A);
        for (int i=0; i<nSec; i++) rotatePoint (eqVerts[i], v, A);
        for (int k=1; k<nStack; k++) {
            for (int i=0; i<nSec; i++) {
                rotatePoint (upVerts[k][i], v, A);
                rotatePoint (downVerts[k][i], v, A);
            };
        };
    };
};

class Cylinder {
    public:
    int nSec;
    Point **upVerts, **downVerts;
    double r, h;
    Cylinder (double r, double h, int nSec) {
        this->r = r, this->h = h, this->nSec = nSec;
        double dTheta = 2 * M_PI / nSec;
        //start from pos x-axis CCW
        downVerts = new Point* [nSec];
        upVerts = new Point* [nSec];
        for (int i=0; i<nSec; i++) {
            downVerts[i] = new Point (r*cos(dTheta*i), r*sin(dTheta*i), 0);
            upVerts[i] = new Point (r*cos(dTheta*i), r*sin(dTheta*i), h);
        };
    };
};

class Cone {
    public:
    int nSec;
    Point **downVerts;
    double r, h;
    Cone (double r, double h, int nSec) {
        this->r = r, this->h = h, this->nSec = nSec;
        double dTheta = 2 * M_PI / nSec;
        //start from pos x-axis CCW
        downVerts = new Point* [nSec];
        for (int i=0; i<nSec; i++) {
            downVerts[i] = new Point (r*cos(dTheta*i), r*sin(dTheta*i), 0);
        };
    };
};

class CubeSphereFace {
    public:
    int nSubdiv, pointsPerRow;
    double r;
    Point ***verts;

    CubeSphereFace (double r, int nSubdiv) {
        this->nSubdiv = nSubdiv, this->r = r;
        pointsPerRow = (int)pow(2, this->nSubdiv) + 1;

        verts = new Point** [pointsPerRow];

        double n1[3];        // normal of longitudinal plane rotating along Y-axis
        double n2[3];        // normal of latitudinal plane rotating along Z-axis
        double v[3];         // direction vector intersecting 2 planes, n1 x n2
        double a1;           // longitudinal angle along Y-axis
        double a2;           // latitudinal angle along Z-axis

        // compute the number of vertices per row, 2^n + 1

        // rotate latitudinal plane from 45 to -45 degrees along Z-axis (top-to-bottom)
        for (int i = 0; i < pointsPerRow; i++) {

            verts[i] = new Point* [pointsPerRow];

            // normal for latitudinal plane
            // if latitude angle is 0, then normal vector of latitude plane is n2=(0,1,0)
            // therefore, it is rotating (0,1,0) vector by latitude angle a2
            a2 = M_PI/180.0 * (45.0f - 90.0f * i / (pointsPerRow - 1));
            n2[0] = -sin(a2);
            n2[1] = cos(a2);
            n2[2] = 0;

            // rotate longitudinal plane from -45 to 45 along Y-axis (left-to-right)
            for (int j = 0; j < pointsPerRow; j++) {
                // normal for longitudinal plane
                // if longitude angle is 0, then normal vector of longitude is n1=(0,0,-1)
                // therefore, it is rotating (0,0,-1) vector by longitude angle a1
                a1 = M_PI/180.0 * (-45.0f + 90.0f * j / (pointsPerRow - 1));
                n1[0] = -sin(a1);
                n1[1] = 0;
                n1[2] = -cos(a1);

                // find direction vector of intersected line, n1 x n2
                v[0] = n1[1] * n2[2] - n1[2] * n2[1];
                v[1] = n1[2] * n2[0] - n1[0] * n2[2];
                v[2] = n1[0] * n2[1] - n1[1] * n2[0];

                // normalize direction vector
                double scale = 1 / sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]) * this->r;
                v[0] *= scale;
                v[1] *= scale;
                v[2] *= scale;

                // add a vertex into array
                verts[i][j] = new Point (v[0], v[1], v[2]);
            }
        }

    };
};