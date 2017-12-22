#include <iostream>
#ifdef __GNUC__
#include <GL/freeglut.h>
#else
#include <GL\freeglut.h>
#endif
#include <ctime>
#include "Utilidades.h"

#define SGI_DEBUG

using namespace std;

std::string windowTitle;

//Movimiento del vehiculo.
float angle, module;

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

/**
 * Inicialización de las variables.
 */
void init();

/**
 * Función de dibujado.
 */
void draw();

void drawCurve(unsigned int Q);

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

    
    camX = 0.f;
    camY = 1.f;
    camZ = 0.f;

    lookAtX = 0.f;
    lookAtY = 1.f;
    lookAtZ = -1.f;
    
    track = glGenLists(1);
    
    glNewList(track, GL_COMPILE);
    
    glColor3fv(GRISOSCURO);
    
    GLfloat p[12] = {0, 0, 0,
                    TRACK_WIDTH, 0, 0,
                    TRACK_WIDTH, 0, TRACK_LONG,
                    0, 0, TRACK_LONG};
    
    quad(&p[0],&p[3],&p[6],&p[9]);
    glTranslatef(TRACK_GAP + TRACK_WIDTH, 0.0, 0.0);
    quad(&p[0],&p[3],&p[6],&p[9]);
    glTranslatef(-(TRACK_GAP + TRACK_WIDTH), 0.0, 0.0);
    
    glTranslatef(TRACK_GAP / 2 + TRACK_WIDTH, 0, TRACK_LONG);

    drawCurve(20);
    
    glTranslatef(-(TRACK_GAP / 2 + TRACK_WIDTH), 0, -TRACK_LONG);
    
    glTranslatef(TRACK_GAP / 2 + TRACK_WIDTH, 0, 0);
    glRotatef(180, 0, 1, 0);
    
    drawCurve(20);
    
    glTranslatef(-(TRACK_GAP / 2 + TRACK_WIDTH), 0, 0);
    
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
        
        glVertex3fv(&p[0]);
        glVertex3fv(&p[3]);
        glVertex3fv(&p[6]);
        glVertex3fv(&p[9]);
        
        quad(&p[0],&p[3],&p[6],&p[9]);
    }
}

<<<<<<< HEAD
void Game::onKeyPressed(unsigned char k, int x, int y) {
	if (k == 'w') {
		module += 0.5;
	}
	else if (k == 's') {
		module += 0.5;
	}
=======
void draw() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ejes();
    
    glPushMatrix();
    glTranslatef(-TRACK_WIDTH / 2, 0.f, 0.f);
    glCallList(track);
    glPopMatrix();
    
    glColor3f(0.1, 0.8, 0.1);
    
    glBegin(GL_QUADS);
    glVertex3f(-100,-0.1,-100);
    glVertex3f(100,-0.1,-100);
    glVertex3f(100,-0.1,100);
    glVertex3f(-100,-0.1,100);
    glEnd();
    
    glutSwapBuffers();
    
    return;
    
>>>>>>> 7f7d74badf1c55485a3a40daac42c8ffdedb8420
}

void loop() {

    static int start = glutGet(GLUT_ELAPSED_TIME);
    int now = glutGet(GLUT_ELAPSED_TIME);
    double deltaTime = (now - start) / 1000.0;

    
    ///ACTUALIZACIÓN DE LA POSICIÓN DEL VEHÍCULO
    
    camZ += sin(rad(angle)) * module * deltaTime;
    camX -= cos(rad(angle)) * module * deltaTime;
    
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
    std::string wt = windowTitle + " " + std::to_string(module) + " m/s";
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

    init();
    cout << windowTitle << " en marcha" << endl;
    
    
    glutMainLoop();

    return EXIT_SUCCESS;
}
