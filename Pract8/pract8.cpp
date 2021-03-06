#include <iostream>
#ifdef __GNUC__
#include <GL/freeglut.h>
#else
#include <GL\freeglut.h>
#endif
#include <ctime>
#include <string>
#include "Utilidades.h"

#include "Keyboard.hpp"


using namespace std;

std::string windowTitle;

//Movimiento del vehiculo.
float angle, module;

//Posicion de la camara.
GLfloat camX, camY, camZ;
GLfloat lookAtX, lookAtY, lookAtZ;

const float CAM_FAR = 300.0;


// ATRIBUTOS DE LA VISTA
const unsigned int WIDTH = 1440, HEIGHT = 900;
const float RATIO = (float)WIDTH / HEIGHT;

// ATRIBUTOS DE LA PISTA
const float TRACK_GAP = 50.f;
const float TRACK_WIDTH = 8.f;
const float TRACK_LONG = 50.f;

GLuint track;
GLuint trackTex;
const char *trackFilename = "roadAsphalt.png";

/// TEXTURA DEL SUELO
const char *grassFilename = "grass.png";
GLuint grassTex;

/// IMAGEN DE FONDO
const char *backgroundFilename = "background.jpg";
GLuint backgroundTex;

/// IMAGEN CARTEL
const char *adFilename = "cocacola.jpg";
GLuint adTex;

/**
 * Inicialización de las variables.
 */
void init();

/**
 * Función de dibujado.
 */
void draw();

/**
 * Dibuja un rectángulo con una textura que mantiene
 * una posición fija respecto a la cámara.
 */
void drawBackground();

/**
 * Dibuja un cartel publicitario.
 */
void drawAd();

/**
 * Dibuja una curva (180 grados) dividida en Q quads.
 */
void drawCurve(unsigned int Q);

/**
 * Dibuja toda la pista.
 */
void drawTrack();

/**
 * Función llamada cuando se produce el evento iddle.
 */
void loop();


void init() {
    windowTitle = "Circuito texturizado";
    angle = 90.f;
    module = 0.f;

    
    camX = 0.f;
    camY = 1.f;
    camZ = 0.f;

    lookAtX = 0.f;
    lookAtY = 1.f;
    lookAtZ = -1.f;
    
    /*CARGA IMAGENES*/
    glGenTextures(1,&backgroundTex);
    glBindTexture(GL_TEXTURE_2D, backgroundTex);
    
    loadImageFile((char*)backgroundFilename);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    //
    
    glGenTextures(1,&adTex);
    glBindTexture(GL_TEXTURE_2D, adTex);
    
    loadImageFile((char*)adFilename);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    //
    
    glGenTextures(1,&trackTex);
    glBindTexture(GL_TEXTURE_2D, trackTex);
    
    loadImageFile((char*)trackFilename);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    //
    
    glGenTextures(1,&grassTex);
    glBindTexture(GL_TEXTURE_2D, grassTex);
    
    loadImageFile((char*)grassFilename);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    //
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // CONSTRUIR CIRCUITO
    
    track = glGenLists(1);
    
    glNewList(track, GL_COMPILE);
    
    drawTrack();
    
    glEndList();
    
    
}

void drawCurve(unsigned int Q) {
    GLfloat p[12];
    for (int i = 1; i <= Q; i++) {
        float deg = i * (180.0 / Q);
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
        
        quad(&p[0],&p[3],&p[6],&p[9]);
    }
}

void drawTrack() {
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, trackTex);
    
    
    
    GLfloat p[12] = {0, 0, 0,
                    TRACK_WIDTH, 0, 0,
                    TRACK_WIDTH, 0, TRACK_LONG,
                    0, 0, TRACK_LONG};
    
    quad(&p[0],&p[3],&p[6],&p[9]);
    glTranslatef(TRACK_GAP + TRACK_WIDTH, 0.0, 0.0);
    quad(&p[0],&p[3],&p[6],&p[9]);
    glTranslatef(-(TRACK_GAP + TRACK_WIDTH), 0.0, 0.0);
    
    glTranslatef(TRACK_GAP / 2 + TRACK_WIDTH, 0, TRACK_LONG);

    drawCurve(30);
    
    glTranslatef(-(TRACK_GAP / 2 + TRACK_WIDTH), 0, -TRACK_LONG);
    
    glTranslatef(TRACK_GAP / 2 + TRACK_WIDTH, 0, 0);
    glRotatef(180, 0, 1, 0);
    
    drawCurve(30);
    
    glTranslatef(-(TRACK_GAP / 2 + TRACK_WIDTH), 0, 0);
    
    
    glBindTexture(GL_TEXTURE_2D, 0);
}

