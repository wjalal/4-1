#include<iostream>
#include<GL/glut.h>
#include<math.h>
#include<bits/stdc++.h>

#define PI acos(-1.0)
#define degToRad(x) (x * PI / 180.0)
#define radToDeg(x) (x * 180.0 / PI)

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640

typedef struct {
    double x, y;
} Point;

double radius = 0.5;
double anim_x = 0.25;

using namespace std;

void drawCircle(double radius, Point center){
    glBegin(GL_LINE_LOOP);
    for(int i=0; i<360; i++){
        double angle = degToRad(i);
        double x = center.x + radius*cos(angle);
        double y = center.y + radius*sin(angle);
        glVertex2d(x, y);
    }
    glEnd();
}

double angle = 0, angle2 = 0, angle3 = 90;

void drawFunc(function<double(double)> func, double lx, double rx, int prec){
    if(prec==0){
        //drawSegment(point(lx,func(lx),0),point(rx,func(rx),0));

        glBegin(GL_LINES);
        glVertex2d(lx, func(lx));
        glVertex2d(rx, func(rx));
        glEnd();
        return;
    }
    drawFunc(func,lx,(lx+rx)/2,prec-1);
    drawFunc(func,(lx+rx)/2,rx,prec-1);
    return;
}
void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    // draw circle

    glColor3f(0, 0, 1);
    drawCircle(radius, {0, 0});

    //draw a line from center to a point on the circle
    // glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    Point center = {0, 0};
    glVertex2f(center.x, center.y);
    Point marker = {center.x + radius*cos(degToRad(angle)), center.y + radius*sin(degToRad(angle))};
    glVertex2f(marker.x, marker.y);
    glEnd();

    glColor3f(1, 0, 0);
    double radius2 = 0.4*radius;
    drawCircle(radius2, {marker.x, marker.y});
    glBegin(GL_LINES);
    Point center2 = marker;
    glVertex2f(center2.x, center2.y);
    Point marker2 = {center2.x + radius2*cos(degToRad(angle2)), center2.y + radius2*sin(degToRad(angle2))};
    glVertex2f(marker2.x, marker2.y);
    glEnd();
    // vary the angle of the line and draw the line continuously such that it looks like a rotating line
   
    
    glColor3f(1, 1, 0);
    double radius3 = 0.3*radius2;
    drawCircle(radius3, {marker2.x, marker2.y});


    // // now that we have drawn the line and the circle, we will draw a sine wave that will be traced by the line

    // Point rightMarker = {0.25,marker.y};
    // glBegin(GL_LINES);
    // glColor3f(1, 1, 1);
    // glVertex2d(marker.x, marker.y);
    // glVertex2d(rightMarker.x, rightMarker.y);
    // glEnd();

    // glBegin(GL_POINTS); {
    //     for (double x=rightMarker.x; x <= anim_x; x+=1.0/640.0 ) {
    //         glVertex2f(x, -0.5*sin( 14.0*(x - rightMarker.x) - degToRad(angle)));
    //     }
    // } glEnd();

    glFlush();
}

void idle(){

    angle += 0.0025;
    if(angle > 360) angle = 0;

    angle2 += 0.0075;
    if(angle2 > 360) angle2 = 0;

    glutPostRedisplay();
    
}

/* Callback handler for normal-key event */
void keyboardListener(unsigned char key, int xx, int yy)
{
    switch (key){

    case 'a': 
        break;

    default:
        return;
    }

    glutPostRedisplay();
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);  
    glutInitWindowPosition(0, 0);
    glutCreateWindow("opengl");
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboardListener);
    // anim8(0);
    glutMainLoop();
    return 0;
}
