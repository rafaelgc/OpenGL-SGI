#include "LightingManager.hpp"

#include <cmath>
#include "MathUtil.hpp"

#include <iostream>

#include <queue>

LightingManager::LightingManager() {
    visionDistance = 100.0;
}

void LightingManager::addLight(Light &light, bool updatePosition, bool force) {
    lights.push_back(std::make_pair(&light, LightSettings(force, updatePosition)));
}

void LightingManager::update(float x, float y, float z, float lookAtX, float lookAtY, float lookAtZ) {
//    LightingManagerComparator c;
//    std::priority_queue<LightState, std::vector<LightState>, c> queue;
    
    // Desde este método se ocultan/muestran las luces
    // posicionales para gestionar fácilmente más de 8
    // luces.
    // Las luces direccionales no se ocultan porque
    // afectan a toda la escena.
    
    for (unsigned int i = 0; i < MaxLights; i++) {
        // .distance almacena la distancia a la luz
        // si se conoce y si no -1. Este atributo
        // está pensado para ahorrar cálculos, pues
        // estas distancias interesas en varias iteraciones
        // del algoritmo. No obstante, es importante
        // reiniciarla antes de comenzar la ejecución
        // porque podría contener una distancia
        // incorrecta.
        lightStates[i].distance = -1.f;
    }
    
    /// 1. Buscar las luces cercanas.
    ///     -Las que están activas se mantienen así.
    ///     -Las que están inactivas se tratan de activar.
    ///
    for (unsigned int i = 0; i < lights.size(); i++) {
        // Si la luz está activa, se pasa a otra.
        bool isEnabled = false;
        for (int j = 0; j < MaxLights; j++) {
            if (lightStates[j].enabled && lightStates[j].lightIndex == i) {
                isEnabled = true;
                break;
            }
        }
        
        if (isEnabled) { continue; }
        
        
        float distanceToLight = MathUtil::distance(lights[i].first->getX(), lights[i].first->getY(), lights[i].first->getZ(), x, y, z);
        
        // Si una luz está en el rango de visión y está apagada,
        // es candidata a ser encendida.        
        if (distanceToLight < visionDistance) {
            bool turnedOn = false;
            
            
            // En primer lugar habrá que ver si hay
            // alguna luz sin usar y usar esa.
            
            for (unsigned int j = 0; j < MaxLights & !turnedOn; j++) {
                if (!lightStates[j].enabled) {
                    //ÉXITO. Se enciende.
                    lightStates[j].enabled = true;
                    lightStates[j].lightIndex = i;
                    
                    lights[i].first->enable(GL_LIGHT0 + j);
                    std::cout << "Turned on" << std::endl;
                    turnedOn = true;
                }
            }
            
            //Si todas las luces están siendo usadas se buscará
            //alguna que esté más lejos que esta y se hará el
            //intercambio.
            if (!turnedOn) {
                
                
                int lightCandidate = getUselessLightState(x, y, z, lookAtX, lookAtY, lookAtZ, distanceToLight);
                
                if (lightCandidate >= 0) {
                    lightStates[lightCandidate].lightIndex = i;
                    lights[i].first->enable(GL_LIGHT0 + lightCandidate);
                }
                
            }
            
        } 
        
    }
    
    //std::cout << "update" << std::endl;
    
    /// Algunas luces pueden requerir que su posición sea actualizada.
    for (unsigned int i = 0; i < MaxLights; i++) {
    
        if (lightStates[i].enabled && lights[lightStates[i].lightIndex].second.updatePosition) {
            lights[lightStates[i].lightIndex].first->update(GL_LIGHT0 + i);
        }
    }
    
}

int LightingManager::getUselessLightState(float x, float y, float z, float lookAtX, float lookAtY, float lookAtZ, float distance) {
    unsigned int lightCandidate = -1;
    
    float lx, ly, lz;
    float vx = x - lookAtX;
    float vy = y - lookAtY;
    float vz = z - lookAtZ;
    float lookModule = sqrt(vx * vx + vy * vy + vz * vz);
    
    for (unsigned int j = 0; j < MaxLights; j++) {
        if (!lightStates[j].enabled) { continue; }
        Light* l = lights[lightStates[j].lightIndex].first;
        if (lights[lightStates[j].lightIndex].second.force) { continue; }
        
        lx = x - l->getX();
        ly = y - l->getY();
        lz = z - l->getZ();
        float lmodule = sqrt(lx * lx + ly * ly + lz * lz);
        
        float ang = acos(vx * lx + vy * ly + vz * lz / (lmodule * lookModule));
        
        std::cout << "J: " << j << " ANG: " << ang * 57.29 << " LMODULE: " << lookModule << std::endl;
        
        if (ang * 57.29 > 70) {
            std::cout << "USELESS " << j << " WITH ANGLE " << ang * 57.29 << std::endl;
            return j;
        }
        
        //std::cout << "ANGLE TO " << j << ": " << acos(ang) * 57.29 << std::endl;
        
        /*if (ang > cos(3.14 / 3)) {
            return j;
        }*/
    }
    
    for (unsigned int j = 0; j < MaxLights; j++) {
        Light* l = lights[lightStates[j].lightIndex].first;
        if (lights[lightStates[j].lightIndex].second.force) { continue; }
        
        if (lightStates[j].distance < 0.0) {
            lightStates[j].distance = MathUtil::distance(l->getX(), l->getY(), l->getZ(), x, y, z);
        }
    
        // Si la luz que no se está mostrando está más cerca
        // que la que se muestra, esta última pasa a ser
        // candidata para ser ocultada.
        if (lightStates[j].distance > distance) {
            // Además, de entre todas las candidatas a ser ocultadas, escogemos la más
            // lejana.
            if (lightCandidate == -1 || lightStates[lightCandidate].distance < lightStates[j].distance) {
                lightCandidate = j;
            }
        }
    }
    
    return lightCandidate;
}
