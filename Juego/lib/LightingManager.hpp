#ifndef LIGHTNINGMANAGER_HPP
#define LIGHTNINGMANAGER_HPP

#include "Light.hpp"

#include <vector>

/*class LightState {
    public:
    void setState(bool enabled, GLenum light);
};*/
class LightState {
    public:
    bool enabled;
    GLenum light;
    
    LightState() { enabled = false; light = 0; }
    LightState(bool enabled) { this->enabled = enabled; this->light = 0; }
    
};

class LightingManager {
    public:
    
    void setDistance(float distance);
    
    void addLight(Light &light);
    void update(float x, float y, float z);
    private:
    
    float visionDistance;
    std::vector<std::pair<Light*, LightState>> lights;
};

#endif
