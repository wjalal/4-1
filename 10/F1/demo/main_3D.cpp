#include<bits/stdc++.h>
#include<math.h>
#include<random>
#ifdef __linux__
    #include<GL/glut.h>
#elif WIN32
    #include <windows.h>
    #include <glut.h>
#endif


using namespace std;
int drawaxes;


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
        cosTx = x/D, cosTy = x/D, cosTz = x/D;
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

    void normalize() {
        this->x /= D, this->y /= D; this->z /= D;
        updateCalc();
    }
};

Line lookAt (100, 100, 100, 0, 0, 0);
Vec *upVec, *lookAtVec, *rightVec;

void initCamVecs() {
    lookAtVec = new Vec(&lookAt);
    cout << lookAtVec->x << ", " << lookAtVec->y << ", " << lookAtVec->z << endl;
    double fZ = sqrt ((lookAtVec->D)*(lookAtVec->D) - (lookAtVec->z)*(lookAtVec->z));
    // cout << upVecZ << ", " << fZ/lookAtVec->D << endl;
    double upVecZ = fZ/lookAtVec->D;
    double uZc = sqrt (1 - upVecZ*upVecZ); 
    double upVecX = (lookAtVec->z > 0 ? -1:+1) * lookAtVec->x/fZ *  uZc;
    double upVecY = (lookAtVec->z > 0 ? -1:+1) * lookAtVec->y/fZ *  uZc;
    upVec = new Vec (upVecX, upVecY, upVecZ);
    cout << upVec->x << ", " << upVec->y << ", " << upVec->z << endl;
    rightVec = lookAtVec->cross(upVec);
    rightVec->normalize();
    cout << rightVec->x << ", " << rightVec->y << ", " << rightVec->z << endl;
}

void init(){
    printf("Do your initialization here\n");
    drawaxes = 1;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(80, 1, 1, 1000.0);
    initCamVecs();
}

void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 0.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void zoomCamera (double d) {
    lookAt.p1->x += (d * lookAtVec->cosTx), lookAt.p1->y += (d * lookAtVec->cosTx), lookAt.p1->z += (d * lookAtVec->cosTx);
    lookAt.p2->x += (d * lookAtVec->cosTx), lookAt.p2->y += (d * lookAtVec->cosTx), lookAt.p2->z += (d * lookAtVec->cosTx);
    delete lookAtVec;
    lookAtVec = new Vec(&lookAt);
    // cout << lookAt.p1->x << ", " << lookAt.p1->y << ", " << lookAt.p1->z << endl;
    glutSwapBuffers(); 
}

void trnsltVertical (double d) {
    lookAt.p1->x += (d * upVec->x), lookAt.p1->y += (d * upVec->y), lookAt.p1->z += (d * upVec->z);
    lookAt.p2->x += (d * upVec->x), lookAt.p2->y += (d * upVec->y), lookAt.p2->z += (d * upVec->z);
    delete lookAtVec;
    lookAtVec = new Vec(&lookAt);
}

void trnsltHorizontal (double d) {
    lookAt.p1->x += (d * rightVec->x), lookAt.p1->y += (d * rightVec->y), lookAt.p1->z += (d * rightVec->z);
    lookAt.p2->x += (d * rightVec->x), lookAt.p2->y += (d * rightVec->y), lookAt.p2->z += (d * rightVec->z);
    delete lookAtVec;
    lookAtVec = new Vec(&lookAt);
}

void rotateHorizontal (double a) {      // rotate lookVec by angle a about upVec from eye
    double A = a * M_1_PI / 180.0;
    Vec* u = upVec->cross(lookAtVec);
    Vec* Rot = new Vec (cos(A)*lookAtVec->x + sin(A)*u->x, 
                        cos(A)*lookAtVec->y + sin(A)*u->y, 
                        cos(A)*lookAtVec->z + sin(A)*u->z);
    lookAt.p2->x = lookAt.p1->x + Rot->x;
    lookAt.p2->y = lookAt.p1->y + Rot->y;
    lookAt.p2->z = lookAt.p1->z + Rot->z;
    delete lookAtVec;
    lookAtVec = new Vec(&lookAt);
    delete rightVec;
    rightVec = lookAtVec->cross(upVec);
    rightVec->normalize();
} 

void rotateVertical (double a) {      // rotate lookVec by angle a about rightVec from eye
    double A = a * M_1_PI / 180.0;
    Vec* u = rightVec->cross(lookAtVec);
    Vec* Rot = new Vec (cos(A)*lookAtVec->x + sin(A)*u->x, 
                        cos(A)*lookAtVec->y + sin(A)*u->y, 
                        cos(A)*lookAtVec->z + sin(A)*u->z);
    lookAt.p2->x = lookAt.p1->x + Rot->x;
    lookAt.p2->y = lookAt.p1->y + Rot->y;
    lookAt.p2->z = lookAt.p1->z + Rot->z;
    delete lookAtVec;
    lookAtVec = new Vec(&lookAt);
    delete upVec;
    upVec = rightVec->cross(lookAtVec);
    upVec->normalize();
} 

