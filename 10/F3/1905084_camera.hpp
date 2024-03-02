#include "1905084_Vec.hpp"
#include <iostream>
// Line lookAt (0, -0.1, 600, 0, 0, 0);
using namespace std;

extern Line lookAt;
extern Vec *upVec, *lookAtVec, *rightVec;

void zoomCamera (double d) {
    lookAt.p1->x += (d * lookAtVec->cosTx), lookAt.p1->y += (d * lookAtVec->cosTy), lookAt.p1->z += (d * lookAtVec->cosTz);
    lookAt.p2->x += (d * lookAtVec->cosTx), lookAt.p2->y += (d * lookAtVec->cosTy), lookAt.p2->z += (d * lookAtVec->cosTz);
    delete lookAtVec;
    lookAtVec = new Vec(&lookAt);
    lookAtVec->normalize();
};

void trnsltVertical (double d) {
    lookAt.p1->x += (d * upVec->x), lookAt.p1->y += (d * upVec->y), lookAt.p1->z += (d * upVec->z);
    lookAt.p2->x += (d * upVec->x), lookAt.p2->y += (d * upVec->y), lookAt.p2->z += (d * upVec->z);
    delete lookAtVec;
    lookAtVec = new Vec(&lookAt);
    lookAtVec->normalize();
};

void trnsltHorizontal (double d) {
    lookAt.p1->x += (d * rightVec->x), lookAt.p1->y += (d * rightVec->y), lookAt.p1->z += (d * rightVec->z);
    lookAt.p2->x += (d * rightVec->x), lookAt.p2->y += (d * rightVec->y), lookAt.p2->z += (d * rightVec->z);
    delete lookAtVec;
    lookAtVec = new Vec(&lookAt);
    lookAtVec->normalize();
};

void rotateHorizontal (double a) {      // rotate lookVec by angle a about upVec from eye
    double A = a * M_PI / 180.0;
    Vec* l = new Vec(&lookAt);
    Vec* u = upVec->cross(l);
    Vec* Rot = new Vec (cos(A)*l->x + sin(A)*u->x, 
                        cos(A)*l->y + sin(A)*u->y, 
                        cos(A)*l->z + sin(A)*u->z);
    lookAt.p2->x = lookAt.p1->x + Rot->x;
    lookAt.p2->y = lookAt.p1->y + Rot->y;
    lookAt.p2->z = lookAt.p1->z + Rot->z;
    delete lookAtVec;
    lookAtVec = new Vec(&lookAt);
    lookAtVec->normalize();
    delete rightVec;
    rightVec = lookAtVec->cross(upVec);
    rightVec->normalize();
};

void rotateVertical (double a) {      // rotate lookVec by angle a about rightVec from eye
    double A = a * M_PI / 180.0;
    Vec* l = new Vec(&lookAt);
    Vec* u = rightVec->cross(l);
    Vec* Rot = new Vec (cos(A)*l->x + sin(A)*u->x, 
                        cos(A)*l->y + sin(A)*u->y, 
                        cos(A)*l->z + sin(A)*u->z);
    lookAt.p2->x = lookAt.p1->x + Rot->x;
    lookAt.p2->y = lookAt.p1->y + Rot->y;
    lookAt.p2->z = lookAt.p1->z + Rot->z;
    delete lookAtVec;
    lookAtVec = new Vec(&lookAt);
    lookAtVec->normalize();
    delete upVec;
    upVec = rightVec->cross(lookAtVec);
    upVec->normalize();
}; 

void revolveVertical (double a) {      // rotate lookVec by angle a about rightVec from target
    double A = a * M_PI / 180.0;
    Vec* l = new Vec(&lookAt);
    Vec* u = rightVec->cross(l);
    Vec* Rot = new Vec (cos(A)*l->x + sin(A)*u->x, 
                        cos(A)*l->y + sin(A)*u->y, 
                        cos(A)*l->z + sin(A)*u->z);
    lookAt.p1->x = lookAt.p2->x - Rot->x;
    lookAt.p1->y = lookAt.p2->y - Rot->y;
    lookAt.p1->z = lookAt.p2->z - Rot->z;
    delete lookAtVec;
    lookAtVec = new Vec(&lookAt);
    lookAtVec->normalize();
    delete upVec;
    upVec = rightVec->cross(lookAtVec);
    upVec->normalize();
}; 

