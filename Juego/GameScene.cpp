#include "GameScene.hpp"

#include "lib/Keyboard.hpp"
#include "lib/Application.hpp"
#include "Utilidades.h"

const float GameScene::TRACK_GAP = 60.f;
const float GameScene::TRACK_WIDTH = 10.f;
const float GameScene::TRACK_LONG = 100.f;
const float GameScene::CAM_FAR = 300.0;



GameScene::GameScene() : Scene("GameScene") {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45, (float) Application::width / Application::height, 1, CAM_FAR);

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
    
    loadImageFile("background.jpg");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    //
    
    glGenTextures(1,&adTex);
    glBindTexture(GL_TEXTURE_2D, adTex);
    
    loadImageFile("cocacola.jpg");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    //
    
    glGenTextures(1,&trackTex);
    glBindTexture(GL_TEXTURE_2D, trackTex);
    
    loadImageFile("roadAsphalt.png");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    //
    
    glGenTextures(1,&grassTex);
    glBindTexture(GL_TEXTURE_2D, grassTex);
    
    loadImageFile("grass.png");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    //
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // CONSTRUIR CIRCUITO
    
    track = glGenLists(1);
    
    glNewList(track, GL_COMPILE);
    
    drawTrack();
    
    glEndList();
    
    

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    //SOMBRA
    glShadeModel (GL_SMOOTH);
    
    //Luz de la luna
    GLfloat moonPosition[] = { 0, 10.0, 10.0, 0.0 };
    GLfloat moonAmbient[] = { 1.05, 1.05, 1.05 };
    GLfloat moonDiffuse[] = { 1.05, 1.05, 1.05 };
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
    
    GLfloat headlightAmbient[] = { 0.5, 0.5, 0.5 };
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
}

void GameScene::onActivate() {
    Scene::onActivate();
     //ILUMINACION
    glEnable(GL_LIGHTING);
    
}

void GameScene::onDeactivate() {
    Scene::onDeactivate();
    glDisable(GL_LIGHTING);
}

void GameScene::manageEvents(float deltaTime) {
    if (Keyboard::isKeyPressed(Keyboard::Key::Up)) {
        module += 2 * deltaTime;
    }
    else {
        module -= 2 * deltaTime;
        if (module < 0.f) { module = 0.f; }
    }
    
    //Freno suave.
    if (Keyboard::isKeyPressed(Keyboard::Key::Down)) {
        module -= 2 * deltaTime;
        if (module < 0) { module = 0; }
    }
    
    //Freno rápido.
    if (Keyboard::isKeyPressed(Keyboard::Key::Space)) {
        module -= max(4, module / 3) * deltaTime;
        if (module < 0) { module = 0; }
    }
    
    if (Keyboard::isKeyPressed(Keyboard::Key::Left)) {
        angle += min(30, 30 * (module / 10)) * deltaTime;
    }
    
    if (Keyboard::isKeyPressed(Keyboard::Key::Right)) {
        angle -= min(30, 30 * (module / 10)) * deltaTime;
    }
}

void GameScene::logic(float deltaTime) {
    ///ACTUALIZACIÓN DE LA POSICIÓN DEL VEHÍCULO
    
    //std::cout << 1 / deltaTime << std::endl;
    
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
    std::string wt =  "Juego " + std::to_string(module) + " m/s, " + std::to_string(module * 3.6) + "km/h";
    glutSetWindowTitle(wt.c_str());
}

void GameScene::render() {

    
    texto(0.5, 0.5, "Hola mundo");

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
}

void GameScene::drawTrack() {
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, trackTex);
    
    GLfloat p[12] = {
                    0, 0, TRACK_LONG,
                    TRACK_WIDTH, 0, TRACK_LONG,
                    TRACK_WIDTH, 0, 0,
                    0, 0, 0,
                    
                    };
    
    quad(&p[0],&p[3],&p[6],&p[9], 15, 100);
    glTranslatef(TRACK_GAP + TRACK_WIDTH, 0.0, 0.0);
    quad(&p[0],&p[3],&p[6],&p[9], 50, 50);
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

void GameScene::drawCurve(unsigned int Q) {
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

void GameScene::drawAd() {
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

void GameScene::drawBackground() {
    static const unsigned int C = 20;
    
    glBindTexture(GL_TEXTURE_2D, backgroundTex);   
    
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    
    for (int i = 1; i <= C; i++) {
      float angle = rad(i * (360.0 / C));
      float prevAngle = rad((i - 1) * (360.0 / C));
      
      float x0 = cos(prevAngle) * (CAM_FAR - 10);
      float z0 = sin(prevAngle) * (CAM_FAR - 10);
      
      float x1 = cos(angle) * (CAM_FAR - 10);
      float z1 = sin(angle) * (CAM_FAR - 10);
      
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
