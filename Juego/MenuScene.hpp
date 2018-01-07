#include "lib/Scene.hpp"

class MenuScene : public Scene {
    public:
    MenuScene();
    
    void manageEvents(float deltaTime);
    void logic(float deltaTime);
    void render();
    
    private:
    float tCounter;
};