void revolveHorizontal (double a) {      // rotate lookVec by angle a about upVec from target
    double A = a * M_PI / 180.0;
    Vec* l = new Vec(&lookAt);
    Vec* u = upVec->cross(l);
    Vec* Rot = new Vec (cos(A)*l->x + sin(A)*u->x, 
                        cos(A)*l->y + sin(A)*u->y, 
                        cos(A)*l->z + sin(A)*u->z);
    lookAt.p1->x = lookAt.p2->x - Rot->x;
    lookAt.p1->y = lookAt.p2->y - Rot->y;
    lookAt.p1->z = lookAt.p2->z - Rot->z;
    delete lookAtVec;
    lookAtVec = new Vec(&lookAt);
    lookAtVec->normalize();
    delete rightVec;
    rightVec = lookAtVec->cross(upVec);
    rightVec->normalize();
}; 


void tilt (double a) {      // rotate upVec by angle a about lookVec
    double A = a * M_PI / 180.0;
    Vec* r = new Vec (lookAtVec->x, lookAtVec->y, lookAtVec->z);
    r->normalize();
    Vec* u = r->cross(upVec);
    Vec* Rot = new Vec (cos(A)*upVec->x + sin(A)*u->x, 
                        cos(A)*upVec->y + sin(A)*u->y, 
                        cos(A)*upVec->z + sin(A)*u->z);
    delete r; delete u;
    delete upVec;
    upVec = Rot;
    upVec->normalize();
    delete rightVec;
    rightVec = lookAtVec->cross(upVec);
    rightVec->normalize();
}; 

void initCamVecs() {
    lookAtVec = new Vec(&lookAt);
    lookAtVec->normalize();
    double fZ = sqrt ((lookAtVec->D)*(lookAtVec->D) - (lookAtVec->z)*(lookAtVec->z));
    // cout << upVecZ << ", " << fZ/lookAtVec->D << endl;
    double upVecZ = fZ/lookAtVec->D;
    double uZc = sqrt (1 - upVecZ*upVecZ); 
    double upVecX = (lookAtVec->z > 0 ? -1:+1) * lookAtVec->x/fZ *  uZc;
    double upVecY = (lookAtVec->z > 0 ? -1:+1) * lookAtVec->y/fZ *  uZc;
    upVec = new Vec (upVecX, upVecY, upVecZ);
    upVec->normalize();
    rightVec = lookAtVec->cross(upVec);
    rightVec->normalize();
    cout << lookAtVec->x << ", " << lookAtVec->y << ", " << lookAtVec->z << endl;
    cout << upVec->x << ", " << upVec->y << ", " << upVec->z << endl;
    cout << rightVec->x << ", " << rightVec->y << ", " << rightVec->z << endl;
};

void camKeyboardListener (unsigned char key) {
    switch (key) {
        case '1': rotateHorizontal(+2.5);  break;
        case '2': rotateHorizontal(-2.5);  break;
        case '3': rotateVertical(+2.5);    break;
        case '4': rotateVertical(-2.5);    break;
        case '5': tilt(+2.5); break;
        case '6': tilt(-2.5); break;
        case 'w': revolveVertical(-2.5);   break;
        case 's': revolveVertical(+2.5);   break;
        case 'q': revolveHorizontal(-2.5); break;
        case 'e': revolveHorizontal(+2.5); break;
        default : break;
    };
    cout << lookAtVec->x << ", " << lookAtVec->y << ", " << lookAtVec->z << " : " << lookAtVec->D << endl;
    cout << upVec->x << ", " << upVec->y << ", " << upVec->z << " : " << upVec->D << endl;
    cout << rightVec->x << ", " << rightVec->y << ", " << rightVec->z << " : " << rightVec->D << endl;
};

void camSpecialKeyboardListener (int key) {
    switch (key) {
        case GLUT_KEY_UP:  zoomCamera(+5); break;
        case GLUT_KEY_DOWN: zoomCamera(-5); break;
        case GLUT_KEY_LEFT: trnsltHorizontal(-5); break;
        case GLUT_KEY_RIGHT: trnsltHorizontal(+5); break;
        case GLUT_KEY_PAGE_UP: trnsltVertical(+5); break;
        case GLUT_KEY_PAGE_DOWN: trnsltVertical(-5); break;
        default: break;
    };

    cout << lookAtVec->x << ", " << lookAtVec->y << ", " << lookAtVec->z << " : " << lookAtVec->D << endl;
    cout << upVec->x << ", " << upVec->y << ", " << upVec->z << " : " << upVec->D << endl;
    cout << rightVec->x << ", " << rightVec->y << ", " << rightVec->z << " : " << rightVec->D << endl;
};