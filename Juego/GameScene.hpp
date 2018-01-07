#ifndef GAMESCENE_HPP
#define GAMESCENE_HPP

#include "lib/Scene.hpp"
#include "lib/GL.hpp"
#include "lib/LightingManager.hpp"
#include "lib/Light.hpp"

#include "Point.hpp"

#include <vector>

class GameScene : public Scene {
    public:
    enum RenderMode { Solid, Wired };
    
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
    
    void setDay();
    void setNight();
    
    void fog(); // Enciende/apaga.
    void updateFogColor();
    
    private:
    
    void draw(bool hd = true);
    void drawTrack(bool hd = true, bool wired = false);
    void drawCurve(unsigned int q,  bool hd = true, bool wired = false);
    void drawAd();
    void drawBackground();
    void drawTerrain(float quadSize, int quadsAmount, float y, float tex = 1);
    
    void switchRenderMode();
    
    float angle, speed;
    float camX, camY, camZ;
    float lookAtX, lookAtY, lookAtZ;
    
    GLuint adTex, trackTex, grassTex, backgroundTex;
    
    GLuint track, bTrack, trackWired, bTrackWired;
    
    GLfloat headlightPosition[4];
    
    std::vector<Point> points;
    int score;
    float visibleScore;
    
    
    Light headlight, globalLight, streetlights[8];
    LightingManager lightingManager;
    bool day;
    bool hud;
    bool fogEnabled;
    
    RenderMode renderMode;
};

#endif
