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

GLdouble angleCircle = 0.f, angleSec = 0.f;

//Hora actual.
int sec = 0, minute = 0, hour = 0;

void setupTime() {
    time_t ti;
    time(&ti);
    tm* t = localtime(&ti);
    
    hour = t->tm_hour;
    minute = t->tm_min;
    sec = t->tm_sec;
    
}

void loop() {
	static int start = glutGet(GLUT_ELAPSED_TIME);
	int now = glutGet(GLUT_ELAPSED_TIME);

	double deltaTime = (now - start) / 1000.0;
	
	angleCircle += 180 * deltaTime;
	angleSec += 6 * deltaTime;

	glutPostRedisplay();

	start = now;
}

void secFunc(int millis) {
    //sec=1;
    sec++;
    if (sec == 60) {
        sec = 0;
        minute++;
        if (minute == 60) {
            minute = 0;
            hour++;
            if (hour == 12) {
                hour = 0;
            }
        }
    }
    std::cout << hour << ":" << minute << ":" << sec << std::endl;
    
    glutTimerFunc(1000, secFunc, 0);
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

void drawSolidCircle(GLfloat radius) {
	glBegin(GL_POLYGON);
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

    /// Círculo 3
	glColor3f(0.5, .5, 0.0);
	glPushMatrix();
	glRotatef(angleCircle, 1.f, 1.f, 0.f);
	glRotatef(45.f, 0.f, 0.f, 1.f);
	glRotatef(90.f, 1.f, 0.f, 0.f);
	drawCircle(radius);
	glPopMatrix();
}

void drawSpheres(GLfloat radius) {
	/// MOVIMIENTO CONTINUO

	/// Circulo 2
	glColor3f(0.0, .5, 0.0);
	glPushMatrix();
	glRotatef(90, 1.f, 0.f, 0.f);
	drawCircle(radius);
	
	glRotatef(45 + angleCircle, 0.f, 0.f, 1.f);
	glTranslatef(radius, 0.f, 0.f);
	glutSolidSphere(0.01, 20, 20);
	
	glPopMatrix();

    // Círculo 3
	glColor3f(0.5, .5, 0.0);
	glPushMatrix();
	//glRotatef(angleCircle, 1.f, 1.f, 0.f);
	glRotatef(45.f, 0.f, 0.f, 1.f);
	glRotatef(90.f, 1.f, 0.f, 0.f);
	drawCircle(radius);
	
	glRotatef(90 + angleCircle, 0.f, 0.f, 1.f);
	glTranslatef(radius, 0.f, 0.f);
	glutSolidSphere(0.01, 20, 20);
	
	glPopMatrix();
	
	/// Circulo 1
	glColor3f(0.5, 0.0, 0.0);
	glPushMatrix();
	drawCircle(radius);
	
	glRotatef(angleCircle, 0.f, 0.f, 1.f);
	glTranslatef(radius, 0.f, 0.f);
	glutSolidSphere(0.01, 20, 20);
	
	glPopMatrix();
}

void drawSeconds() {
    //BARRA SEGUNDO (CONTINUA)
	glColor3f(0.f, 0.f, 0.f);
	glPushMatrix();
	glRotatef(-angleSec, 0.f, 0.f, 1.f);
	glTranslatef(0.f, 0.1f, 0.f);

	glBegin(GL_POLYGON);
	glVertex2f(-0.01, 0);
	glVertex2f(0.01, 0);
	glVertex2f(0.0, 0.4);
	glEnd();
	glPopMatrix();
}

void drawMinutes() {
    //BARRA MINUTOS (DISCRETA)
	glColor3f(0.3f, 0.3f, 0.3f);
	glPushMatrix();
	glRotatef(-minute * 6, 0.f, 0.f, 1.f);
	glTranslatef(0.f, 0.25f, 0.f);

	glBegin(GL_POLYGON);
	glVertex2f(-0.01, 0);
	glVertex2f(0.01, 0);
	glVertex2f(0.0, 0.25);
	glEnd();
	glPopMatrix();
}

void drawHours() {
    //BARRA HORAS (CONTINUO)
	glColor3f(0.5f, 0.5f, 0.5f);
	glPushMatrix();
	glRotatef(-hour * 30 - minute / 60.f * 30, 0.f, 0.f, 1.f);
	glTranslatef(0.f, 0.4f, 0.f);

	glBegin(GL_POLYGON);
	glVertex2f(-0.01, 0);
	glVertex2f(0.01, 0);
	glVertex2f(0.0, 0.1);
	glEnd();
	glPopMatrix();
}

void display() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//AGUJAS
	drawSeconds();
	drawMinutes();
	drawHours();
	
	drawSpheres(0.1);

	///MARCAS HORAS
	glColor3f(0.0, .0, 0.6);
	for (unsigned int i = 0; i < 12; i++) {
		glPushMatrix();

		glRotatef(30 * i, 0.f, 0.f, 1.f);
		glTranslatef(0.f, 0.5f - 0.02f, 0.f);
		drawGyroscope(0.02);

		glPopMatrix();
	}

	glColor3f(0.0, 0.6, 0.6);
	for (unsigned int i = 0; i < 60; i++) {
	
	    if (i == sec) {
	        glPushMatrix();
	        glRotatef(360 - 6 * i, 0.f, 0.f, 1.f);
    		glTranslatef(0.f, 0.5f - (i % 5 == 0 ? 0.02 : 0.01), 0.f);
	        glutSolidSphere( i % 5 == 0 ? 0.02 : 0.01, 20, 20);
	        glPopMatrix();
	    }
	
		if (i % 5 == 0) continue;
		glPushMatrix();

		glRotatef(6 * i, 0.f, 0.f, 1.f);
		glTranslatef(0.f, 0.5f - 0.01f, 0.f);
		drawCircle(0.01);

		glPopMatrix();
	}

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
	
	glutTimerFunc(1000, secFunc, 0);

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

    //CONFIGURAR HORA
    setupTime();
    
    angleSec = (sec / 60.f) * 360;

	glutMainLoop();

	return EXIT_SUCCESS;
}