void draw() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //ejes();
    
    // PISTA
    
    glPushMatrix();
    glTranslatef(-TRACK_WIDTH / 2, 0.f, 0.f);
    glCallList(track);
    glPopMatrix();
    
    // SUELO PLANO (sigue a la cámara)
    
    glColor3f(0.0, 0.4, 0.0);
    glBegin(GL_QUADS);
    glVertex3f(-CAM_FAR + camX,-0.2,-CAM_FAR + camZ);
    glVertex3f(CAM_FAR + camX,-0.2,-CAM_FAR + camZ);
    glVertex3f(CAM_FAR + camX,-0.2, CAM_FAR + camZ);
    glVertex3f(-CAM_FAR + camX,-0.2, CAM_FAR + camZ);
    glEnd();
    
    //SUELO TEXTURIZADO
    
    glColor3f(1.0, 1.0, 1.0);
    
    glBindTexture(GL_TEXTURE_2D, grassTex);
    
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(-CAM_FAR,-0.1,-CAM_FAR);
    glTexCoord2f(10, 0);
    glVertex3f(CAM_FAR,-0.1,-CAM_FAR);
    glTexCoord2f(10, 10);
    glVertex3f(CAM_FAR,-0.1, CAM_FAR);
    glTexCoord2f(0, 10);
    glVertex3f(-CAM_FAR,-0.1, CAM_FAR);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    
    //FONDO
    
    glPushMatrix();
    glTranslatef(camX, -1, camZ);
    drawBackground();
    glPopMatrix();
    
    // CARTELES PUBLICITARIOS
    
    glPushMatrix();
    glTranslatef(0,0,12);
    drawAd();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(TRACK_GAP + TRACK_WIDTH,0,12);
    glRotatef(180, 0.0, 1.0, 0.0);
    glTranslatef(0,0,12);
    drawAd();
    glPopMatrix();
    
    
    glutSwapBuffers();
    
    return;
    
}

void drawBackground() {
    static const unsigned int C = 20;
    
    glBindTexture(GL_TEXTURE_2D, backgroundTex);   
    
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    
    for (int i = 1; i <= C; i++) {
      float angle = rad(i * (360.0 / C));
      float prevAngle = rad((i - 1) * (360.0 / C));
      
      float x0 = cos(prevAngle) * CAM_FAR;
      float z0 = sin(prevAngle) * CAM_FAR;
      
      float x1 = cos(angle) * CAM_FAR;
      float z1 = sin(angle) * CAM_FAR;
      
      glTexCoord2f(prevAngle / (2 * PI), 0);
      glVertex3f(x0, 0, z0);
      glTexCoord2f(prevAngle / (2 * PI), 1);
      glVertex3f(x0, 300, z0);
      
      glTexCoord2f(angle / (2 * PI), 1);
      glVertex3f(x1, 300, z1);
      glTexCoord2f(angle / (2 * PI), 0);
      glVertex3f(x1, 0, z1);

      
    }
    
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    
}

void drawAd() {
    glTranslatef(-TRACK_WIDTH / 2, 4, 0);
    glBindTexture(GL_TEXTURE_2D, adTex);
    glColor3f(1.0, 1.0, 1.0);
    
    glBegin(GL_POLYGON);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.0, 1.0, 0.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(TRACK_WIDTH, 1.0, 0.0);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(TRACK_WIDTH, 0.0, 0.0);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glTranslatef(TRACK_WIDTH / 2, -4, 0);
    
    glBegin(GL_QUADS);
    
    //Barra vertical
    glColor3f(0.3, 0.3, 0.3);
    glVertex3f(-TRACK_WIDTH / 2, .0, .0);
    glVertex3f(-TRACK_WIDTH / 2 + 0.1, .0, .0);
    glVertex3f(-TRACK_WIDTH / 2 + 0.1, 4.0, .0);
    glVertex3f(-TRACK_WIDTH / 2, 4.0, .0);
    
    glVertex3f(TRACK_WIDTH / 2, .0, .0);
    glVertex3f(TRACK_WIDTH / 2 - 0.1, .0, .0);
    glVertex3f(TRACK_WIDTH / 2 - 0.1, 4.0, .0);
    glVertex3f(TRACK_WIDTH / 2, 4.0, .0);
    glEnd();
}

void loop() {

    static int start = glutGet(GLUT_ELAPSED_TIME);
    int now = glutGet(GLUT_ELAPSED_TIME);
    double deltaTime = (now - start) / 1000.0;
    
    if (isKeyPressed(Key::Up)) {
        module += 2 * deltaTime;
    }
    
    if (isKeyPressed(Key::Down)) {
        module -= 2 * deltaTime;
        if (module < 0) { module = 0; }
    }
    
    if (isKeyPressed(Key::Left)) {
        angle += 20 * deltaTime;
    }
    
    if (isKeyPressed(Key::Right)) {
        angle -= 20 * deltaTime;
    }
    
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
    std::string wt = windowTitle + " " + std::to_string(module) + " m/s, " + std::to_string(module * 3.6) + "km/h";
    glutSetWindowTitle(wt.c_str());

    
    start = now;

    glutPostRedisplay();
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

    
    FreeImage_Initialise();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1440, 900);
    glutCreateWindow(windowTitle.c_str());
    glutDisplayFunc(draw);
    glutReshapeFunc(reshape);
    glutIdleFunc(loop);
    
    glEnableClientState(GL_VERTEX_ARRAY | GL_COLOR_ARRAY);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_TEXTURE_2D);

    //CONFIGURAR CAMARA
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45, RATIO, 1, CAM_FAR);

    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
    //gluLookAt(0.0, 1.0, 0.0, 0.0, 1.0, -1.0, 0.0, 1.0, 0.0);
    
    init();
    initKeyboard();
    cout << windowTitle << " en marcha" << endl;
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glutMainLoop();

	FreeImage_DeInitialise();

    return EXIT_SUCCESS;
}
