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
#include "MenuScene.hpp"
#include "InstructionsScene.hpp"

#include <iostream>

using namespace std;

// ATRIBUTOS DE LA VENTANA
const unsigned int WIDTH = 1024, HEIGHT = 768;

int main(int argc, char** argv) {

    FreeImage_Initialise();
    Application::init(&argc, argv, "Juego", WIDTH, HEIGHT);
    
    std::cout << "INSTRUCCIONES" << std::endl;
    std::cout << "Flecha arriba/abajo: acelerar y frenar" << std::endl;
    std::cout << "Flecha izq./der.: giro del vehículo (debe estar en movimiento)" << std::endl;
    std::cout << "Espacio: freno rápido" << std::endl;
    std::cout << "N: activar/desactivar niebla" << std::endl;
    std::cout << "L: dia/noche" << std::endl;
    std::cout << "S: activar/desactivar modo alámbrico" << std::endl;
    std::cout << "C: activar/desactivar hud" << std::endl;
    
    GameScene gameScene;
    MenuScene menuScene;
    InstructionsScene instructionsScene;
    
    SceneManager::instance().addScene(menuScene);
    SceneManager::instance().addScene(gameScene);
    SceneManager::instance().addScene(instructionsScene);
    
    SceneManager::instance().activateScene("MenuScene");
    
    Application::run();
    
	FreeImage_DeInitialise();

    return EXIT_SUCCESS;
}
