#include "geometry.hpp"

void drawSphere (Sphere* s) {
    int nSec = s->nSec, nStack = s->nStack;
    glBegin (GL_QUADS); {
        for (int k=1; k < nStack-1; k++) {
            for (int i=0; i < nSec; i++) {
                if (i%2)  glColor3f (0.8, 0.0, 0.0);
                else glColor3f (1.0, 0.8, 0.0);
                glVertex3f (s->upVerts[k+1][i]->x, s->upVerts[k+1][i]->y, s->upVerts[k+1][i]->z);
                glVertex3f (s->upVerts[k][i]->x, s->upVerts[k][i]->y, s->upVerts[k][i]->z);
                if (i%2)  glColor3f (1.0, 0.0, 0.0);
                else glColor3f (0.8, 0.6, 0.0);
                glVertex3f (s->upVerts[k][(i+1)%nSec]->x, s->upVerts[k][(i+1)%nSec]->y, s->upVerts[k][(i+1)%nSec]->z);
                glVertex3f (s->upVerts[k+1][(i+1)%nSec]->x, s->upVerts[k+1][(i+1)%nSec]->y, s->upVerts[k+1][(i+1)%nSec]->z);

                if (!(i%2))  glColor3f (0.8, 0.0, 0.0);
                else glColor3f (1.0, 0.8, 0.0);
                glVertex3f (s->downVerts[k+1][i]->x, s->downVerts[k+1][i]->y, s->downVerts[k+1][i]->z);
                glVertex3f (s->downVerts[k][i]->x, s->downVerts[k][i]->y, s->downVerts[k][i]->z);
                if (!(i%2))  glColor3f (1.0, 0.0, 0.0);
                else glColor3f (0.8, 0.6, 0.0);
                glVertex3f (s->downVerts[k][(i+1)%nSec]->x, s->downVerts[k][(i+1)%nSec]->y, s->downVerts[k][(i+1)%nSec]->z);
                glVertex3f (s->downVerts[k+1][(i+1)%nSec]->x, s->downVerts[k+1][(i+1)%nSec]->y, s->downVerts[k+1][(i+1)%nSec]->z);
            };
        };
        for (int i=0; i < nSec; i++) {
            if (i%2)  glColor3f (0.8, 0.0, 0.0);
            else glColor3f (1.0, 0.8, 0.0);
            glVertex3f (s->upVerts[1][i]->x, s->upVerts[1][i]->y, s->upVerts[1][i]->z);
            glVertex3f (s->eqVerts[i]->x, s->eqVerts[i]->y, s->eqVerts[i]->z);
            if (i%2)  glColor3f (1.0, 0.0, 0.0);
            else glColor3f (0.8, 0.6, 0.0);
            glVertex3f (s->eqVerts[(i+1)%nSec]->x, s->eqVerts[(i+1)%nSec]->y, s->eqVerts[(i+1)%nSec]->z);
            glVertex3f (s->upVerts[1][(i+1)%nSec]->x, s->upVerts[1][(i+1)%nSec]->y, s->upVerts[1][(i+1)%nSec]->z);

            if (!(i%2))  glColor3f (0.8, 0.0, 0.0);
            else glColor3f (1.0, 0.8, 0.0);
            glVertex3f (s->downVerts[1][i]->x, s->downVerts[1][i]->y, s->downVerts[1][i]->z);
            glVertex3f (s->eqVerts[i]->x, s->eqVerts[i]->y, s->eqVerts[i]->z);
            if (!(i%2))  glColor3f (1.0, 0.0, 0.0);
            else glColor3f (0.8, 0.6, 0.0);
            glVertex3f (s->eqVerts[(i+1)%nSec]->x, s->eqVerts[(i+1)%nSec]->y, s->eqVerts[(i+1)%nSec]->z);
            glVertex3f (s->downVerts[1][(i+1)%nSec]->x, s->downVerts[1][(i+1)%nSec]->y, s->downVerts[1][(i+1)%nSec]->z);
        };

    } glEnd();
    glBegin (GL_TRIANGLES); {
        for (int i=0; i < nSec; i++) {
            if (i%2)  glColor3f (0.8, 0.0, 0.0);
            else glColor3f (1.0, 0.8, 0.0);
            glVertex3f (s->nPole->x, s->nPole->y, s->nPole->z);
            glVertex3f (s->upVerts[nStack-1][i]->x, s->upVerts[nStack-1][i]->y, s->upVerts[nStack-1][i]->z);
            if (i%2)  glColor3f (1.0, 0.0, 0.0);
            else glColor3f (0.8, 0.6, 0.0);
            glVertex3f (s->upVerts[nStack-1][(i+1)%nSec]->x, s->upVerts[nStack-1][(i+1)%nSec]->y, s->upVerts[nStack-1][(i+1)%nSec]->z);

            if (!(i%2))  glColor3f (0.8, 0.0, 0.0);
            else glColor3f (1.0, 0.8, 0.0);
            glVertex3f (s->sPole->x, s->sPole->y, s->sPole->z);
            glVertex3f (s->downVerts[nStack-1][i]->x, s->downVerts[nStack-1][i]->y, s->downVerts[nStack-1][i]->z);
            if (!(i%2)) glColor3f (1.0, 0.0, 0.0);
            else glColor3f (0.8, 0.6, 0.0);
            glVertex3f (s->downVerts[nStack-1][(i+1)%nSec]->x, s->downVerts[nStack-1][(i+1)%nSec]->y, s->downVerts[nStack-1][(i+1)%nSec]->z);
        };
    } glEnd();
};

