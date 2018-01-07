#include "lib/Scene.hpp"

class InstructionsScene : public Scene {
    public:
    InstructionsScene();
    
    void manageEvents(float deltaTime);
    void logic(float deltaTime);
    void render();
    
    private:
};
