#ifndef GAMESCENE_HPP
#define GAMESCENE_HPP

#include "lib/Scene.hpp"
#include "lib/GL.hpp"

#include "Point.hpp"

#include <vector>

class GameScene : public Scene {
    public:
    static const float TRACK_GAP;
    static const float TRACK_WIDTH;
    static const float TRACK_LONG;
    
    static const float CAM_FAR;
    
    GameScene();
    void manageEvents(float deltaTime);
    void logic(float deltaTime);
    void render();
    
    void onActivate();
    void onDeactivate();
    
    private:
    
    void drawTrack();
    void drawCurve(unsigned int q);
    void drawAd();
    void drawBackground();
    
    float angle, speed;
    float camX, camY, camZ;
    float lookAtX, lookAtY, lookAtZ;
    
    GLuint adTex, trackTex, grassTex, backgroundTex;
    
    GLuint track;
    
    GLfloat headlightPosition[4];
    
    std::vector<Point> points;
    int score;
};

#endif
