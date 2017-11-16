#include <iostream>
#ifdef __GNUC__
#include <GL/freeglut.h>
#else
#include <GL\freeglut.h>
#endif
#include "Utilidades.h"

#define PROYECTO "Reloj analógico"

const double CAM_X = 0.0;
const double CAM_Y = 0.0;
const double CAM_Z = 5.0;
const double ASPECT = 1.0;

using namespace std;

static GLuint triangulo;

GLdouble angle = 0.f;

GLdouble angleCircle = 0.f;
GLdouble angleSec = 0.f;

void loop() {
	static int start = glutGet(GLUT_ELAPSED_TIME);
	int now = glutGet(GLUT_ELAPSED_TIME);

	double deltaTime = (now - start) / 1000.0;
	
	//std::cout << deltaTime << std::endl;

	angle += 10.0 * deltaTime;
	angleCircle += 180 * deltaTime;

	angleSec += 6 * deltaTime;

	glutPostRedisplay();

	start = now;
}

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
		glColor3f((float)i / N, (float)i / N, (float)0);
		glCallList(triangulo);
	}

	glPopMatrix();
}

void drawCircle(GLfloat radius) {
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i <= 300; i++) {
		double angle = 2 * PI * i / 300;
		double x = cos(angle);
		double y = sin(angle);
		glVertex2d(radius * x, radius * y);
	}
	glEnd();
}

void drawGyroscope(GLfloat radius) {
	/// MOVIMIENTO CONTINUO

	/// Circulo 1
	glColor3f(0.5, 0.0, 0.0);
	glPushMatrix();
	glRotatef(angleCircle, 0.f, 1.f, 0.f);
	drawCircle(radius);
	glPopMatrix();

	/// Circulo 2
	glColor3f(0.0, .5, 0.0);
	glPushMatrix();
	glRotatef(angleCircle, 1.f, 0.f, 0.f);
	drawCircle(radius);
	glPopMatrix();

	glColor3f(0.5, .5, 0.0);
	glPushMatrix();
	glRotatef(angleCircle, 1.f, 1.f, 0.f);
	glRotatef(45.f, 0.f, 0.f, 1.f);
	glRotatef(90.f, 1.f, 0.f, 0.f);
	drawCircle(radius);
	glPopMatrix();
}

void display() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor4f(0.0, 0.3, 0.9, 0.1f);

	glPushMatrix();
	glRotatef(angle, 0.f, 1.f, 0.f);
	//glutWireSphere(0.5, 15, 15);
	glPopMatrix();

	///MARCAS HORAS
	glColor3f(0.0, .0, 0.6);
	for (unsigned int i = 0; i < 12; i++) {
		glPushMatrix();

		glRotatef(30 * i, 0.f, 0.f, 1.f);
		glTranslatef(0.f, 0.5f - 0.02f, 0.f);
		//drawCircle(0.02);
		drawGyroscope(0.02);

		glPopMatrix();
	}

	glColor3f(0.0, 0.6, 0.6);
	for (unsigned int i = 0; i < 60; i++) {
		if (i % 5 == 0) continue;
		glPushMatrix();

		glRotatef(6 * i, 0.f, 0.f, 1.f);
		glTranslatef(0.f, 0.5f - 0.01f, 0.f);
		drawCircle(0.01);

		glPopMatrix();
	}

	//SEGUNDO
	glColor3f(0.f, 0.f, 0.f);
	glPushMatrix();
	glRotatef(-angleSec, 0.f, 0.f, 1.f);
	glTranslatef(0.f, 0.1f, 0.f);

	glBegin(GL_POLYGON);
	glVertex2f(-0.01, 0);
	glVertex2f(0.01, 0);
	glVertex2f(0.01, 0.4);
	glVertex2f(-0.01, 0.4);
	glEnd();
	glPopMatrix();
	//drawTriangles(0.0, 0.0, 0);

	//glFlush();
	glutSwapBuffers();

}

void reshape(GLint w, GLint h) {
	//float s = min(h, w);
	GLint s = h;
	glViewport(w / 2 - s / 2, 0, s, s);
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(400, 200);
	glutCreateWindow(PROYECTO);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(loop);
	cout << PROYECTO << " en marcha" << endl;

	glEnableClientState(GL_VERTEX_ARRAY | GL_COLOR_ARRAY);
	glEnable(GL_DEPTH_TEST | GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//CONFIGURAR CAMARA
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//0.5 es el radio de la esfera.
	gluPerspective(deg(asin(0.5 / sqrt(CAM_X * CAM_X + CAM_Y * CAM_Y + CAM_Z * CAM_Z))) * 2, ASPECT, 1, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(CAM_X, CAM_Y, CAM_Z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

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
