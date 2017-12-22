#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <string>

class Application {
    public:
    static unsigned int width, height;
    
    static void init(int *argc, char **argv, std::string windowTitle, unsigned int width, unsigned int height);
    static void run();
    protected:
    
    static void draw();
    static void reshape(int width, int height);
    static void loop();
    
};

#endif
