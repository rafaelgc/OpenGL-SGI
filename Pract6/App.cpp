#include "App.hpp"

#ifdef __GNUC__
#include <GL/freeglut.h>
#else
#include <GL\freeglut.h>
#endif

App* App::theInstance = nullptr;

App& App::instance() {
    if (theInstance == nullptr) {
        theInstance = new App();
    }
    
    return *theInstance;
}

App::App() {
}

void App::setup(int argc, char **argv) {
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	
	glutDisplayFunc(this->displayFunc);
	glutReshapeFunc(this->reshapeFunc);
	glutIdleFunc(this->iddleFunc);
	
	//glutInitWindowSize(400, 200);
	//glutCreateWindow(PROYECTO);
}

void App::iddleFunc() {

}

void App::displayFunc() {
}

void App::reshapeFunc(int w, int h) {

}
