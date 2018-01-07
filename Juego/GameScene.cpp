#include "GameScene.hpp"

#include "lib/Keyboard.hpp"
#include "lib/Application.hpp"
#include "Utilidades.h"
#include "lib/SceneManager.hpp"

#include <algorithm>

const float GameScene::TRACK_GAP = 65.f;
const float GameScene::TRACK_WIDTH = 12.f;
const float GameScene::TRACK_LONG = 100.f;
const float GameScene::CAM_FAR = 300.0;

GameScene::GameScene() : Scene("GameScene") {
    
    renderMode = Solid;
    
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
    
    // Alta calidad (vista normal)
    track = glGenLists(1);
    glNewList(track, GL_COMPILE);
    drawTrack();
    glEndList();
    
    // Baja calidad (retrovisor).
    bTrack = glGenLists(1);
    glNewList(bTrack, GL_COMPILE);
    drawTrack(false);
    glEndList();
    
    // Alta calidad (vista normal)
    trackWired = glGenLists(1);
    glNewList(trackWired, GL_COMPILE);
    drawTrack(true, true);
    glEndList();
    
    // Baja calidad (retrovisor).
    bTrackWired = glGenLists(1);
    glNewList(bTrackWired, GL_COMPILE);
    drawTrack(false, true);
    glEndList();
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    //SOMBRA
    glShadeModel (GL_SMOOTH);
    
    //Luz global (sol / luna)
    globalLight.setType(Light::Type::Directional);
    globalLight.setPosition(0, 10, 10);
    setDay();
    
    lightingManager.addLight(globalLight, false, true);
    
    //Faro
    
    headlight.setType(Light::Type::Positional);
    headlight.setPosition(.0, .7, .0);
    headlight.setAmbient(0.6, 0.6, 0.6);
    headlight.setDiffuse(1.0, 1.0, 1.0);
    headlight.setSpecular(1.0, 1.0, 1.0);
    headlight.setDirection(0, -0.2, -1);
    headlight.setCutoff(25.0);
    headlight.setExponent(20.0);
    
    lightingManager.addLight(headlight, false, true);
    
    //Farolas
    
    streetlights[0].setType(Light::Type::Positional);
    //streetlights[0].setPosition(0.0, 8, 0);
    streetlights[0].setAmbient(0.8, 0.8, 0.8);
    streetlights[0].setDiffuse(1.0, 1.0, 1.0);
    streetlights[0].setSpecular(0.3, 0.3, 0.3);
    streetlights[0].setDirection(0, -0.2, 0);
    streetlights[0].setCutoff(90);
    streetlights[0].setExponent(20.0);
    
    for (int i = 0; i < 8; i++) {
        if (i >= 1) { streetlights[i] = streetlights[i - 1]; }
        
        if ((i % 2) == 0) {
            streetlights[i].setPosition(0, 8.0, i * (TRACK_LONG / 8));
        }
        else {
            streetlights[i].setPosition(TRACK_GAP + TRACK_WIDTH, 8.0, i * (TRACK_LONG / 8));
        }
        
        lightingManager.addLight(streetlights[i]);
    }
    
}

void GameScene::onActivate() {
    Scene::onActivate();
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(75, (float) Application::width / Application::height, 1, CAM_FAR);

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
    
    glEnable(GL_LIGHTING);
    glEnable(GL_SCISSOR_TEST);
    
    // PUNTOS
    
    points.clear();
    for (int i = 15; i <= 60; i += 30) {
        points.push_back(Point(0, 1, i));
    }
    
    for (int i = 15; i <= 60; i += 30) {
        points.push_back(Point(TRACK_GAP + TRACK_WIDTH, 1, i));
    }
    
    /* FOG */
    
    fogEnabled = true;
    glFogf(GL_FOG_DENSITY, 0.05);
    fog();
    
    hud = true;
    
}

void GameScene::onDeactivate() {
    Scene::onDeactivate();
    glDisable(GL_LIGHTING);
    glDisable(GL_SCISSOR_TEST);
}

void GameScene::setNight() {
    day = false;
    globalLight.setAmbient(0.01, 0.01, 0.01);
    globalLight.setDiffuse(0.01, 0.01, 0.01);
    globalLight.setSpecular(0.01, 0.01, 0.01);
    
    updateFogColor();
}

