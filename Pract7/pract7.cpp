#include <iostream>
#ifdef __GNUC__
#include <GL/freeglut.h>
#else
#include <GL\freeglut.h>
#endif
#include <ctime>
#include "Utilidades.h"
#include <string>

#define NSGI_DEBUG

using namespace std;

std::string windowTitle;

//Movimiento del vehiculo.
float angle, module;

//Luz del vehículo.
GLfloat headlightPosition[4];

//Posicion de la camara.
GLfloat camX, camY, camZ;
GLfloat lookAtX, lookAtY, lookAtZ;

// ATRIBUTOS DE LA VISTA
const unsigned int WIDTH = 1440, HEIGHT = 900;
const float RATIO = (float)WIDTH / HEIGHT;

// ATRIBUTOS DE LA PISTA
const float TRACK_GAP = 50.f;
const float TRACK_WIDTH = 8.f;
const float TRACK_LONG = 50.f;
GLuint track;

// FAROLAS
GLuint streetlight;
GLfloat streetlightPosition1[] = { -2, 4, 10, 1.0 };
GLfloat streetlightPosition2[] = { -2, 4, 40, 1.0 };
GLfloat streetlightPosition3[] = { TRACK_WIDTH / 2 + TRACK_GAP + 2, 4, 10, 1.0 };
GLfloat streetlightPosition4[] = { TRACK_WIDTH / 2 + TRACK_GAP + 2, 4, 40, 1.0 };

/**
 * Inicialización de las variables.
 */
void init();

/**
 * Función de dibujado.
 */
void draw();

void drawCurve(unsigned int Q);

void drawStreetlight();

/**
 * Configuración de las propiedades de las farolas.
*/
void setupStreetlight(GLenum light, GLfloat position[]);

/**
 * Función llamada cuando se produce el evento iddle.
 */
void loop();

void onKeyPressed(unsigned char k, int x, int y);
void onSpecialKeyPressed(int k, int x, int y);

void init() {
    windowTitle = "Interfaz de conducción";
    angle = 90.f;
    module = 0.f;

    #ifndef SGI_DEBUG
    camX = 0.f;
    camY = 1.f;
    camZ = -4.f;

    lookAtX = 0.f;
    lookAtY = 1.f;
    lookAtZ = -1.f;
    #else
    camX = 0.f;
    camY = 99.f;
    camZ = 0.f;

    lookAtX = 0.f;
    lookAtY = -1.f;
    lookAtZ = 0.f;
    #endif
    
    // PISTA
    track = glGenLists(1);
    
    glNewList(track, GL_COMPILE);
    
    GLfloat p[12] = {0, 0, 0,
                    TRACK_WIDTH, 0, 0,
                    TRACK_WIDTH, 0, TRACK_LONG,
                    0, 0, TRACK_LONG};
    

                
    quad(&p[0],&p[9],&p[6],&p[3], 100, 100);
    glTranslatef(TRACK_GAP + TRACK_WIDTH, 0.0, 0.0);
    quad(&p[0],&p[9],&p[6],&p[3], 100, 100);
    glTranslatef(-(TRACK_GAP + TRACK_WIDTH), 0.0, 0.0);
    
    glTranslatef(TRACK_GAP / 2 + TRACK_WIDTH, 0, TRACK_LONG);

    drawCurve(20);
    
    glTranslatef(-(TRACK_GAP / 2 + TRACK_WIDTH), 0, -TRACK_LONG);
    
    glTranslatef(TRACK_GAP / 2 + TRACK_WIDTH, 0, 0);
    glRotatef(180, 0, 1, 0);
    
    drawCurve(20);
    
    glTranslatef(-(TRACK_GAP / 2 + TRACK_WIDTH), 0, 0);
    
    glEndList();
    
    // FAROLA
    streetlight = glGenLists(1);
    
    glNewList(streetlight, GL_COMPILE);
    drawStreetlight();
    glEndList();
}

void drawCurve(unsigned int Q) {
    GLfloat p[12];
    for (int i = 1; i <= Q; i++) {
        float deg = i * (180 / Q);
        float prevDeg = (i - 1) * (180 / Q);
        
        p[0] = TRACK_GAP / 2 * cos(rad(deg));
        p[1] = 0.f;
        p[2] = TRACK_GAP / 2 * sin(rad(deg));
        
        p[3] = (TRACK_GAP / 2 + TRACK_WIDTH) * cos(rad(deg));
        p[4] = 0.f;
        p[5] = (TRACK_GAP / 2 + TRACK_WIDTH) * sin(rad(deg));
        
        p[9] = TRACK_GAP / 2 * cos(rad(prevDeg));
        p[10] = 0.f;
        p[11] = TRACK_GAP / 2 * sin(rad(prevDeg));
        
        p[6] = (TRACK_GAP / 2 + TRACK_WIDTH) * cos(rad(prevDeg));
        p[7] = 0.f;
        p[8] = (TRACK_GAP / 2 + TRACK_WIDTH) * sin(rad(prevDeg));
        
        quad(&p[0],&p[3],&p[6],&p[9], 20, 20);
    }
}

