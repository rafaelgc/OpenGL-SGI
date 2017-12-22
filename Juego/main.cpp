#include <iostream>
#ifdef __GNUC__
#include <GL/freeglut.h>
#else
#include <GL\freeglut.h>
#endif
#include <ctime>
#include <string>
#include "Utilidades.h"

#include "lib/SceneManager.hpp"
#include "lib/Scene.hpp"
#include "lib/Application.hpp"
#include "lib/Keyboard.hpp"

#include "GameScene.hpp"


using namespace std;

// ATRIBUTOS DE LA VISTA
const unsigned int WIDTH = 1440, HEIGHT = 900;
const float RATIO = (float)WIDTH / HEIGHT;

int main(int argc, char** argv) {

    FreeImage_Initialise();
    Application::init(&argc, argv, "Juego", WIDTH, HEIGHT);
    
    GameScene gameScene;
    
    SceneManager::instance().addScene(gameScene);
    SceneManager::instance().activateScene("GameScene");
    
    Application::run();
    
	FreeImage_DeInitialise();

    return EXIT_SUCCESS;
}