void drawCylinder (Cylinder* s, int colVar = 0) {
    int nSec = s->nSec;
    glBegin (GL_QUADS); {
        for (int i=0; i < nSec; i++) {
            if (colVar == 0) glColor3f (0.2, 0.2, 1.0);
            else if (colVar == 1) glColor3f (0.1, 0.5, 0.1);
            else if (colVar == 2) glColor3ub (235, 185, 5);
            glVertex3f (s->upVerts[i]->x, s->upVerts[i]->y, s->upVerts[i]->z);
            glVertex3f (s->upVerts[(i+1)%nSec]->x, s->upVerts[(i+1)%nSec]->y, s->upVerts[(i+1)%nSec]->z);
            if (colVar == 0) glColor3f (0.5, 0.5, 1.0);
            else if (colVar == 1) glColor3f (0.3, 0.7, 0.3);
            glVertex3f (s->downVerts[(i+1)%nSec]->x, s->downVerts[(i+1)%nSec]->y, s->downVerts[(i+1)%nSec]->z);
            glVertex3f (s->downVerts[i]->x, s->downVerts[i]->y, s->downVerts[i]->z);
        };
    } glEnd();
};

void drawCone (Cone* s, int colVar = 0) {
    int nSec = s->nSec;
    glBegin (GL_TRIANGLES); {
        for (int i=0; i < nSec; i++) {
            if (colVar == 0) glColor3f (0.2, 0.2, 1.0);
            else if (colVar == 1) glColor3f (0.1, 0.5, 0.1);
            glVertex3f (0.0, 0.0, s->h);
            if (colVar == 0) glColor3f (0.5, 0.5, 1.0);
            else if (colVar == 1) glColor3f (0.3, 0.7, 0.3);
            glVertex3f (s->downVerts[(i+1)%nSec]->x, s->downVerts[(i+1)%nSec]->y, s->downVerts[(i+1)%nSec]->z);
            glVertex3f (s->downVerts[i]->x, s->downVerts[i]->y, s->downVerts[i]->z);
            glVertex3f (s->downVerts[(i+1)%nSec]->x, s->downVerts[(i+1)%nSec]->y, s->downVerts[(i+1)%nSec]->z);
            glVertex3f (s->downVerts[i]->x, s->downVerts[i]->y, s->downVerts[i]->z);
            glVertex3f (0.0, 0.0, 0.0);
        };
    } glEnd();
};

void drawArrow (Cylinder* ax, Cone* hd, int colVar = 0) {
    drawCylinder(ax, colVar);
    glPushMatrix();
        glTranslatef(0,0,ax->h);
        drawCone(hd, colVar);
    glPopMatrix();
};

void drawOftahedronFace (int colVar = 0) {
    glBegin (GL_TRIANGLES); {
        if (colVar == 0) glColor3f (1.0, 0.3, 0.4);
        else if (colVar == 1)  glColor3f (0.4, 0.3, 1.0);
        glVertex3f (0, 0, 1);
        glVertex3f (0, 1, 0);
        glVertex3f (1, 0, 0);
    } glEnd();
};

void drawCubeSphereFace (CubeSphereFace* ss, int colVar = 0) {
    glBegin (GL_QUADS); {
        if (colVar == 0) glColor3f (0.2, 0.2, 0.8);
        else if (colVar == 1) glColor3f (0.1, 0.5, 0.1);
        else if (colVar == 2) glColor3f (0.7, 0.2, 0.2);
        for (int i = 0; i < ss->pointsPerRow-1; i++) {
            for (int j = 0; j < ss->pointsPerRow-1; j++) {
                glVertex3f (ss->verts[i][j]->x, ss->verts[i][j]->y, ss->verts[i][j]->z);
                glVertex3f (ss->verts[i][j+1]->x, ss->verts[i][j+1]->y, ss->verts[i][j+1]->z);
                glVertex3f (ss->verts[i+1][j+1]->x, ss->verts[i+1][j+1]->y, ss->verts[i+1][j+1]->z);
                glVertex3f (ss->verts[i+1][j]->x, ss->verts[i+1][j]->y, ss->verts[i+1][j]->z);
            };
        };
    } glEnd();
};