void draw() {
    glClearColor(.0, .0, .0, .0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLightfv(GL_LIGHT2, GL_POSITION, streetlightPosition1);
    glLightfv(GL_LIGHT3, GL_POSITION, streetlightPosition2);
    glLightfv(GL_LIGHT4, GL_POSITION, streetlightPosition3);
    glLightfv(GL_LIGHT5, GL_POSITION, streetlightPosition4);
    
    glPushMatrix();
    glTranslatef(-TRACK_WIDTH / 2, 0.f, 0.f);
    glCallList(track);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-TRACK_WIDTH / 2, .0, 10);
    glCallList(streetlight);
    glTranslatef(TRACK_WIDTH + TRACK_GAP, .0, .0);
    glCallList(streetlight);
    glTranslatef(.0, .0, 30);
    drawStreetlight();
    glTranslatef(-(TRACK_WIDTH + TRACK_GAP), .0, .0);
    glCallList(streetlight);
    glPopMatrix();
    
    glutSwapBuffers();
    
    return;
    
}

void loop() {

    static int start = glutGet(GLUT_ELAPSED_TIME);
    int now = glutGet(GLUT_ELAPSED_TIME);
    double deltaTime = (now - start) / 1000.0;

    
    ///ACTUALIZACIÓN DE LA POSICIÓN DEL VEHÍCULO
    
    camZ += sin(rad(angle)) * module * deltaTime;
    camX -= cos(rad(angle)) * module * deltaTime;
    
    //headlightPosition[0] = camX;
    //headlightPosition[2] = camZ;
    //glLightfv(GL_LIGHT1, GL_POSITION, headlightPosition);
    
    ///ACTUALIZACIÓN DEL PUNTO DONDE MIRA LA CÁMARA
    /*El usuario siempre mira hacia delante por eso se calcula
     el valor absoluto del módulo. Se le suma 1 para garantizar
     que sea mayor que 0.*/
    lookAtZ = camZ + sin(rad(angle)) * (abs(module) + 1);
    lookAtX = camX - cos(rad(angle)) * (abs(module) + 1);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camX, camY, camZ, lookAtX, lookAtY, lookAtZ, 0.0, 1.0, 0.0);
    
    ///ACTUALIZACIÓN DEL TÍTULO DE LA VENTANA
    std::string wt = windowTitle + " " + std::to_string(module) + " m/s, " + std::to_string(1 / deltaTime) + " fps";
    glutSetWindowTitle(wt.c_str());

    
    start = now;

    glutPostRedisplay();
}

void onKeyPressed(unsigned char k, int x, int y) {}

void onSpecialKeyPressed(int k, int x, int y) {
    if (k == GLUT_KEY_UP) {
        module += 0.1;
    } else if (k == GLUT_KEY_DOWN) {
        module -= 0.1;
        if (module < 0) module = 0;
    }
    else if (k == GLUT_KEY_LEFT) {
        angle += 0.25;
    }
    else if (k == GLUT_KEY_RIGHT) {
        angle -= 0.25;
    }
    
}


void reshape(GLint w, GLint h) {
    
    float razonAD= float(w)/h;
    float wp,hp;
    if(razonAD<RATIO){
        wp= float(w);
        hp= wp/RATIO;
        glViewport(0,int(h/2.0-hp/2.0),w,int(hp));
    }
    else{
        hp= float(h);
        wp= hp*RATIO;
        glViewport(int(w/2.0-wp/2.0),0,int(wp),h);
    }
}

