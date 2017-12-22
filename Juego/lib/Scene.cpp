#include "Scene.hpp"

Scene::Scene(std::string name) {
    setState(State::Inactive);
    this->name = name;
}

Scene::~Scene() {
}

Scene::State Scene::getState() const{
    return state;
}

std::string Scene::getName() const{
    return name;
}

void Scene::setState(State state) {
    this->state = state;
}

void Scene::onActivate() {
    setState(State::Active);
}

void Scene::onDeactivate() {
    setState(State::Inactive);
}

void Scene::onPause() {
    setState(State::Paused);
}

void Scene::advanceTime(float deltaTime) {
    if (getState() == State::Inactive) {
        //Si la escena está inactiva, salimos directamente.
        return;
    }

    if (getState() == State::Active) {
        /*Sólo cuando la escena está activa llamamos a los métodos encargados
         de gestionar los eventos y la lógica del juego.*/
        manageEvents(deltaTime);
        logic(deltaTime);
    }

}

void Scene::manageEvents(float deltaTime) {

}
