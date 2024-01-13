#include <bits/stdc++.h>
#include <random>
#include "draw_geometry.hpp"
#include <chrono>

using namespace std::chrono;

Sphere ball (25.0, 16, 8);
Cylinder ballDirAx (1.5, 40.0, 8);
Cone ballDirHd (3.0, 10.0, 8);

double ballCtrlAngle = 45, ballPosX = 0, ballPosY = 0, boundary = 200;
double simVel = 300, mspf = 30, simStartPosX, simStartPosY, simD0 = 0;
bool simulation = false;
int simColId;
high_resolution_clock::time_point tStart;

Line lookAt (250, 250, 300, 0, 0, 0);

void init(){
    printf("Do your initialization here\n");
    drawaxes = 1;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(50, 1, 1, 2000.0);
    initCamVecs();
};

void drawBoard() {
    if (drawboard) {
        glBegin(GL_QUADS); {
            for (int i = -4000; i<+4000; i+=50) {
                for (int j = -4000; j<+4000; j+=50) {
                    if ((i+j)%100) glColor3f(0.2, 0.2, 0.2);
                    else glColor3f(0.8, 0.8, 0.8);
                    glVertex3f(i, j, 0);
                    glVertex3f(i+50, j, 0);
                    glVertex3f(i+50, j+50, 0);
                    if ((i+j)%100) glColor3f(0, 0, 0);
                    else glColor3f(1, 1, 1);
                    glVertex3f(i, j+50, 0);
                };
            };
        } glEnd();
    };
};

void drawWalls (double L, double h) {
    for (int i=0; i<4; i++) {
        glPushMatrix();
            glRotatef (90*i, 0, 0, 1);
            glBegin (GL_QUADS); {
                glColor3f (0.4, 0.2, 0.1);
                glVertex3f (-L, -L, h);
                glVertex3f (-L, -L, 0);
                glVertex3f (-L, +L, 0);
                glColor3f (0.6, 0.3, 0.15);
                glVertex3f (-L, +L, h);
            } glEnd();
        glPopMatrix();
    };
};

void rollBall (double a) {
    double A = ballCtrlAngle * M_PI/180;
    double d = a/360 * 2 * M_PI * ball.r;
    double dX = d*cos(A), dY = d*sin(A), dX0;

    dX0 = dX;
    if (fabs(ballPosX+dX) > boundary-ball.r) {
        dX = boundary-ball.r - fabs(ballPosX);
        dY = dX * tan(A);
        A = fmod (atan2(+sin(A),-cos(A)) + 2*M_PI, 2*M_PI);
    } else if (fabs(ballPosY+dY) > boundary-ball.r) {
        dY = boundary-ball.r - fabs(ballPosY);
        dX = dY / tan(A);
        A = fmod (atan2(-sin(A),+cos(A)) + 2*M_PI, 2*M_PI);
    };
    ballCtrlAngle = 180.0/M_PI * A;
    ballPosX += dX, ballPosY += dY;
    Vec* v = new Vec (-sin(A), cos(A), 0);
    ball.rotate (v, a*dX/dX0);    
};

void simSchedColl();

void simFlipX (int id) {
    if (id != simColId) return;
    double A = ballCtrlAngle * M_PI/180;
    A = fmod (atan2(+sin(A),-cos(A)) + 2*M_PI, 2*M_PI);
    ballCtrlAngle = 180.0/M_PI * A;
    simSchedColl();
};

void simFlipY (int id) {
    if (id != simColId) return;
    double A = ballCtrlAngle * M_PI/180;
    A = fmod (atan2(-sin(A),+cos(A)) + 2*M_PI, 2*M_PI);
    ballCtrlAngle = 180.0/M_PI * A;
    simSchedColl();
};

void simulateBall() {
    if (simulation) {
        high_resolution_clock::time_point t = high_resolution_clock::now();
        duration<double> diff = t - tStart;
        milliseconds ms = duration_cast<milliseconds>(diff);

        double A = ballCtrlAngle * M_PI/180.0, d = simVel / 1000.0 /360 * 2 * M_PI * ball.r * ms.count();
        ballPosX = simStartPosX + d*cos(A), ballPosY = simStartPosY + d*sin(A);
        double a = (d-simD0)/(ball.r * 2*M_PI) * 360;
        Vec* v = new Vec (-sin(A), cos(A), 0);
        ball.rotate (v, a);    
        delete v;
        simD0 = d;
    };
};

void simSchedColl() {
    simColId++;
    simStartPosX = ballPosX, simStartPosY = ballPosY;
    double a = simVel / 1000.0;
    double A = ballCtrlAngle * M_PI/180.0, d = a/360 * 2 * M_PI * ball.r;
    double dX = d*cos(A), dY = d*sin(A);

    double tX = fabs (((dX>0? +1:-1)*(boundary-ball.r)-ballPosX) / dX);
    double tY = fabs (((dY>0? +1:-1)*(boundary-ball.r)-ballPosY) / dY);
    // cout << tX << " " << tY << endl;
    if (tX<tY) glutTimerFunc (tX, simFlipX, simColId);
    else glutTimerFunc (tY, simFlipY, simColId);
    tStart = high_resolution_clock::now(); 
    simD0 = 0;
};

void keyboardListener(unsigned char key,int x, int y){
    camKeyboardListener(key);
    if (key == 'j') {
        ballCtrlAngle = fmod (ballCtrlAngle + 15 + 360, 360);
        if (simulation) simSchedColl();
    } else if (key == 'l') {
        ballCtrlAngle = fmod (ballCtrlAngle - 15 + 360, 360);
        if (simulation) simSchedColl();
    } else if (key == 'i') {
        rollBall(+15);
        if (simulation) simSchedColl();
    } else if (key == 'k') {
        rollBall(-15);
        if (simulation) simSchedColl();
    } else if (key == ' ') {
        simulation = !simulation;
        simColId = 0;
        if (simulation) simSchedColl();
    };
};


void specialKeyboardListener(int key,int x, int y){
    camSpecialKeyboardListener(key);
};

void display() {    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
   
    gluLookAt  (lookAt.p1->x, lookAt.p1->y, lookAt.p1->z, 
                lookAt.p2->x, lookAt.p2->y, lookAt.p2->z,  
                upVec->x, upVec->y, upVec->z);
    drawBoard();
    glPushMatrix();
        glScalef (1.01, 1.01, 1);
        drawWalls (boundary, 40);
    glPopMatrix();
    drawWalls (boundary, 40);

    glPushMatrix();       
        glTranslatef(ballPosX,ballPosY,ball.r);
        drawSphere(&ball);

        if (drawUpArrow) {
            glPushMatrix();
                glTranslatef(0, 0, ball.r);
                drawArrow(&ballDirAx, &ballDirHd, 1);
            glPopMatrix();
        };
        
        glPushMatrix();
            glRotatef(ballCtrlAngle, 0, 0 ,1);
            glPushMatrix();
                glRotatef(90, 0, 1, 0);
                drawArrow(&ballDirAx, &ballDirHd);
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();

    glutSwapBuffers();

};

void idle(){
    if (time(0)%2) drawUpArrow = true;
    else drawUpArrow = false;
    simulateBall();
    glutPostRedisplay();
};

int main(int argc,char** argv){
    glutInit(&argc,argv);
    glutInitWindowSize(800, 800);   // Set the window's initial width & height
    glutInitWindowPosition(1920+350, 125); // Position the window's initial top-left corner
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Rolling Ball : 1905084");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyboardListener);
    glutIdleFunc(idle);
    init();

    glutMainLoop();
    return 0;

};