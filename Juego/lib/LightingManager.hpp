///////////////////////////////////////////
/// RAFAEL GONZÁLEZ CARRIZO - DIC. 2017 ///
///////////////////////////////////////////

#ifndef LIGHTNINGMANAGER_HPP
#define LIGHTNINGMANAGER_HPP

#include "Light.hpp"

#include <vector>
#include <queue>
#include <iostream>

/* Clase auxiliar*/
class LightState {
    public:
    bool enabled;
    int lightIndex;
    
    LightState() { enabled = false; }
    //LightState(bool enabled) { this->enabled = enabled; this->light = 0; distance = -1.f; }
    
};

class LightHelper {
    public:
    Light *light;
    bool updatePosition;
    bool force;
    float score;
    
    bool enabled;
    GLenum glLight;
    
    
    public:
    LightHelper() { enabled = false; force = false; updatePosition = true; score = 0.0; }
    LightHelper(bool force, bool updatePosition) { enabled = false; this->force = force; this->updatePosition = updatePosition; }
    bool getUpdatePosition() { return updatePosition; }
};

class LightingManagerComparator {
    public:
    bool operator ()(LightHelper ls1, LightHelper ls2) {
        //Cuanto menor sea el score, más visible es la luz
        //y más interesa que se vea.
        return ls1.score < ls2.score;
    }
};

/**
 * OpenGL permite establecer 8 luces de manera simultánea. Esta
 * clase es un planificador que, dadas una serie de luces,
 * decide cuáles deberían estar encendidas y cuáles no en base
 * a su posición para respetar la restricción de 8 luces.
*/
class LightingManager {
    public:
    
    LightingManager();
    
    void setDistance(float distance);
    
    void addLight(Light &light, bool updatePosition = true, bool force = false);
    void update(float x, float y, float z, float lookAtX, float lookAtY, float lookAtZ);
    
    protected:
    int getUselessLightState(float x, float y, float z, float lookAtX, float lookAtY, float lookAtZ, float distance);
    
    private:
    
    float visionDistance;
    //std::vector<std::pair<Light*, LightState>> lights;
    std::vector<LightHelper> lights;
    std::queue<GLenum> freeLights;
    
    static const unsigned int MaxLights = 8;
    LightState lightStates[MaxLights];
    
};

#endif
