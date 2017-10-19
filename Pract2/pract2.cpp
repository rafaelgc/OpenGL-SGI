#include <iostream>
#include <GL\freeglut.h>

#define PROYECTO "Estrella de David"

using namespace std;

/*
GLfloat coords[9] = { 0.0, 0.7, 0.0, 0.0, 1.0, 0.0,
				 -1.0, 0.0, 0.0};
GLuint indices[3] = { 0, 1, 2 };
*/

static GLuint triangulo;

void display() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glCallList(triangulo);

	glFlush();
}

void reshape(GLint w, GLint h) {
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutCreateWindow(PROYECTO);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	cout << PROYECTO << " en marcha" << endl;

	glEnableClientState(GL_VERTEX_ARRAY|GL_COLOR_ARRAY);

	//CALCULAR COORDENADAS

	GLfloat v[12];

	for (unsigned int i = 0; i < 6; i++) {
		float rad = 3.1415 / 3 + i * 3.1415/3; //PI ------------------------
		float x = sin(rad);
		float y = cos(rad);

		v[2 * i] = x;
		v[2 * i + 1] = y;
	}
	
	//CONFIGURAR TRIANGULO
	triangulo = glGenLists(1);

	glNewList(triangulo, GL_COMPILE);
	
	glColor3f(0.0, 0.0, 0.3);
	
	glBegin(GL_TRIANGLE_STRIP);

	for (unsigned int i = 0; i < 6; i+=2) {
		//Interior
		glVertex2f(v[2 * i] * 0.7, v[2 * i + 1] * 0.7);

		//Exterior
		glVertex2f(v[2 * i], v[2 * i + 1]);
	}
	glVertex2f(v[0] * 0.7, v[1] * 0.7);
	glVertex2f(v[0], v[1]);

	glEnd();

	glBegin(GL_TRIANGLE_STRIP);

	for (unsigned int i = 0; i < 6; i += 2) {
		//Interior
		glVertex2f(v[2 * i + 2] * 0.7, v[2 * i + 3] * 0.7);

		//Exterior
		glVertex2f(v[2 * i + 2], v[2 * i + 3]);
	}
	glVertex2f(v[2] * 0.7, v[3] * 0.7);
	glVertex2f(v[2], v[3]);

	glEnd();

	glEndList();

	glutMainLoop();

	return EXIT_SUCCESS;
}