int main(int argc, char** argv) {

    
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1440, 900);
    glutCreateWindow(windowTitle.c_str());
    glutDisplayFunc(draw);
    glutReshapeFunc(reshape);
    glutIdleFunc(loop);
    glutKeyboardFunc(onKeyPressed);
    glutSpecialFunc(onSpecialKeyPressed);
    
    glEnableClientState(GL_VERTEX_ARRAY | GL_COLOR_ARRAY);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);

    //CONFIGURAR CAMARA
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45, 1.6, 1, 100);

    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
    //gluLookAt(0.0, 1.0, 0.0, 0.0, 1.0, -1.0, 0.0, 1.0, 0.0);
    
    //ILUMINACION
    glEnable(GL_LIGHTING);
    glShadeModel (GL_SMOOTH);
    
    //Material carretera.
    GLfloat matSpecular[] = { 0.3, 0.3, 0.3, 0.3 };
    GLfloat matDiffuse[] = { 0.8, 0.8, 0.8 };
    GLfloat matShininess[] = { 3.0 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);
    
    
    //Luz de la luna
    GLfloat moonPosition[] = { 0, 10.0, 10.0, 0.0 };
    GLfloat moonAmbient[] = { 0.05, 0.05, 0.05 };
    GLfloat moonDiffuse[] = { 0.05, 0.05, 0.05 };
    GLfloat moonSpecular[] = {.0, .0, .0 };
    glLightfv(GL_LIGHT0, GL_POSITION, moonPosition);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, moonDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, moonSpecular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, moonAmbient);
    glEnable(GL_LIGHT0);

    //Faro
    headlightPosition[0] = 0;
    headlightPosition[1] = 0.7;
    headlightPosition[2] = 0;
    headlightPosition[3] = 1;
    
    GLfloat headlightAmbient[] = { 0.2, 0.2, 0.2 };
    GLfloat headlightDiffuse[] = { 1.0, 1.0, 1.0 };
    GLfloat headlightSpecular[] = {.3, .3, .3 };
    GLfloat headlightDir[] = {0, -0.5, -1 };
    glLightfv(GL_LIGHT1, GL_POSITION, headlightPosition);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, headlightDiffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, headlightSpecular);
    glLightfv(GL_LIGHT1, GL_AMBIENT, headlightAmbient);
    glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,headlightDir);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 20.0);
    glEnable(GL_LIGHT1);
    
    
    //Farolas    
    setupStreetlight(GL_LIGHT2, streetlightPosition1);
    setupStreetlight(GL_LIGHT3, streetlightPosition2);
    setupStreetlight(GL_LIGHT4, streetlightPosition3);
    setupStreetlight(GL_LIGHT5, streetlightPosition4);

    init();
    std::cout << windowTitle << " en marcha" << std::endl;
    
    
    glutMainLoop();

    return EXIT_SUCCESS;
}

void setupStreetlight(GLenum light, GLfloat position[]) {
    static GLfloat streetlightAmbient[] = { 0.0, 0.0, 0.0 };
    static GLfloat streetlightDiffuse[] = { .5, .5, .2 };
    static GLfloat streetlightSpecular[] = {.0, .0, .0 };
    static GLfloat streetlightDir[] = {0, -1, 0 };
    
    glLightfv(light, GL_POSITION, position);
    glLightfv(light, GL_DIFFUSE, streetlightDiffuse);
    glLightfv(light, GL_SPECULAR, streetlightSpecular);
    glLightfv(light, GL_AMBIENT, streetlightAmbient);
    glLightfv(light,GL_SPOT_DIRECTION, streetlightDir);
    glLightf(light, GL_SPOT_CUTOFF, 45.0);
    glLightf(light, GL_SPOT_EXPONENT, 10.0);
    glEnable(light);
}

void drawStreetlight() {
    glPushMatrix();
    
    glBegin(GL_QUADS);
    //Barra vertical
    glNormal3f(.0, .0, -1.0);
    glVertex3f(0.1, .0, .0);
    glVertex3f(.0, .0, .0);
    glVertex3f(.0, 4.0, .0);
    glVertex3f(0.1, 4.0, .0);
    
    glNormal3f(.0, .0, 1.0);
    glVertex3f(0.1, .0, .1);
    glVertex3f(.0, .0, .1);
    glVertex3f(.0, 4.0, .1);
    glVertex3f(0.1, 4.0, .1);
    
    glNormal3f(-1.0, .0, .0);
    glVertex3f(.0, .0, .0);
    glVertex3f(.0, .0, 0.1);
    glVertex3f(.0, 4.0, 0.1);
    glVertex3f(0.0, 4.0, .0);
    
    glNormal3f(1.0, .0, .0);
    glVertex3f(.1, .0, .0);
    glVertex3f(.1, .0, 0.1);
    glVertex3f(.1, 4.0, 0.1);
    glVertex3f(.1, 4.0, .0);
    glEnd();
    
    //Barra horizontal
    glTranslatef(.0, 4.0, .0);
    glBegin(GL_QUADS);
    glNormal3f(.0, .0, -1.0);
    glVertex3f(2.0, .0, .0);
    glVertex3f(.0, .0, .0);
    glVertex3f(.0, .1, .0);
    glVertex3f(2.0, 0.1, .0);
    
    glNormal3f(.0, .0, 1.0);
    glVertex3f(2.0, .0, .1);
    glVertex3f(.0, .0, .1);
    glVertex3f(.0, .1, .1);
    glVertex3f(2.0, 0.1, .1);
    
    glNormal3f(-1.0, .0, .0);
    glVertex3f(.0, .0, .0);
    glVertex3f(.0, .0, 0.1);
    glVertex3f(.0, 0.1, 0.1);
    glVertex3f(.0, 0.1, .0);
    
    glNormal3f(1.0, .0, .0);
    glVertex3f(2.0, .0, .0);
    glVertex3f(2.0, .0, 0.1);
    glVertex3f(2.0, 0.1, 0.1);
    glVertex3f(2.0, 0.1, .0);
    
    glEnd();
    
    glPopMatrix();
}
