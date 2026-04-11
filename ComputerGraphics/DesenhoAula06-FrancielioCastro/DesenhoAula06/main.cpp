// =========================================================
//  main.cpp - DesenhoCenárioCompleto
//  Ciencia da Computacao - Computacao Grafica
//  Created by Franciélio Castro on 09/04/26.
//  MAT 20249050551
// =========================================================

#ifdef __APPLE__ // MacOS
#define GL_SILENCE_DEPRECATION
    #include <GLUT/glut.h>
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#else // Windows e Linux
    #include <GL/glut.h>
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

#include <math.h>

// =========================================================
// FUNÇÕES AUXILIARES DE DESENHO
// =========================================================

// Função para desenhar círculos preenchidos
void desenhaCirculo(float cx, float cy, float r, int n) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= n; i++) {
        float t = 2 * 3.1415926 * i / n;
        glVertex2f(cx + cos(t)*r, cy + sin(t)*r);
    }
    glEnd();
}

// Função para desenhar pétalas (losangos rotacionados)
void desenhaPetala(float cx, float cy, float tamanho) {
    glPushMatrix();
    glTranslatef(cx, cy, 0);
    glRotatef(45, 0, 0, 1);
    float t = tamanho / 2.0;
    glBegin(GL_QUADS);
        glVertex2f(-t, -t);
        glVertex2f( t, -t);
        glVertex2f( t,  t);
        glVertex2f(-t,  t);
    glEnd();
    glPopMatrix();
}

// =========================================================
// CONFIGURAÇÃO INICIAL
// =========================================================