void GameScene::setDay() {
    day = true;
    globalLight.setAmbient(0.90, 0.90, 0.90);
    globalLight.setDiffuse(0.90, 0.90, 0.90);
    globalLight.setSpecular(0.90, 0.90, 0.90);
    
    updateFogColor();
}

void GameScene::fog() {
    if (fogEnabled) {
        glDisable(GL_FOG);
    }
    else {
        glEnable(GL_FOG);
        updateFogColor();
    }
    
    fogEnabled = !fogEnabled;
}

void GameScene::switchRenderMode() {
    renderMode = renderMode == RenderMode::Solid ? RenderMode::Wired : RenderMode::Solid;
    
    if (renderMode == RenderMode::Wired) {
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
    }
    else {
        glEnable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
    }
}

void GameScene::updateFogColor() {
    if (day) {
        GLfloat fogColor[4] = { 0.8, 0.8, 0.8, 0.8 };
        glFogfv(GL_FOG_COLOR, fogColor);
    }
    else {
        GLfloat fogColor[4] = { .0, .0, .0, .0 };
        glFogfv(GL_FOG_COLOR, fogColor);
    }
}

void GameScene::manageEvents(float deltaTime) {
    
    ///////////////////////////////
    /// REGRESAR AL MENU        ///
    ///////////////////////////////
    
    if (Keyboard::isKeyPressedNoRepeat(Keyboard::Key::Escape)) {
        SceneManager::instance().switchTo("MenuScene");
    }
    
    ///////////////////////////////
    /// CONFIGURACIÓN DEL MUNDO ///
    ///////////////////////////////
    
    if (Keyboard::isKeyPressedNoRepeat(Keyboard::Key::L)) {
        if (day) setNight();
        else setDay();
    }
    
    if (Keyboard::isKeyPressedNoRepeat(Keyboard::Key::N)) {
        fog();
    }
    
    if (Keyboard::isKeyPressedNoRepeat(Keyboard::Key::S)) {
        switchRenderMode();
    }
    
    if (Keyboard::isKeyPressedNoRepeat(Keyboard::Key::C)) {
        hud = !hud;
    }
    
    ////////////////////////////
    /// CONTROL DEL VEHÍCULO ///
    ////////////////////////////

    if (Keyboard::isKeyPressed(Keyboard::Key::Up)) {
        speed += 4 * deltaTime;
        if (speed > 57) { speed = 57; }
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

    lightingManager.update(camX, camY, camZ, lookAtX, lookAtY, lookAtZ);

    ///ACTUALIZACIÓN DE LA POSICIÓN DEL VEHÍCULO
    camZ += sin(rad(angle)) * speed * deltaTime;
    camX -= cos(rad(angle)) * speed * deltaTime;
    
    ///ACTUALIZACIÓN DEL PUNTO DONDE MIRA LA CÁMARA
    lookAtZ = camZ + sin(rad(angle));
    lookAtX = camX - cos(rad(angle));
    
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
            //std::cout << score << std::endl;
        }
    }
    
}

