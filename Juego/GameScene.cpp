#include "GameScene.hpp"

#include "lib/Keyboard.hpp"
#include "lib/Application.hpp"
#include "Utilidades.h"

#include <algorithm>

const float GameScene::TRACK_GAP = 65.f;
const float GameScene::TRACK_WIDTH = 12.f;
const float GameScene::TRACK_LONG = 100.f;
const float GameScene::CAM_FAR = 300.0;



GameScene::GameScene() : Scene("GameScene") {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45, (float) Application::width / Application::height, 1, CAM_FAR);

    angle = 90.f;
    speed = 0.f;

    camX = 0.f;
    camY = 1.f;
    camZ = 0.f;

    lookAtX = 0.f;
    lookAtY = 1.f;
    lookAtZ = -1.f;
    
    score = 0;
    visibleScore = 0.f;
    
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
    globalLight.setType(Light::Type::Directional);
    globalLight.setPosition(0, 10, 10);
    globalLight.setAmbient(0.1, 0.1, 0.1);
    globalLight.setDiffuse(0.1, 0.1, 0.1);
    globalLight.setSpecular(1.0, 1.0, 1.0);
    globalLight.enable(GL_LIGHT0);
    

    //Faro
    headlightPosition[0] = 0;
    headlightPosition[1] = 0.7;
    headlightPosition[2] = 0;
    headlightPosition[3] = 1;
    
    headlight.setType(Light::Type::Positional);
    headlight.setAmbient(0.8, 0.8, 0.8);
    headlight.setDiffuse(1.0, 1.0, 1.0);
    headlight.setSpecular(0.3, 0.3, 0.3);
    headlight.setDirection(0, -0.2, -1);
    headlight.setCutoff(25.0);
    headlight.setExponent(20.0);
    headlight.enable(GL_LIGHT1);
    
    // PUNTOS
    
    for (int i = 15; i <= 60; i += 10) {
        points.push_back(Point(0, 1, i));
    }
    
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
        speed += 4 * deltaTime;
    }
    else {
        speed -= 2 * deltaTime;
        if (speed < 0.f) { speed = 0.f; }
    }
    
    //Freno suave.
    if (Keyboard::isKeyPressed(Keyboard::Key::Down)) {
        speed -= 2 * deltaTime;
        if (speed < 0) { speed = 0; }
    }
    
    //Freno rápido.
    if (Keyboard::isKeyPressed(Keyboard::Key::Space)) {
        speed -= std::max<float>(4, speed / 3) * deltaTime;
        if (speed < 0) { speed = 0; }
    }
    
    // GIRO
    if (Keyboard::isKeyPressed(Keyboard::Key::Left)) {
        angle += std::min<float>(40, 40 * (speed / 10)) * deltaTime;
    }
    
    if (Keyboard::isKeyPressed(Keyboard::Key::Right)) {
        angle -= std::min<float>(40, 40 * (speed / 10)) * deltaTime;
    }
}

void GameScene::logic(float deltaTime) {
    ///ACTUALIZACIÓN DE LA POSICIÓN DEL VEHÍCULO
    camZ += sin(rad(angle)) * speed * deltaTime;
    camX -= cos(rad(angle)) * speed * deltaTime;
    
    ///ACTUALIZACIÓN DEL PUNTO DONDE MIRA LA CÁMARA
    /*El usuario siempre mira hacia delante por eso se calcula
     el valor absoluto del módulo. Se le suma 1 para garantizar
     que sea mayor que 0.*/
    lookAtZ = camZ + sin(rad(angle)) * (abs(speed) + 1);
    lookAtX = camX - cos(rad(angle)) * (abs(speed) + 1);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camX, camY, camZ, lookAtX, lookAtY, lookAtZ, 0.0, 1.0, 0.0);
    
    /// PUNTUACION
    
    if (visibleScore < score) {
        visibleScore += 1000 * deltaTime;
    }
    
    /// POWER UPS
    
    for (Point &point : points) {
        if (point.manage(camX, camY, camZ, deltaTime)) {
            score += point.getScored();
            std::cout << score << std::endl;
        }
    }
    
    
    
    ///ACTUALIZACIÓN DEL TÍTULO DE LA VENTANA
    std::string wt =  "Juego " + std::to_string(speed) + " m/s, " + std::to_string(speed * 3.6) + "km/h";
    glutSetWindowTitle(wt.c_str());
}

void GameScene::render() {
    if (getState() == State::Inactive) {
        //Si la escena está inactiva, salimos directamente.
        return;
    }
    
    //
    ejes();
    
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
    
    ///
    
    glColor3f(0.5, 0.2, 0.2);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, -1);
    glVertex3f(0, 0, 5);
    glVertex3f(0, 1, 5);
    glVertex3f(1, 1, 5);
    glVertex3f(1, 0, 5);
    glEnd();
    
    std::string sSpeed = std::to_string(speed * 3.6);
    texto(50, 50, sSpeed.substr(0, sSpeed.find_first_of("."))  + " km/h", BLANCO);
    
    texto(Application::width - 100, 50, std::to_string((int) visibleScore), BLANCO);
    
    for (Point &point : points) {
        point.draw();
    }
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
    
    quad(&p[0],&p[3],&p[6],&p[9], 50, 100);
    glTranslatef(TRACK_GAP + TRACK_WIDTH, 0.0, 0.0);
    quad(&p[0],&p[3],&p[6],&p[9], 50, 100);
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
