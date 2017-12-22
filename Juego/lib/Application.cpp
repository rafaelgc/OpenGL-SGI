#include "Application.hpp"

#include "Keyboard.hpp"
#include "SceneManager.hpp"

#ifdef __GNUC__
#include <GL/freeglut.h>
#else
#include <GL\freeglut.h>
#endif

#include <iostream>

unsigned int Application::width = 800;
unsigned int Application::height = 600;

void Application::init(int *argc, char **argv, std::string windowTitle, unsigned int width, unsigned int height) {
    //GLUT settings
    glutInit(argc, argv);
    
    Application::width = width;
    Application::height = height;
    
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

void Application::reshape(int w, int h) {
    float RATIO = width / (float)height;
    float razonAD= float(w)/h;
    float wp,hp;
    if(razonAD<RATIO){
        wp= float(w);
        hp= wp/RATIO;
        glViewport(0,int(h/2.0-hp/2.0),w,int(hp));
    }
    else{
        hp= float(h);
        wp= hp*RATIO;
        glViewport(int(w/2.0-wp/2.0),0,int(wp),h);
    }

}

void Application::loop() {
    SceneManager::instance().manage();
}
