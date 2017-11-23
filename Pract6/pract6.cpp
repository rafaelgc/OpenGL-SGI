#include <iostream>
#ifdef __GNUC__
#include <GL/freeglut.h>
#else
#include <GL\freeglut.h>
#endif
#include <ctime>
#include "Utilidades.h"

#define PROYECTO "Interfaz de conduccion"

using namespace std;

class Game {
public:
	Game();

	void draw();
	void loop(float deltaTime);

	void onKeyPressed(unsigned char k, int x, int y);

private:
	//Movimiento del vehiculo.
	float angle, module;

	//Posicion de la camara.
	GLfloat camX, camY, camZ;
	GLfloat lookAtX, lookAtY, lookAtZ;
};

Game::Game() {
	angle = module = 0.f;

	camX = 0.f;
	camY = 1.f;
	camZ = 0.f;

	lookAtX = 0.f;
	lookAtY = 1.f;
	lookAtZ = -1.f;
}

void Game::draw() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat pts[] = { -4.f, .0, .0, 4.f, .0, .0, 4.f, .0, -10.f, -4.f, .0, -10.f };

	glColor3fv(GRISCLARO);
	quad(&pts[0], &pts[3], &pts[6], &pts[9]);
}

void Game::loop(float deltaTime) {

	camZ -= module * deltaTime;
	lookAtZ -= module * deltaTime;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camX, camY, camZ, lookAtX, lookAtY, lookAtZ, 0.0, 1.0, 0.0);
}

void Game::onKeyPressed(unsigned char k, int x, int y) {
	if (k == 'w') {
		module += 0.1;
	}
	else if (k == 's') {
		module += 0.1;
	}
}

///////
Game game;

void loop() {
	static int start = glutGet(GLUT_ELAPSED_TIME);
	int now = glutGet(GLUT_ELAPSED_TIME);
	double deltaTime = (now - start) / 1000.0;

	game.loop(deltaTime);

	glutPostRedisplay();

	start = now;
}

void display() {
	game.draw();
	glutSwapBuffers();

}

void onKey(unsigned char key, int x, int y) {
	std::cout << key << time(NULL) << std::endl;
	game.onKeyPressed(key, x, y);
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
	glutKeyboardFunc(onKey);

	cout << PROYECTO << " en marcha" << endl;

	glEnableClientState(GL_VERTEX_ARRAY | GL_COLOR_ARRAY);
	glEnable(GL_DEPTH_TEST);

	//CONFIGURAR CAMARA
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//0.5 es el radio de la esfera.
	gluPerspective(45, 2, 1, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 1.0, 0.0, 0.0, 1.0, -1.0, 0.0, 1.0, 0.0);

	glutMainLoop();

	return EXIT_SUCCESS;
}
