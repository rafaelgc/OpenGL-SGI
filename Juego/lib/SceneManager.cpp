#include "SceneManager.hpp"

#ifdef __GNUC__
#include <GL/freeglut.h>
#else
#include <GL\freeglut.h>
#endif

SceneManager::SceneManager() {
    lastFrameTime = glutGet(GLUT_ELAPSED_TIME);
}

void SceneManager::addScene(Scene &scene, bool dontAddIfExists) {
    if (dontAddIfExists) {
        /*Si el usuario establece este argumento como true, se comprobará si existe una escena
         * ya añadida y, si existe, no se añadirá. Si se establece a false, no se comprobará y
         * será más eficiente.
         * Por defecto viene en false.
         */
        if (this->lookForScene(scene.getName())) {
            return;
        }
    }

    scenes.push_back(&scene);
    
}

void SceneManager::manage() {
    float dt = (glutGet(GLUT_ELAPSED_TIME) - lastFrameTime) / 1000.0;
    
    

    for (auto it = scenes.begin(); it != scenes.end(); ++it) {
        
        (*it)->advanceTime(dt);

    }
    
    glutPostRedisplay();
    
    lastFrameTime = glutGet(GLUT_ELAPSED_TIME);
}

void SceneManager::render() {
    startRender();
    for (auto it = scenes.begin(); it != scenes.end(); ++it) {
        (*it)->render();
    }    
    endRender();
}

void SceneManager::startRender() {
    glClearColor(.0, .0, .0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SceneManager::endRender() {
    glutSwapBuffers();
}

void SceneManager::activateScene(std::string name) {
    Scene *scene;
    scene = lookForScene(name);
    if (scene) {
        scene->onActivate();
    }
}

void SceneManager::activateSceneAndDeactivateTheRest(std::string name) {
    for (auto it = scenes.begin(); it != scenes.end(); ++it) {
        if ((*it)->getName()!=name){
            (*it)->onDeactivate();
        }
    }
    activateScene(name);
}

void SceneManager::deactivateScene(std::string name) {
    Scene *scene = lookForScene(name);
    if (scene) {
        scene->onDeactivate();
    }
}

void SceneManager::pauseScene(std::string name) {
    Scene *scene = lookForScene(name);
    if (scene) {
        scene->onPause();
    }
}

void SceneManager::removeScene(std::string name) {
    deactivateScene(name);
    
    Scene* scene = lookForScene(name);
    for (auto it = scenes.begin(); it!=scenes.end(); ++it) {
        if ((*it)==scene) {
            scenes.erase(it);
            break;
        }
    }
}

void SceneManager::deactivateAllScenes() {
    /*Parar todas la escenas, lo que conlleva a que el bucle de gestión acabe y la aplicación se cierre.
     */
    for (auto it = scenes.begin(); it != scenes.end(); ++it) {
        
        (*it)->onDeactivate();
    }
}

Scene* SceneManager::lookForScene(std::string name) {
    /*La función busca si existe una escena con el nombre indicado, si existe devuelve el puntero,
    si no existe devuelve NULL.*/

    for (auto it = scenes.begin(); it != scenes.end(); ++it) {
        if ((*it)->getName() == name) {
            return *it;
        }
    }
    return nullptr;
}

bool SceneManager::allScenesInactive() const{
    for (auto it = scenes.begin(); it!=scenes.end(); ++it){
        if ((*it)->getState()!=Scene::State::Inactive){
            return false;
        }
    }
    return true;
}


