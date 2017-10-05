#include <iostream>
#include <GL\freeglut.h>

#define PROYECTO "Primer programa en OpenGL"

using namespace std;

void display() {
	glClearColor(0.0, 0.0, 0.3, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

void reshape(GLint w, GLint h) {
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 400);
	glutCreateWindow(PROYECTO);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutPositionWindow(50, 600 - 400);

	cout << PROYECTO << " en marcha" << endl;

	glutMainLoop();

	return EXIT_SUCCESS;
}