#include "MenuScene.hpp"
#include "lib/Keyboard.hpp"
#include "lib/SceneManager.hpp"
#include "lib/Application.hpp"

#include "Utilidades.h"
#include <iostream>

MenuScene::MenuScene() : Scene("MenuScene") {
    tCounter = 0.f;
}

void MenuScene::manageEvents(float deltaTime) {
    if (Keyboard::isKeyPressedNoRepeat(Keyboard::Key::Escape)) {
        SceneManager::instance().deactivateAllScenes();
    }
    
    if (Keyboard::isKeyPressed(Keyboard::Key::Enter)) {
        SceneManager::instance().switchTo("GameScene");
    }
    
    if (Keyboard::isKeyPressed(Keyboard::Key::H)) {
        SceneManager::instance().switchTo("InstructionsScene");
    }
}

void MenuScene::logic(float deltaTime) {
    tCounter += deltaTime;
    if (tCounter >= 1.6) { tCounter = 0.f; }
}

void MenuScene::render() {
    if (getState() == State::Inactive) {
        //Si la escena está inactiva, salimos directamente.
        return;
    }
    
    if (tCounter < 0.8) {
        texto(Application::x + (Application::vWidth / 2) * 0.9, Application::y + Application::vHeight / 2, "[ENTER]", BLANCO);
    }
    
    texto(Application::x + (Application::vWidth / 2) * 0.8, Application::y + Application::vHeight * 0.3, "[H]:   instrucciones", BLANCO);
    texto(Application::x + (Application::vWidth / 2) * 0.8, Application::y + Application::vHeight * 0.2, "[ESC]: salir", BLANCO);
}