void revolveVertical (double a) {      // rotate lookVec by angle a about rightVec from target
    double A = a * M_1_PI / 180.0;
    Vec* u = rightVec->cross(lookAtVec);
    Vec* Rot = new Vec (cos(A)*lookAtVec->x + sin(A)*u->x, 
                        cos(A)*lookAtVec->y + sin(A)*u->y, 
                        cos(A)*lookAtVec->z + sin(A)*u->z);
    lookAt.p1->x = lookAt.p2->x - Rot->x;
    lookAt.p1->y = lookAt.p2->y - Rot->y;
    lookAt.p1->z = lookAt.p2->z - Rot->z;
    delete lookAtVec;
    lookAtVec = new Vec(&lookAt);
    delete upVec;
    upVec = rightVec->cross(lookAtVec);
    upVec->normalize();
} 

void tilt (double a) {      // rotate upVec by angle a about lookVec
    double A = a * M_1_PI / 180.0;
    Vec* r = new Vec (lookAtVec->x, lookAtVec->y, lookAtVec->z);
    r->normalize();
    Vec* u = r->cross(upVec);
    Vec* Rot = new Vec (cos(A)*upVec->x + sin(A)*u->x, 
                        cos(A)*upVec->y + sin(A)*u->y, 
                        cos(A)*upVec->z + sin(A)*u->z);
    delete upVec;
    upVec = Rot;
    delete rightVec;
    rightVec = lookAtVec->cross(upVec);
    rightVec->normalize();
} 

// a openGL integer
GLint counter = 0;

void keyboardListener(unsigned char key,int x, int y){
    switch (key)
    {
    case '1':
        rotateHorizontal(15);
        break;
    case '2':
        rotateHorizontal(-15);
        break;
    case '3':
        rotateVertical(15);
        break;
    case '4':
        rotateVertical(-15);
        break;
    case '5':
        tilt(15);
        break;
    case '6':
        tilt(-15);
        break;
    case 'w':
        revolveVertical(-15);
        break;
    case 's':
        revolveVertical(+15);
        break;
    default:
        printf("We don't know what you pressed\n");
        break;
    }
}


void specialKeyboardListener(int key,int x, int y){
    switch (key)
    {
    case GLUT_KEY_UP:
        zoomCamera(+5);
        break;
    case GLUT_KEY_DOWN:
        zoomCamera(-5);
        break;
    case GLUT_KEY_LEFT:
        trnsltHorizontal(-5);
        break;
    case GLUT_KEY_RIGHT:
        trnsltHorizontal(+5);
        break;
    case GLUT_KEY_PAGE_UP:
        trnsltVertical(+5);
        break;
    case GLUT_KEY_PAGE_DOWN:
        trnsltVertical(-5);
        break;
    default:
        printf("We don't know what you pressed\n");
        break;
    }
}

void drawSquare(double a){
    glBegin(GL_QUADS);{
        glVertex3f( a, a,0);
        glVertex3f( a,-a,0);
        glVertex3f(-a,-a,0);
        glVertex3f(-a, a,0);
    }glEnd();
}

double _rand(){
    return (double)rand() / RAND_MAX;
}

void display(){
    printf("Display function called for %d times\n", counter);
    
    // glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // Set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
   
    gluLookAt  (lookAt.p1->x, lookAt.p1->y, lookAt.p1->z, 
                lookAt.p2->x, lookAt.p2->y, lookAt.p2->z,  
                upVec->x, upVec->y, upVec->z);
    drawAxes();

    glBegin(GL_QUADS); {
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(20,20,20);
        glVertex3f(20,-20,20);
        glVertex3f(-20,-20,20);
        glVertex3f(-20,20,20);
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(20,20,20);
        glVertex3f(20,20,-20);
        glVertex3f(20,-20,-20);
        glVertex3f(20,-20,20);
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(20,-20,20);
        glVertex3f(-20,-20,20);
        glVertex3f(-20,-20,-20);
        glVertex3f(20,-20,-20);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(-20,-20,-20);
        glVertex3f(-20,20,-20);
        glVertex3f(20,20,-20);
        glVertex3f(20,-20,-20);
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(-20,-20,-20);
        glVertex3f(-20,-20,20);
        glVertex3f(-20,20,20);
        glVertex3f(-20,20,-20);
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(-20,20,-20);
        glVertex3f(20,20,-20);
        glVertex3f(20,20,20);
        glVertex3f(-20,20,20);
    } glEnd();

    // glFlush();
    glutSwapBuffers();

}

void idle(){

    counter++;
    glutPostRedisplay();
}

int main(int argc,char** argv){
    glutInit(&argc,argv);
    glutInitWindowSize(450, 450);   // Set the window's initial width & height
    glutInitWindowPosition(1920+750, 0+250); // Position the window's initial top-left corner
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Test");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyboardListener);
    glutIdleFunc(idle);
    init();


    glutMainLoop();
    return 0;

}