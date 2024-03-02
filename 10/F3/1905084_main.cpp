#include <random>
#include "1905084_geometry.hpp"
#include <chrono>
#include <string> 
#include <fstream>
#include "1905084_bitmap_image.hpp"
#include <bits/stdc++.h>

using namespace std::chrono;

int recLevel = 0, screenHeight, screenWidth, captureId = 11; 
double fovY = 90;

// Line lookAt (0, -90, 20, -30, 60, 20);
Line lookAt (-10, -90, 50, 10, 30, 70);
Vec *upVec, *lookAtVec, *rightVec;

ifstream sceneFile;
vector <Object*> objects;
vector <PointLight*> pointLights;
vector <SpotLight*> spotLights;

void init(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(fovY, 1, 1, 1000.0);
    initCamVecs();
    tilt(7.5);

};

void display() {    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
   
    gluLookAt  (lookAt.p1->x, lookAt.p1->y, lookAt.p1->z, 
                lookAt.p2->x, lookAt.p2->y, lookAt.p2->z,  
                upVec->x, upVec->y, upVec->z);

    for (int i=0; i<objects.size(); i++) {
        objects[i]->draw();
    };

    for (int i=0; i<pointLights.size(); i++) {
        pointLights[i]->draw();
    };

    for (int i=0; i<spotLights.size(); i++) {
        spotLights[i]->draw();
    }; 

    glutSwapBuffers();

};

void idle(){
    // if (time(0)%2) drawUpArrow = true;
    // else drawUpArrow = false;
    // simulateBall();
    glutPostRedisplay();
};

void loadData() {
    int nObj, nPointLight, nSpotLight;
    sceneFile >> recLevel >>  screenWidth >> nObj;
    screenHeight = screenWidth;
    string shape;
    double r, g, b, ambi, diff, spec, refl, shine;
    for (int i=0; i<nObj; i++) {
        sceneFile >> shape;
        double x, y, z, R;
        Object* obj;
        if (shape == "sphere") {
            sceneFile >> x >> y >> z >> R;
            Vec* center = new Vec (x, y, z);
            obj = new Sphere (center, R);
        } else if (shape == "triangle") {
            sceneFile >> x >> y >> z;
            Vec* p1 = new Vec (x, y ,z);
            sceneFile >> x >> y >> z;
            Vec* p2 = new Vec (x, y ,z);
            sceneFile >> x >> y >> z;
            Vec* p3 = new Vec (x, y ,z);
            obj = new Triangle (p1, p2, p3);
        } else if (shape == "general") {
            double A, B, C, D, E, F, G, H, I, J, l, w, h;
            sceneFile >> A >> B >> C >> D >> E >> F >> G >> H >> I >> J;
            sceneFile >> x >> y >> z >> l >> w >> h;
            Vec* p = new Vec (x, y ,z);
            obj = new General (p, l, w, h, A, B, C, D, E, F, G, H, I, J);
        };
        sceneFile >> r >> g >> b >> ambi >> diff >> spec >> refl >> shine;
        obj->setColor (r, g, b);
        obj->setCoeff (ambi, diff, spec, refl);
        obj->setShine (shine);
        objects.push_back(obj);
    };
    sceneFile >> nPointLight;
    for (int i=0; i<nPointLight; i++) {
        double x, y, z;
        sceneFile >> x >> y >> z;
        Vec* p = new Vec (x, y ,z);
        PointLight* light = new PointLight(p);
        sceneFile >> r >> g >> b;
        light->setColor (r, g, b);
        pointLights.push_back(light);
    };
    sceneFile >> nSpotLight;
    for (int i=0; i<nSpotLight; i++) {
        double x, y, z, c;
        sceneFile >> x >> y >> z;
        Vec* p = new Vec (x, y ,z);
        sceneFile >> r >> g >> b;
        sceneFile >> x >> y >> z;
        Vec* dir = new Vec (x, y ,z);
        sceneFile >> c;
        SpotLight* light = new SpotLight(p, dir, c);
        light->setColor (r, g, b);
        spotLights.push_back(light);
    };
};


void capture() {
    int imageWidth = screenWidth, imageHeight = screenHeight;
    bitmap_image image (imageWidth, imageHeight);
    image.set_all_channels (0, 0, 0);

    double planeDistance = 1 / tan (fovY*M_PI/180.0/2.0);
    Vec topLeft(0,0,0), eye(lookAt.p1->x, lookAt.p1->y, lookAt.p1->z);
    topLeft = eye + (*lookAtVec)*planeDistance + (*rightVec)*(-1) + (*upVec); 
    double du = 2.0/imageWidth, dv = 2.0/imageHeight;
    topLeft = topLeft + (*rightVec)*(0.5*du) + (*upVec)*(-0.5*dv);

    for (int i=0; i<imageWidth; i++) {
        for (int j=0; j<imageHeight; j++) {
            Object* nearest = nullptr;
            double tMin = INFINITY;
            Vec* curPixel = new Vec(0,0,0);
            *curPixel = topLeft + (*rightVec)*(i*du) + (*upVec)*(-j*dv);
            *curPixel = *curPixel + eye*(-1);
            Ray* ray = new Ray (eye.x, eye.y, eye.z, curPixel->x, curPixel->y, curPixel->z);
            double* color = nullptr;
            for (int k=0; k<objects.size(); k++) {
                double* oColor = new double[3];
                double t = objects[k]->intersect(ray, oColor, 1);
                // if (t != -1 && k==8) cout << "k = " << k << " t = " << t << endl;
                if (t>0 && t<tMin) {
                    nearest = objects[k], tMin = t;
                    if (color) delete[] color;
                    color = oColor;
                } else delete[] oColor;
            };
            if (nearest) {
                // if (nearest == objects[8]) cout << 255.0*color[RED] << " " << 255.0*color[GRN] << " " << 255.0*color[BLU] << endl;
                for (int c=0; c<3; c++) if (color[c] > 1) color[c] = 1;
                image.set_pixel (i, j, 255.0*color[RED], 255.0*color[GRN], 255.0*color[BLU]);
                delete[] color;
            };
            delete ray; delete curPixel;
        };
    };

    string name = "images/Output_" + to_string(captureId) + ".bmp";
    image.save_image (name);
    image.clear();
    cout << "image " << captureId << " saved" << endl;
    captureId++;
};


void keyboardListener(unsigned char key,int x, int y){
    camKeyboardListener(key);
    if (key == '0') capture();
};


void specialKeyboardListener(int key,int x, int y){
    camSpecialKeyboardListener(key);
};

void onexit() {
    for (int i=0; i<objects.size(); i++) delete objects[i];
    for (int i=0; i<pointLights.size(); i++) delete pointLights[i];
    for (int i=0; i<spotLights.size(); i++) delete spotLights[i];
    vector<Object*>().swap(objects);
	vector<PointLight*>().swap(pointLights);
	vector<SpotLight*>().swap(spotLights);
    cout << "Bye" << endl; 
};
 
int main(int argc,char** argv) {
    sceneFile.open("scene.txt");
    loadData();
    objects.push_back (new Floor(1000, 20));

    glutInit(&argc,argv);
    glutInitWindowSize(screenWidth, screenHeight);  
    glutInitWindowPosition(1920+350, 125); 
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Raytracing : 1905084");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyboardListener);
    glutIdleFunc(idle);
    init();
    atexit(onexit);
    glutMainLoop();
    return 0;

};