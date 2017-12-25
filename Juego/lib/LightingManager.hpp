#ifndef LIGHTNINGMANAGER_HPP
#define LIGHTNINGMANAGER_HPP

#include "Light.hpp"

#include <vector>
#include <iostream>

/* Clase auxiliar*/
class LightState {
    public:
    bool enabled;
    GLenum light;
    float distance;
    int lightIndex;
    
    LightState() { enabled = false; light = 0; distance = -1.f; }
    //LightState(bool enabled) { this->enabled = enabled; this->light = 0; distance = -1.f; }
    
};

class LightSettings {
    public:
    bool updatePosition;
    bool force;
    public:
    LightSettings() { force = false; updatePosition = true; }
    LightSettings(bool force, bool updatePosition) { this->force = force; this->updatePosition = updatePosition; std::cout << this->updatePosition << std::endl; }
    bool getUpdatePosition() { return updatePosition; }
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
    std::vector<std::pair<Light*, LightSettings>> lights;
    
    static const unsigned int MaxLights = 8;
    LightState lightStates[MaxLights];
    
};

#endif
