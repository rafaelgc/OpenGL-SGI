#include "LightingManager.hpp"

#include <cmath>
#include "MathUtil.hpp"

#include <iostream>

#include <queue>
#include <algorithm>

LightingManager::LightingManager() {
    visionDistance = 100.0;
    
    for (unsigned int i = 0; i < MaxLights; i++) {
        freeLights.push(GL_LIGHT0 + i);
    }
}

void LightingManager::addLight(Light &light, bool updatePosition, bool force) {
    LightHelper lh(force, updatePosition);
    lh.light = &light;
    
    lights.push_back(lh);
    
}

void LightingManager::update(float x, float y, float z, float lookAtX, float lookAtY, float lookAtZ) {

    /// Calcular la visibilidad de cada una de las luces para determinar
    /// cuáles mostrar.
    
    float lx, ly, lz;
    float vx = x - lookAtX;
    float vy = y - lookAtY;
    float vz = z - lookAtZ;
    float lookModule = sqrt(vx * vx + vy * vy + vz * vz);
    
    //std::priority_queue<LightHelper*, std::vector<LightHelper*>, LightingManagerComparator> queue;
    
    for (unsigned int i = 0; i < lights.size(); i++) {
        if (lights[i].force) { lights[i].score = -1.0; continue; }
        
        Light* l = lights[i].light;
        
        float score = 0.0;
        
        lx = x - l->getX();
        ly = y - l->getY();
        lz = z - l->getZ();
        float lmodule = sqrt(lx * lx + ly * ly + lz * lz);
        
        
        score = lmodule;
        
        /*
        /// DESCOMENTAR PARA TENER EN CUENTA EL ÁNGULO DE VISIÓN.
        float ang = acos(vx * lx + vy * ly + vz * lz / (lmodule * lookModule)) * 57.29;
        //Se penaliza cuando la luz no está en el ángulo de visión.
        if (ang > 100) {
            score *= 2000;
        }
        */
        lights[i].score = score;
        
        //queue.push(&lights[i]);
    }
    
    LightingManagerComparator cmp;
    std::sort(lights.begin(), lights.end(), cmp);
    
    
    
    /// Las luces menos prioritarias pasan a estar libres.
    //std::queue<GLenum> freeLights;
    for (unsigned int i = MaxLights; i < lights.size(); i++) {
        if (lights[i].enabled) {
            freeLights.push(lights[i].glLight);
        }
    }
    
    /// Se activan las luces más prioritarias.
    const unsigned int ML = MaxLights;
    for (unsigned int i = 0; i < std::min<const unsigned int>(ML, lights.size()); i++) {
        
        /// Si la luz ya estaba usada no hay que hacer nada.
        /// Si no lo estaba habrá activarla.
        
        if (!lights[i].enabled) {
            for (unsigned int i = 0; i < lights.size(); i++) {
                if (lights[i].enabled && lights[i].glLight == freeLights.front()) {
                    lights[i].enabled = false;
                }
            }
            
            lights[i].enabled = true;
            lights[i].glLight = freeLights.front();
            lights[i].light->enable(lights[i].glLight);
            freeLights.pop();
            
        }
        
    }
    
    for (unsigned int i = 0; i < lights.size(); i++) {
    
        if (lights[i].enabled && (lights[i].light->needsUpdate() || lights[i].updatePosition)) {
            lights[i].light->update(lights[i].glLight);
        }
    }
    
}

