///////////////////////////////
/// RAFAEL GONZÁLEZ CARRIZO ///
///////////////////////////////

/// LIBRERÍA PARA CONSULTAR EN TIEMPO REAL EL
/// ESTADO DE UNA TECLA.
#pragma once

#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

// No están implementadas todas las teclas.
enum Key {Up, Down, Left, Right};

/// Buffer para almacenar el estado de las teclas.
const unsigned int KEYBOARD_BUFFER_SIZE = 256;
extern bool keyState[KEYBOARD_BUFFER_SIZE];

/*Inicializa los callbacks y el buffer.*/
void initKeyboard();

/*Funciones para consultar el teclado. */
bool isKeyPressed(Key key);

/// USO INTERNO

//Callbacks que deben ser ejecutados por GLUT.
void onKeyPressed(unsigned char k, int x, int y);
void onSpecialKeyPressed(int k, int x, int y);
void onSpecialKeyReleased(int key, int x, int y);
void onKeyReleased(unsigned char key, int x, int y);

void setSpecialKeysState(int key, bool pressed);

#endif
