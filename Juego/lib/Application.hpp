#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <string>

class Application {
    public:
    // width, height: tamaño inicial de la ventana.
    // Establece la relación de aspecto.
    static unsigned int width, height;
    
    // winWidth, winHeight: tamaño actual de la ventana.
    static unsigned int winWidth, winHeight;
    
    // Punto de origen de dibujado.
    static unsigned int x, y;
    // Ancho y alto del viewport.
    static unsigned int vWidth, vHeight;
    
    static void init(int *argc, char **argv, std::string windowTitle, unsigned int width, unsigned int height);
    static void run();
    static void viewport();
    
    protected:
    
    static void draw();
    static void reshape(int width, int height);
    static void loop();
    
};

#endif
