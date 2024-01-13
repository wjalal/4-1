#include <bits/stdc++.h>
#include <random>
#include "draw_geometry.hpp"

CubeSphereFace ss (sqrt(3), 5);
Cylinder cs (sqrt(3), sqrt(2), 20);

double transFactor = 1.0, rotAngle = 0.0;

Line lookAt (200, 200, 200, 0, 0, 0);

void init(){
    printf("Do your initialization here\n");
    drawaxes = 1;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(50, 1, 1, 2000.0);
    initCamVecs();
};

void drawOppVertSpheres (int colVar = 0) {
    for (int k=0; k<2; k++) {
        glPushMatrix();
            glRotatef (180*k, 1, 0, 0);
            glPushMatrix();
                glTranslatef (0, 0, transFactor);
                double ssScaleFactor = (1 - transFactor)/3.0;
                glScalef (ssScaleFactor, ssScaleFactor, ssScaleFactor);
                glRotatef (90, 0, -1, 0);
                drawCubeSphereFace(&ss, colVar);
            glPopMatrix();
        glPopMatrix();
    };
};

void drawSlantEdgeCylinders() {
    for (int k=0; k<2; k++) {
        glPushMatrix();
        glRotatef (180*k, 1, 0, 0);
        for (int i=0; i<4; i++) {
            glPushMatrix();
                // glTranslatef (+1.0/3.0, +1.0/3.0, 0);
                glRotatef (90*i, 0, 0, 1);
                glPushMatrix();
                    glTranslatef (+0, +0, +transFactor);
                    glRotatef (45, 1, 0, 0);
                    glTranslatef (-0, -0, -transFactor);
                    glTranslatef (0, 0, transFactor*(1-cs.h/2.0));
                    glScalef ((1-transFactor)/3.0, (1-transFactor)/3.0, transFactor);
                    glTranslatef (0, 0, -cs.h/2.0);
                    drawCylinder(&cs, 2);
                glPopMatrix();
            glPopMatrix();
        };
        glPopMatrix();
    };
};

void drawFlatEdgeCylinders() {
    for (int k=0; k<2; k++) { 
        glPushMatrix();
        glRotatef (90*k, 0, 0, 1);
        for (int i=0; i<2; i++) {
            glPushMatrix();
            glTranslatef (-transFactor*i, -transFactor*i, 0);
            glTranslatef (0.5*transFactor, 0.5*transFactor, 0);
                glPushMatrix();
                    glRotatef (90, 1, 1, 0);
                    glScalef ((1-transFactor)/3.0, (1-transFactor)/3.0, transFactor);
                    glTranslatef (0, 0, -cs.h/2.0);
                    drawCylinder(&cs, 2);
                glPopMatrix();
            glPopMatrix();
        };
        glPopMatrix();
    };
};

void drawOctahedron() {
    glPushMatrix();
        glScalef (80, 80, 80);

        for (int k=0; k<2; k++) {
            glPushMatrix();
            glRotatef (180*k, 1, 0, 0);
            for (int i=0; i<4; i++) {
                glPushMatrix();
                glRotatef (90*i, 0, 0, 1);

                glTranslatef (+1.0/3.0, +1.0/3.0, +1.0/3.0);
                glScalef (transFactor, transFactor, transFactor);
                glTranslatef (-1.0/3.0, -1.0/3.0, -1.0/3.0);

                drawOftahedronFace(i%2);

                glPopMatrix();
            };
            glPopMatrix();
        };

        drawOppVertSpheres(0);
        for (int i=0; i<2; i++) {
            glPushMatrix();
                glRotatef (90, 1-i, i, 0);
                drawOppVertSpheres(i+1);
            glPopMatrix();
        };

        drawSlantEdgeCylinders();
        drawFlatEdgeCylinders();
    glPopMatrix();
};

void keyboardListener(unsigned char key,int x, int y){
    camKeyboardListener(key);
    if (key == ',') {
        if (transFactor > 0.05) transFactor -= 0.0625;
    } else if (key == '.') {
        if (transFactor < 0.95) transFactor += 0.0625;
    } else if (key == 'a') {
        rotAngle = fmod (rotAngle+360 + 5, 360);
     }else if (key == 'd') {
        rotAngle = fmod (rotAngle+360 - 5, 360);
    };
};


void specialKeyboardListener(int key,int x, int y){
    camSpecialKeyboardListener(key);
};


double _rand(){
    return (double)rand() / RAND_MAX;
};

void display() {    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
   
    gluLookAt  (lookAt.p1->x, lookAt.p1->y, lookAt.p1->z, 
                lookAt.p2->x, lookAt.p2->y, lookAt.p2->z,  
                upVec->x, upVec->y, upVec->z);

    glPushMatrix();
        glRotatef (rotAngle, 0, 0, 1);
        drawOctahedron();
    glPopMatrix();
    glutSwapBuffers();
};

void idle(){
    if (time(0)%2) drawUpArrow = true;
    else drawUpArrow = false;
    glutPostRedisplay();
};

int main(int argc,char** argv){
    glutInit(&argc,argv);
    glutInitWindowSize(800, 800);   // Set the window's initial width & height
    glutInitWindowPosition(1920+350, 125); // Position the window's initial top-left corner
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Magic Cube : 1905084");
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyboardListener);
    glutIdleFunc(idle);
    init();

    glutMainLoop();
    return 0;
};