void GameScene::render() {
    if (getState() == State::Inactive) {
        //Si la escena está inactiva, salimos directamente.
        return;
    }
    
    
    glScissor(Application::x, Application::y, Application::vWidth, Application::vHeight);
    
    ///////////////////////////////
    /// DIBUJADO DEL ESCENARIO. ///
    ///////////////////////////////
    
    draw(true);
    
    /////////////////////////////////////////
    /// ELEMENTOS SOLIDARIOS A LA CAMARA. ///
    /////////////////////////////////////////
    if (hud) {
        // PUNTUACION / VELOCIDAD //
        
        std::string sSpeed = std::to_string(speed * 3.6);
        texto(Application::x + 50, Application::y + 50, sSpeed.substr(0, sSpeed.find_first_of("."))  + " km/h", BLANCO);
        texto(Application::width - 100, 50, std::to_string((int) visibleScore), BLANCO);
        
        // VELOCIMETRO //
        glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT);
	    glDisable(GL_LIGHTING);
	    glDisable(GL_DEPTH_TEST);
	    glDisable(GL_TEXTURE_2D);
	
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        
        glOrtho(0, Application::vWidth, 0, Application::vHeight, -5, 5);
        
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        
        glPushMatrix();
        
        glColor4f(0, 0, 0, 1);
        glTranslatef(Application::vWidth / 2, 0, 0);
        glBegin(GL_POLYGON);
        for (int i = 0; i < 365; i += 10) {
            glVertex3f(cos(rad(i)) * Application::vWidth * 0.15, sin(rad(i)) * Application::vWidth * 0.15, 0);
        }
        glEnd();
        
        glColor4f(1, 1, 1, 1);
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < 365; i += 10) {
            glVertex3f(cos(rad(i)) * Application::vWidth * 0.15, sin(rad(i)) * Application::vWidth * 0.15, 0);
        }
        glEnd();
        
        glPopMatrix();
        
        glTranslatef(Application::vWidth / 2, 0, 0);
        glRotatef(90 - std::min(180 * ((speed * 3.6) / 200), 170.0), 0, 0, 1);
        glBegin(GL_POLYGON);
            glVertex3f(-0.005, 0, 0);
            glVertex3f(Application::vWidth * 0.005, 0, 0);
            glVertex3f(0, Application::vWidth * 0.1, 0);
        glEnd();
        
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        
        glPopAttrib();
        
        
        // FIN VELOCIMETRO //
        
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        
        // ESPEJO RETROVISOR //
        
        // Configuración de la cámara del retrovisor.
        gluPerspective(45, 2.5, 1, CAM_FAR);
        
        const int REARVIEW_WIDTH = Application::vWidth * 0.3;
        const int REARVIEW_HEIGHT = REARVIEW_WIDTH / 2.5;
        
        glViewport(Application::winWidth / 2 - REARVIEW_WIDTH / 2, Application::winHeight - REARVIEW_HEIGHT - Application::y, REARVIEW_WIDTH, REARVIEW_HEIGHT);
        //Con glScissor se evita que al hacer glClear se limpien los píxeles
        //dibujados anteriormente.
        glScissor(Application::winWidth / 2 - REARVIEW_WIDTH / 2, Application::winHeight - REARVIEW_HEIGHT - Application::y, REARVIEW_WIDTH, REARVIEW_HEIGHT);
        
        glClearColor(.0, .0, .0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        gluLookAt(camX, camY, camZ, camX - cos(rad(-angle)), lookAtY, camZ + sin(rad(-angle)), 0.0, 1.0, 0.0);
        
        // El escenario vuelve a dibujarse desde el punto de vista del espejo.
        draw(false);
        
        // Se restaura la matriz del MODELVIEW.
        glPopMatrix();
        
        glMatrixMode(GL_PROJECTION);
        // Se restaura la matriz de proyección.
        glPopMatrix();
        
        glMatrixMode(GL_MODELVIEW);
        
        /// Fin del retrovisor.
        
        
        
        Application::viewport(); //Restaurar viewport.
    
    }
}

