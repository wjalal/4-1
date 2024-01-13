#include<bits/stdc++.h>

#ifdef __linux__
    #include<GL/glut.h>
#elif WIN32
    #include <windows.h>
    #include <glut.h>
#endif


using namespace std;

void init(){
    printf("Do your initialization here\n");
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    

}

// a openGL integer
GLint counter = 0;

void keyboardListener(unsigned char key,int x, int y){
    switch (key)
    {
    case 'K':
        /* code */
        printf("K pressed\n");
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

void display(){
    printf("Display function called for %d times\n", counter);
    
    // glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // Set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_LINES);{
        glColor3f(1.0f, 1.0f, 1.0f); // Green
        glVertex2f(-1.0f, 0.0f);
        glVertex2f(1.0f, 0.0f);
        glVertex2f(0.0f, -1.0f);
        glVertex2f(0.0f, 1.0f);
       
    }glEnd();


    // for(int i = 0; i < 8; i++){
        glPushMatrix();
        glRotatef((counter)%360 , 0, 0, 1);
        glTranslatef(0.5, 0.5, 0);
        glColor3f(1.0f, 0.0f, 0.0f); // Red
        drawSquare(0.1);
        glPopMatrix();
    // }
    
   


    // glFlush();
    glutSwapBuffers();

}

void idle(){

    counter++;
    glutPostRedisplay();
}

int main(int argc,char** argv){
    glutInit(&argc,argv);
    glutInitWindowSize(720, 450);   // Set the window's initial width & height
    glutInitWindowPosition(750, 250); // Position the window's initial top-left corner
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Test");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardListener);
    glutIdleFunc(idle);
    init();


    glutMainLoop();
    return 0;

}