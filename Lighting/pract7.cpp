#include <iostream>
#ifdef __GNUC__
#include <GL/freeglut.h>
#else
#include <GL\freeglut.h>
#endif
#include <ctime>
#include "Utilidades.h"


void init()
// Funcion de inicializacion propia
{
  // Mensajes por consola
  glClearColor(0,0,0,1); // Color de fondo
  // Inicialización de luces
  GLfloat Al0[]={0.2,0.2,0.2,1.0}; // Color ambiental de la luz
  GLfloat Dl0[]={1.0,1.0,0.0,1.0}; // Color difuso de la luz
  GLfloat Sl0[]={1.0,1.0,0.0,1.0}; // Color especular de la luz
  glLightfv(GL_LIGHT0, GL_AMBIENT, Al0); // Caracteristicas de LIGHT0
  glLightfv(GL_LIGHT0, GL_DIFFUSE, Dl0);
  glLightfv(GL_LIGHT0, GL_SPECULAR, Sl0);
  glEnable(GL_LIGHT0);
  // Caracteristicas del render
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
}
void display()
// Funcion de atencion al dibujo
{
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Borra la pantalla y el Z-buffer
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   /* Aunque variemos la posición de la cámara, la esfera siempre se ve iluminada de frente
   como si la luz estuviera donde esta el observador */
   // gluLookAt(1,0,1,0,0,0,0,1,0);
   gluLookAt(0,1,1,0,0,0,0,1,0); // Situa la camara
   glColor3f(1,1,1);
   glutSolidSphere(0.5,20,20); // Dibuja una esfera blanca
   glutSwapBuffers(); // Finaliza el dibujo
}
void reshape(GLint w, GLint h)
// Funcion de atencion al redimensionamiento
{
  // Usamos toda el area de dibujo
  glViewport(0,0,w,h);
  // Definimos la camara (matriz de proyeccion)
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  float razon = (float) w / h;
  gluPerspective(90,razon,0.1,10);
}

int main(int argc, char** argv)
// Programa principal
{
   glutInit(&argc, argv); // Inicializacion de GLUT
   glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH); // Alta de buffers a usar
   glutInitWindowSize(400,400); // Tamanyo inicial de la ventana
   glutCreateWindow("Luz"); // Creacion de la ventana con su titulo
   glutDisplayFunc(display); // Alta de la funcion de atencion a display
   glutReshapeFunc(reshape); // Alta de la funcion de atencion a reshape
   init(); // Funcion propia de inicializacion
   glutMainLoop(); // Puesta en marcha del programa
 return 0;
}