void GameScene::draw(bool hd) {
    if (day && fogEnabled) {
        glClearColor(0.8, 0.8, 0.8, 1.0);
    }
    else if (day) {
        glClearColor(61.f / 255, 75.f / 255, 38.f / 255, 1.0);
    }
    else {
        glClearColor(0, 0, 0, 1.0);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //
    ejes();
    
    // PISTA
    
    glPushMatrix();
    glTranslatef(-TRACK_WIDTH / 2, 0.f, 0.f);
    if (hd) {
        glCallList(renderMode == RenderMode::Wired ? trackWired : track);
    }
    else {
        glCallList(renderMode == RenderMode::Wired ? bTrackWired : bTrack);
    }
    glPopMatrix();
    
    
    // CESPED
    
    // Se dibujan dos terrenos. El superior tiene una resolución
    // alta y poca extensión. Así se logra que la iluminación
    // en los alrededores del coche se vea más o menos bien.
    if (hd) {
        drawTerrain(1.5, 10, -0.1, 0.2);
    }
    
    // El terreno inferior tiene una extensión más grande
    // pero con pocos polígonos.
    drawTerrain(hd ? 10 : 20, hd ? 10 : 3, -0.2);
    
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
    
    // POWERUPS
    for (Point &point : points) {
        point.draw(renderMode == RenderMode::Wired);
    }
}

void GameScene::drawTerrain(float quadSize, int quadsAmount, float y, float tex) {
    
    int ox = camX / quadSize;
    int oz = camZ / quadSize;
    
    glBindTexture(GL_TEXTURE_2D, grassTex);
    
    glColor3f(1.0, 1.0, 1.0);
    
    for (int i = -quadsAmount; i < quadsAmount; i++) {
        for (int j = -quadsAmount; j < quadsAmount; j++) {
            
            glPushMatrix();
            glTranslatef(i * quadSize, 0, j * quadSize);
            
            //glBegin(GL_QUADS);
            glBegin(renderMode == Solid ? GL_QUADS : GL_LINE_STRIP);
    
            glNormal3f(0, 1, 0);
            glTexCoord2f(0, 0);
            glVertex3f(ox * quadSize, y, oz * quadSize);
            glTexCoord2f(tex, 0);
            glVertex3f(ox * quadSize + quadSize, y, oz * quadSize);
            glTexCoord2f(tex, tex);
            glVertex3f(ox * quadSize + quadSize, y, oz * quadSize + quadSize);
            glTexCoord2f(0, tex);
            glVertex3f(ox * quadSize, y, oz * quadSize + quadSize);
            
            glEnd();
            
            glPopMatrix();
        }
    }
    
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GameScene::drawTrack(bool hd, bool wired) {
    glColor3f(0.0, 0.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, trackTex);
    
    GLfloat p[12] = {
                    0, 0, TRACK_LONG,
                    TRACK_WIDTH, 0, TRACK_LONG,
                    TRACK_WIDTH, 0, 0,
                    0, 0, 0,
                    
                    };
    
    quad(&p[0],&p[3],&p[6],&p[9], hd ? 100 : 5,  hd ? 250 : 10, wired);
    glTranslatef(TRACK_GAP + TRACK_WIDTH, 0.0, 0.0);
    quad(&p[0],&p[3],&p[6],&p[9], hd ? 100 : 5,  hd ? 250 : 10, wired);
    glTranslatef(-(TRACK_GAP + TRACK_WIDTH), 0.0, 0.0);
    
    glTranslatef(TRACK_GAP / 2 + TRACK_WIDTH, 0, TRACK_LONG);

    drawCurve(hd ? 30 : 7, hd, wired);
    
    glTranslatef(-(TRACK_GAP / 2 + TRACK_WIDTH), 0, -TRACK_LONG);
    
    glTranslatef(TRACK_GAP / 2 + TRACK_WIDTH, 0, 0);
    glRotatef(180, 0, 1, 0);
    
    drawCurve(30 ? 30 : 7, hd, wired);
    
    glTranslatef(-(TRACK_GAP / 2 + TRACK_WIDTH), 0, 0);
    
    
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GameScene::drawCurve(unsigned int Q, bool hd, bool wired) {
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
        
        quad(&p[0],&p[3],&p[6],&p[9], hd ? 60 : 2, hd ? 60 : 2, wired);
    }
}

void GameScene::drawAd() {
    glTranslatef(-TRACK_WIDTH / 2, 4, 0);
    glBindTexture(GL_TEXTURE_2D, adTex);
    glColor3f(1.0, 1.0, 1.0);
    
    glBegin(renderMode == Solid ? GL_QUADS : GL_LINE_LOOP);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.0, 2.0, 0.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(TRACK_WIDTH, 2.0, 0.0);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(TRACK_WIDTH, 0.0, 0.0);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glTranslatef(TRACK_WIDTH / 2, -4, 0);
    
    if (renderMode == Solid) {    
        glColor3f(0.3, 0.3, 0.3);
    }
    else {
        glColor3f(1.0, 1.0, 1.0);
    }
    
    glBegin(renderMode == Solid ? GL_QUADS : GL_LINE_LOOP);
    //Barra vertical
    glVertex3f(-TRACK_WIDTH / 2, .0, .0);
    glVertex3f(-TRACK_WIDTH / 2 + 0.1, .0, .0);
    glVertex3f(-TRACK_WIDTH / 2 + 0.1, 4.0, .0);
    glVertex3f(-TRACK_WIDTH / 2, 4.0, .0);
    
    glEnd();
    
    glBegin(renderMode == Solid ? GL_QUADS : GL_LINE_LOOP);
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
    glBegin(renderMode == Solid ? GL_QUADS : GL_LINE_STRIP);
    
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
