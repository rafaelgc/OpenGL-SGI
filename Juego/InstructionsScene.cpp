#include "InstructionsScene.hpp"
#include "lib/Keyboard.hpp"
#include "lib/SceneManager.hpp"
#include "lib/Application.hpp"

#include "Utilidades.h"

InstructionsScene::InstructionsScene() : Scene("InstructionsScene") {
}

void InstructionsScene::manageEvents(float deltaTime) {
    if (Keyboard::isKeyPressedNoRepeat(Keyboard::Key::Escape)) {
        SceneManager::instance().switchTo("MenuScene");
    }
    
}

void InstructionsScene::logic(float deltaTime) {
}

void InstructionsScene::render() {
    if (getState() == State::Inactive) {
        //Si la escena está inactiva, salimos directamente.
        return;
    }
    
    texto(Application::x + Application::vWidth * 0.05, Application::y + Application::vHeight * 0.9, "Flecha arriba/abajo: acelerar y frenar", BLANCO, GLUT_BITMAP_HELVETICA_12);
    texto(Application::x + Application::vWidth * 0.05, Application::y + Application::vHeight * 0.85, "Flecha izq./der.: giro del vehículo (debe estar en movimiento)", BLANCO, GLUT_BITMAP_HELVETICA_12);
    texto(Application::x + Application::vWidth * 0.05, Application::y + Application::vHeight * 0.80, "Espacio: freno rápido", BLANCO, GLUT_BITMAP_HELVETICA_12);
    texto(Application::x + Application::vWidth * 0.05, Application::y + Application::vHeight * 0.75, "N: activar/desactivar niebla", BLANCO, GLUT_BITMAP_HELVETICA_12);
    texto(Application::x + Application::vWidth * 0.05, Application::y + Application::vHeight * 0.70, "L: dia/noche", BLANCO, GLUT_BITMAP_HELVETICA_12);
    texto(Application::x + Application::vWidth * 0.05, Application::y + Application::vHeight * 0.65, "S: activar/desactivar modo alámbrico", BLANCO, GLUT_BITMAP_HELVETICA_12);
    texto(Application::x + Application::vWidth * 0.05, Application::y + Application::vHeight * 0.60, "C: activar/desactivar hud", BLANCO, GLUT_BITMAP_HELVETICA_12);
    
    texto(Application::x + Application::vWidth * 0.05, Application::y + Application::vHeight * 0.05, "Rafael González Carrizo", BLANCO, GLUT_BITMAP_HELVETICA_12);
}
