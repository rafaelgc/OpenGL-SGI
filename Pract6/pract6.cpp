#include <iostream>
#ifdef __GNUC__
#include <GL/freeglut.h>
#else
#include <GL\freeglut.h>
#endif
#include <ctime>
#include "Utilidades.h"

using namespace std;

std::string windowTitle;

//Movimiento del vehiculo.
float angle, module;

//Posicion de la camara.
GLfloat camX, camY, camZ;
GLfloat lookAtX, lookAtY, lookAtZ;

GLfloat pathQuad[12];

/**
 * Inicialización de las variables.
 */
void init();

/**
 * Función de dibujado.
 */
void draw();

/**
 * Función llamada cuando se produce el evento iddle.
 */
void loop();

void onKeyPressed(unsigned char k, int x, int y);

void init() {
    windowTitle = "Interfaz de conducción";
    angle = 90.f;
    module = 0.f;

    /*
     * camX = 0.f;
    camY = 1.f;
    camZ = 0.f;

    lookAtX = 0.f;
    lookAtY = 1.f;
    lookAtZ = -1.f;
     * */
    
    camX = 0.f;
    camY = 5.f;
    camZ = 0.f;

    lookAtX = 0.f;
    lookAtY = 0.f;
    lookAtZ = 0.f;
    
    pathQuad[0] = 0.f; pathQuad[1] = 0.f; pathQuad[2] = 0.f;
    pathQuad[3] = 8.f; pathQuad[4] = 0.f; pathQuad[5] = 0.f;
    pathQuad[6] = 8.f; pathQuad[7] = 0.f; pathQuad[8] = 1.f;
    pathQuad[9] = 0.f; pathQuad[10] = 0.f; pathQuad[11] = 1.f;
    
}

/*
 * Calcula dos de las esquinas del quad a partir de un ángulo en radianes.
 */
void calcQuad(float ang, float amplitude, float periode, GLfloat quad[]) {
    
}

void drawCurve(int mode) {
    glEnable(GL_POINT_SMOOTH);
    glPointSize(5.0);

    glBegin(GL_POINTS);
    unsigned int quadsAmount = 50;
    float freq = 0.5;
    float t = 1 / freq;
    float length = 10;
    float amplitude = 0.5;
    float width = 0.2;
    
    glPointSize(10);
    
    for (int i = 0; i < quadsAmount; i++) {
        
        float ang = rad((i / (float)quadsAmount) * 360);
        
        float x = amplitude * cos(freq * length * ang);
        //float z = sin(ang);
        float z = length * (i / (float)quadsAmount);
        
        float pend = - amplitude * freq * length * sin(freq * length * ang);
        
        float v0_x = 1 / (sqrt(1 + pend * pend));
        float v0_z = -pend / (sqrt(1 + pend * pend));
        
        //Primer punto
        float p0_x = x - v0_x * (width / 2);
        float p0_z = z - v0_z * (width / 2);

        //Cálculo del segundo punto.
        float p1_x = x + v0_x * (width / 2);
        float p1_z = z + v0_z * (width / 2);
        
        if (mode == 0) {
            glColor3f(1, 0, 0);
            glVertex3f(x, 0, z);
        }
        else if (mode == 1) {
            glColor3f(0, 1, 0);
            glVertex3f(p0_x, 0, p0_z);
        }
        else {
            glColor3f(0, 0, 1);
            glVertex3f(p1_x, 0, p1_z);
        }
    }
    glEnd();
}

void draw() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawCurve(0);
    drawCurve(1);
    drawCurve(2);

    glColor3fv(VERDE);
    
    glBegin(GL_QUADS);
    glVertex3f(0,0,0);
    glVertex3f(0,0,0.1);
    glVertex3f(0.1,0,0.1);
    glVertex3f(0.1,0,0);
    glEnd();
    
   
    
    glutSwapBuffers();
    
    return;
    
}

void loop() {

    static int start = glutGet(GLUT_ELAPSED_TIME);
    int now = glutGet(GLUT_ELAPSED_TIME);
    double deltaTime = (now - start) / 1000.0;

    
    ///ACTUALIZACIÓN DE LA POSICIÓN DEL VEHÍCULO
    
    /*camZ -= sin(rad(angle)) * module * deltaTime;
    camX += cos(rad(angle)) * module * deltaTime;*/
    
    ///ACTUALIZACIÓN DEL PUNTO DONDE MIRA LA CÁMARA
    /*El usuario siempre mira hacia delante por eso se calcula
     el valor absoluto del módulo. Se le suma 1 para garantizar
     que sea mayor que 0.*/
    /*lookAtZ = camZ + sin(rad(angle)) * -(abs(module) + 1);
    lookAtX = camX + cos(rad(angle)) * (abs(module) + 1);*/
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camX, camY, camZ, lookAtX, lookAtY, lookAtZ, 0.0, 0.0, 1.0);
    
    ///ACTUALIZACIÓN DEL TÍTULO DE LA VENTANA
    std::string wt = windowTitle + " " + std::to_string(module) + " m/s";
    glutSetWindowTitle(wt.c_str());

    
    start = now;

    glutPostRedisplay();
}

void onKeyPressed(unsigned char k, int x, int y) {
    if (k == 'w' || k == 'W') {
        module += 0.1;
    } else if (k == 's' || k == 'S') {
        module -= 0.1;
    }
    else if (k == 'a' || k == 'A') {
        angle += 0.25;
    }
    else if (k == 'd' || k == 'D') {
        angle -= 0.25;
    }
    
}


void reshape(GLint w, GLint h) {
    GLint s = h;
    /*glViewport(w / 2 - s / 2, 0, s, s);*/
    glViewport(0, 0, 1440, 900); /////////////////////////////////////////////////
}

int main(int argc, char** argv) {

    init();
    
    /*GLfloat pathQuad[12];
    
    calcQuad(0, 4, 2, pathQuad);
    calcQuad(45, 4, 2, &pathQuad[6]);
    
    for (int i = 0; i < 12; i++) {
        std::cout << i << " " << pathQuad[i] << std::endl;
    }
    
    return 0;*/
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1440, 900);
    glutCreateWindow(windowTitle.c_str());
    glutDisplayFunc(draw);
    glutReshapeFunc(reshape);
    glutIdleFunc(loop);
    glutKeyboardFunc(onKeyPressed);
    
    cout << windowTitle << " en marcha" << endl;

    glEnableClientState(GL_VERTEX_ARRAY | GL_COLOR_ARRAY);
    glEnable(GL_DEPTH_TEST);

    //CONFIGURAR CAMARA
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45, 1.6, 1, 100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 1.0, 0.0, 0.0, 1.0, -1.0, 0.0, 1.0, 0.0);

    glutMainLoop();

    return EXIT_SUCCESS;
}
