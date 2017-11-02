#include <iostream>
#include <GL\freeglut.h>
#include "Utilidades.h"

#define PROYECTO "Estrella 3D"

const double CAM_X = 5.0;
const double CAM_Y = 5.0;
const double CAM_Z = 5.0;

//1. La cámara se coloca en cualquier posición mirando hacia el origen.
//2. La esfera se coloca en el origen.
//3. El fov de la cámara será tal que la esfera ocupe toda la cámara (dependerá de la distancia de la cámara al origen y del radio de la esfera).

using namespace std;

static GLuint triangulo;

void drawTriangles(GLfloat x, GLfloat y, GLfloat angle) {
	glPushMatrix();
	glTranslatef(x, y, 0.0);
	glScalef(0.5, 0.5, 0.5);

	const unsigned int N = 6;
	
	for (unsigned int i = 0; i < N; i++) {
		glRotatef(180.0 / N, 0.0, 1.0, 0.0);
		glColor3f((float)0, (float)i / N, (float)i / N);
		glCallList(triangulo);
	}

	glRotatef(180.f, 0.f, 0.f, 1.f);
	for (unsigned int i = 0; i < N; i++) {
		glRotatef(180.0 / N, 0.0, 1.0, 0.0);
		glColor3f((float)i/N, (float)i/N, (float)0);
		glCallList(triangulo);
	}

	glPopMatrix();
}

void display() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(0.0, 0.3, 0.9);
	glutWireSphere(0.5, 15, 15);

	drawTriangles(0.0, 0.0, 0);

	glFlush();

}

void reshape(GLint w, GLint h) {
	float s = min(h, w);
	glViewport(w / 2 - s / 2, 0, s, s);
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	glutCreateWindow(PROYECTO);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	cout << PROYECTO << " en marcha" << endl;

	glEnableClientState(GL_VERTEX_ARRAY | GL_COLOR_ARRAY);
	glEnable(GL_DEPTH_TEST);

	//CONFIGURAR CAMARA
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//0.5 es el radio de la esfera.
	gluPerspective(deg(atan(0.5 / sqrt(CAM_X * CAM_X + CAM_Y * CAM_Y + CAM_Z * CAM_Z))) * 2, 1, 1, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(CAM_X, CAM_Y, CAM_Z, 0.0, 0.0, 0.0, 0.0, 1., 0.0);

	//glViewport(100, 100, 300, 300);

	//CALCULAR COORDENADAS

	GLfloat v[12];

	for (unsigned int i = 0; i < 6; i++) {
		float rad = 3.1415 / 3 + i * PI / 3;
		float x = sin(rad);
		float y = cos(rad);

		v[2 * i] = x;
		v[2 * i + 1] = y;
	}

	//CONFIGURAR TRIANGULO
	triangulo = glGenLists(1);

	glNewList(triangulo, GL_COMPILE);

	glBegin(GL_TRIANGLE_STRIP);

	for (unsigned int i = 0; i < 6; i += 2) {
		//Interior
		glVertex2f(v[2 * i] * 0.7, v[2 * i + 1] * 0.7);

		//Exterior
		glVertex2f(v[2 * i], v[2 * i + 1]);
	}
	glVertex2f(v[0] * 0.7, v[1] * 0.7);
	glVertex2f(v[0], v[1]);

	glEnd();

	glEndList();

	glutMainLoop();

	return EXIT_SUCCESS;
}