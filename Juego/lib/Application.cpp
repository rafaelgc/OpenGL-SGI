#include "Application.hpp"

#include "Keyboard.hpp"
#include "SceneManager.hpp"
#include "GL.hpp"

#include <iostream>

unsigned int Application::width = 800;
unsigned int Application::height = 600;
unsigned int Application::winWidth = 800;
unsigned int Application::winHeight = 600;
unsigned int Application::vWidth = 800;
unsigned int Application::vHeight = 600;
unsigned int Application::x = 0;
unsigned int Application::y = 0;

void Application::init(int *argc, char **argv, std::string windowTitle, unsigned int width, unsigned int height) {
    //GLUT settings
    glutInit(argc, argv);
    
    Application::width = Application::winWidth = Application::vWidth = width;
    Application::height = Application::winHeight = Application::vHeight = height;
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow(windowTitle.c_str());
    glutDisplayFunc(draw);
    glutReshapeFunc(reshape);
    glutIdleFunc(loop);
    
    //OpenGL settings
    glEnableClientState(GL_VERTEX_ARRAY | GL_COLOR_ARRAY);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_TEXTURE_2D);
    
    Keyboard::init();
    
}

void Application::run() {
    glutMainLoop();
}


void Application::draw() {
    SceneManager::instance().render();
}

void Application::viewport() {
    
    int w = Application::winWidth;
    int h = Application::winHeight;
    
    float RATIO = width / (float)height;
    float razonAD = float(w)/h;
    float wp,hp;
    
    if(razonAD<RATIO){
        wp = float(w);
        hp = wp/RATIO;
        
        x = 0;
        y = int(h/2.0-hp/2.0);
        
        vWidth = w;
        vHeight = int(hp);
        
    }
    else{
        hp = float(h);
        wp = hp*RATIO;
        
        x = int(w/2.0-wp/2.0);
        y = 0;
        
        vWidth = int(wp);
        vHeight = h;
        
    }
    
    glViewport(x, y, vWidth, vHeight);
}

void Application::reshape(int w, int h) {

    Application::winWidth = w;
    Application::winHeight = h;

    viewport();
}

void Application::loop() {
    SceneManager::instance().manage();
}