void init() {
    // Cor de fundo padrão (Céu)
    glClearColor(0.2, 0.5, 0.9, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Sistema de coordenadas 800x600
    gluOrtho2D(0, 800, 0, 600);
}

// =========================================================
// RENDERIZAÇÃO DO CENÁRIO
// =========================================================

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Ajuste Global: Move toda a cena para evitar o vão na lateral direita
    glTranslatef(-40.0f, 0.0f, 0.0f);

    // --- 1. GRAMA ---
    glColor3f(0.4, 0.7, 0.2);
    glBegin(GL_QUADS);
        glVertex2f(0, 0);   glVertex2f(840, 0);
        glVertex2f(840, 250); glVertex2f(0, 250);
    glEnd();

    // --- 2. LAGO ---
    glColor3f(0.3, 0.5, 0.9);
    glBegin(GL_QUADS);
        glVertex2f(450, 0); glVertex2f(840, 0);
        glVertex2f(840, 130); glVertex2f(450, 130);
    glEnd();

    // --- 3. MONTANHAS ---
    // Montanha Maior
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_TRIANGLES);
        glVertex2f(520, 250); glVertex2f(620, 430); glVertex2f(720, 250);
    glEnd();
    // Montanha Menor
    glBegin(GL_TRIANGLES);
        glVertex2f(650, 250); glVertex2f(730, 380); glVertex2f(800, 250);
    glEnd();
    // Neve (Topos)
    glColor3f(1, 1, 1);
    glBegin(GL_TRIANGLES);
        glVertex2f(580, 360); glVertex2f(620, 430); glVertex2f(660, 360); // Neve maior
        glVertex2f(700, 330); glVertex2f(730, 380); glVertex2f(760, 330); // Neve menor
    glEnd();

    // --- 4. CASA ---
    // Corpo
    glColor3f(1, 0.8, 0.65);
    glBegin(GL_QUADS);
        glVertex2f(140, 220); glVertex2f(320, 220); glVertex2f(320, 340); glVertex2f(140, 340);
    glEnd();
    // Porta
    glColor3f(0.4, 0.2, 0.1);
    glBegin(GL_QUADS);
        glVertex2f(170, 220); glVertex2f(210, 220); glVertex2f(210, 300); glVertex2f(170, 300);
    glEnd();
    // Janela
    glColor3f(0.5, 0.8, 1);
    glBegin(GL_QUADS);
        glVertex2f(250, 265); glVertex2f(290, 265); glVertex2f(290, 305); glVertex2f(250, 305);
    glEnd();
    // Telhado
    glColor3f(0.8, 0.1, 0.1);
    glBegin(GL_TRIANGLES);
        glVertex2f(140, 340); glVertex2f(230, 440); glVertex2f(320, 340);
    glEnd();

    // --- 5. ÁRVORE ---
    // Tronco
    glColor3f(0.4, 0.2, 0.1);
    glBegin(GL_QUADS);
        glVertex2f(420, 220); glVertex2f(450, 220); glVertex2f(450, 300); glVertex2f(420, 300);
    glEnd();
    // Copa
    glColor3f(0.2, 0.7, 0.2);
    desenhaCirculo(435, 340, 55, 100);

    // --- 6. BARCO ---
    // Casco
    glColor3f(0.5, 0.3, 0.1);
    glBegin(GL_QUADS);
        glVertex2f(580, 50); glVertex2f(700, 50); glVertex2f(730, 90); glVertex2f(550, 90);
    glEnd();
    // Mastro
    glColor3f(0.4, 0.2, 0.1);
    glBegin(GL_QUADS);
        glVertex2f(635, 90); glVertex2f(645, 90); glVertex2f(645, 185); glVertex2f(635, 185);
    glEnd();
    // Vela
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_TRIANGLES);
        glVertex2f(645, 90); glVertex2f(715, 135); glVertex2f(645, 175);
    glEnd();
    // Bandeira
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_TRIANGLES);
        glVertex2f(635, 200); glVertex2f(635, 172); glVertex2f(675, 186);
    glEnd();

    // --- 7. SOL ---
    glColor3f(1, 0.9, 0);
    desenhaCirculo(700, 520, 55, 100);

    // --- 8. NUVENS ---
    glColor3f(1, 1, 1);
    // Nuvem Esquerda
    desenhaCirculo(120, 500, 25, 50); desenhaCirculo(160, 520, 35, 50);
    desenhaCirculo(210, 525, 50, 50); desenhaCirculo(260, 520, 35, 50);
    desenhaCirculo(300, 500, 25, 50);
    // Nuvem Direita
    desenhaCirculo(420, 510, 25, 50); desenhaCirculo(460, 520, 35, 50);
    desenhaCirculo(500, 515, 30, 50); desenhaCirculo(540, 510, 25, 50);

    // --- 9. FLORES ---
    glPushMatrix();
        // Move as flores para baixo na grama
        glTranslatef(0.0f, -65.0f, 0.0f);

        // FLOR 1 (Laranja)
        glColor3f(0.1, 0.3, 0.1); // Caule
        glBegin(GL_QUADS);
            glVertex2f(199, 120); glVertex2f(205, 120); glVertex2f(205, 190); glVertex2f(199, 190);
        glEnd();
        glBegin(GL_TRIANGLES); // Folhas em V
            glVertex2f(199, 125); glVertex2f(175, 155); glVertex2f(199, 145);
            glVertex2f(205, 125); glVertex2f(229, 155); glVertex2f(205, 145);
        glEnd();
        glColor3f(1.0, 0.6, 0.2); // Pétalas
        desenhaPetala(202, 210, 24); desenhaPetala(202, 170, 24);
        desenhaPetala(182, 190, 24); desenhaPetala(222, 190, 24);
        glColor3f(1.0, 0.9, 0.3); // Miolo
        desenhaCirculo(202, 190, 15, 30);

        // FLOR 2 (Roxa)
        glColor3f(0.1, 0.3, 0.1); // Caule
        glBegin(GL_QUADS);
            glVertex2f(299, 120); glVertex2f(305, 120); glVertex2f(305, 190); glVertex2f(299, 190);
        glEnd();
        glBegin(GL_TRIANGLES); // Folhas em V
            glVertex2f(299, 125); glVertex2f(275, 155); glVertex2f(299, 145);
            glVertex2f(305, 125); glVertex2f(329, 155); glVertex2f(305, 145);
        glEnd();
        glColor3f(0.5, 0.2, 0.8); // Pétalas
        desenhaPetala(302, 210, 24); desenhaPetala(302, 170, 24);
        desenhaPetala(282, 190, 24); desenhaPetala(322, 190, 24);
        glColor3f(1.0, 0.9, 0.3); // Miolo
        desenhaCirculo(302, 190, 15, 30);
    glPopMatrix();

    glutSwapBuffers();
}

// =========================================================
// FUNÇÃO PRINCIPAL
// =========================================================

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Trabalho Final CG - Francielio Castro");
    
    init();
    glutDisplayFunc(display);
    
    glutMainLoop();
    return 0;
}
