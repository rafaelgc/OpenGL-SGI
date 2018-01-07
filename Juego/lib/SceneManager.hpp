#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP

#include <list>
#include <string>

#include "Singleton.hpp"
#include "Scene.hpp"

class Scene; //Forward declaration

class SceneManager : public Singleton<SceneManager> {
    friend class Singleton;
    
    public:
    
    /**
     * @brief Añade una escena al vector, con el nombre que se le especifica.
     * @param scene Escena que se quiere añadir.
     * @param dontAddIfExists Si es <b>true</b>, antes de añadir la escena buscaremos si ya
     * hay alguna con ese nombre, si hay alguna con ese nombre, no la añadiremos. Si es <b>false</b> 
     * (valor por defecto), no se buscará y el programa será más eficiente.
     * */
    void addScene(Scene&scene, bool dontAddIfExists = false);
    
    // Llama a la lógica, gestión de eventos.
    // de las escenas activas.
    void manage();
    
    // Llama al dibujado.
    void render();
    
    /**
     * @brief Activa la escena especificada en el nombre, si existe.
     * @param name Nombre de la escena.
     * */
    void activateScene(std::string name);
    /**
     * @brief Activa la escena especificada y desactiva todas las demás.
     * @param name Nombre de la escena.
     * */
    void switchTo(std::string name);

    /**
     * @brief Desactiva la escena especificada en el nombre, si existe.
     * @param name Nombre de la escena.
     * */
    void deactivateScene(std::string name);
    /**
     * @brief Desactiva todas las escenas y, por tanto, termina el bucle de manage(), lo que puede
     * causar el fin de la ejecución del programa.
     * */
    void deactivateAllScenes();

    /**
     * @brief Pausa la escena especificada en el nombre, si existe.
     * @param name Nombre de la escena.
     * */
    void pauseScene(std::string name);
    
    /**
     * @brief Elimina una escena.
     * 
     */
    void removeScene(std::string name);
    
    /**
     * @return True si TODAS las escenas están inactivas.
     */
    bool allScenesInactive() const;

    
    protected:
    
    /**
     * @param nombre Nombre de la escena.
     * @return Puntero a la escena, NULL si no existe tal escena.
     * */
    Scene* lookForScene(std::string name);
    
    //Limpia la pantalla.
    void startRender();
    
    //Intercambia los buffers.
    void endRender();
    
    /**
     * @brief Almacena un puntero a las escenas de las que se encarga el SceneManager.
     * Aquí es donde se guardan referencias a todas la escenas disponibles.
     * */
    std::list<Scene*> scenes;
    
    //Usado para el cálculo de deltaTime.
    int lastFrameTime;
    
    private:
    SceneManager();
    
};

#endif
