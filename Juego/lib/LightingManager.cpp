#include "LightingManager.hpp"

LightingManager::LightingManager() {
    distance = 100.0;
}

void LightingManager::addLight(Light &light) {
    lights.push_back(std::make_pair(&light, LightState(false)));
}

void LightingManager::update(float x, float y, float z) {

    /// 1. Buscar las luces cercanas.
    ///     -Las que están activas se mantienen así.
    ///     -Las que están inactivas se tratan de activar.
    
    for (auto& light : lights) {
    
        if (light.second.enabled == true) { confinue; }
        
        float distanceToLight =
            sqrt((light.first->getX() - x) * (light.first->getX() - x) +
                (light.first->getY() - y) * (light.first->getY() - y) +
                (light.first->getZ() - z) * (light.first->getZ() - z));
                
        if (distanceToLight < visionDistance) {
            
        } 
        
    }
    
    /// 2. Desactivar luces.
    /// Si se quiere activar alguna luz pero ya están todas ocupadas
    /// habrá que decidir si alguna de las visibles es prescindible.

}
