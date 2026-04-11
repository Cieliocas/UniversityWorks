// =========================================================
//  main.cpp - Braco Mecanico
//  Ciencia da Computacao - Computacao Grafica
//  Created by Franciélio Castro on 10/04/26.
//  MAT 20249050551
// =========================================================

#ifdef __APPLE__
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
    #include <GLUT/glut.h>
#else
    #include <GL/gl.h>
    #include <GL/glu.h>
    #include <GL/glut.h>
#endif

#include <cstdlib>

static int shoulder = 0, elbow = 0;
static int indicator = -25, middle = -25, thumb = 25;
static int angle = 0;

void init(void);
void keyboard (unsigned char key, int x, int y);
void display(void);
void reshape (int w, int h);

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(700, 512);
    glutCreateWindow("Braco Robotico");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}

void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
}

void reshape (int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65, (float)w / (float)h, 1.0, 20.0);

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef (0.0, 0.0, -6.0);
}

void keyboard (unsigned char key, int x, int y)
{
    switch (key)
    {
        case 's': shoulder += 5; break;
        case 'S': shoulder -= 5; break;

        case 'e': if (elbow < 130) elbow += 5; break;
        case 'E': if (elbow > -130) elbow -= 5; break;

        case 'p': if (thumb < 60) thumb += 5; break;
        case 'P': if (thumb > -30) thumb -= 5; break;

        case 'i': if (indicator < 60) indicator += 5; break;
        case 'I': if (indicator > -90) indicator -= 5; break;

        case 'm': if (middle < 60) middle += 5; break;
        case 'M': if (middle > -90) middle -= 5; break;

        case 'y': angle -= 5; break;
        case 'Y': angle += 5; break;

        case 27: exit(0);
    }

    glutPostRedisplay();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

    // ===== OMBRO =====
    glTranslatef (0, 1.0, 0.0);
    glRotatef ((GLfloat) angle, 0.0, 1.0, 0.0);
    glRotatef ((GLfloat) shoulder, 0.0, 0.0, 1.0);

    // esfera do ombro
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glutSolidSphere(0.25, 20, 20);
    glPopMatrix();

    // braço
    glPushMatrix();
    glTranslatef(0.0, -1.0, 0.0);
    glScalef (0.4, 2.0, 0.7);
    glColor3f(1.0, 0.0, 0.0);
    glutSolidCube (1.0);
    glPopMatrix();

    // ===== COTOVELO =====
    glTranslatef (0.0, -2.0, 0.0);

    // esfera do cotovelo
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glutSolidSphere(0.2, 20, 20);
    glPopMatrix();

    glRotatef ((GLfloat) elbow, 0.0, 0.0, 1.0);

    // antebraço
    glPushMatrix();
    glTranslatef(0.0, -1.0, 0.0);
    glScalef (0.4, 2.0, 0.7);
    glColor3f(0.0, 1.0, 0.0);
    glutSolidCube (1.0);
    glPopMatrix();

    // ===== MÃO =====
    glTranslatef(0.0, -2.0, 0.0);

    // Polegar
    glPushMatrix();
    glTranslatef (0.3, 0.0, 0.0);
    glRotatef ((GLfloat) thumb, 0.0, 0.0, 1.0);

    glPushMatrix();
    glScalef (0.2, 0.6, 0.3);
    glColor3f(1.0, 1.0, 0.0);
    glutSolidCube (1.0);
    glPopMatrix();
    glPopMatrix();

    // Indicador
    glPushMatrix();
    glTranslatef (-0.15, 0.0, 0.2);
    glRotatef ((GLfloat) indicator, 0.0, 0.0, 1.0);

    glPushMatrix();
    glScalef (0.2, 0.6, 0.3);
    glColor3f(0.0, 0.0, 1.0);
    glutSolidCube (1.0);
    glPopMatrix();
    glPopMatrix();

    // Médio
    glPushMatrix();
    glTranslatef (-0.15, 0.0, -0.2);
    glRotatef ((GLfloat) middle, 0.0, 0.0, 1.0);

    glPushMatrix();
    glScalef (0.2, 0.6, 0.3);
    glColor3f(0.0, 1.0, 1.0);
    glutSolidCube (1.0);
    glPopMatrix();
    glPopMatrix();

    glPopMatrix();

    glutSwapBuffers();
}
