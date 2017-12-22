#include "Keyboard.hpp"

#ifdef __GNUC__
#include <GL/freeglut.h>
#else
#include <GL\freeglut.h>
#endif

bool Keyboard::keyState[KEYBOARD_BUFFER_SIZE];

void Keyboard::init() {
    for (unsigned int i = 0; i < KEYBOARD_BUFFER_SIZE; i++) {
        keyState[i] = false;
    }
    
    glutKeyboardFunc(onKeyPressed);
    glutSpecialFunc(onSpecialKeyPressed);
    glutKeyboardUpFunc(onKeyReleased);
    glutSpecialUpFunc(onSpecialKeyReleased);
}

bool Keyboard::isKeyPressed(Key key) {
    return keyState[key];
}

void Keyboard::onKeyPressed(unsigned char k, int x, int y) {
    // SIN IMPLEMENTAR.
}

void Keyboard::onSpecialKeyPressed(int k, int x, int y) {
    setSpecialKeysState(k, true);
}

void Keyboard::onSpecialKeyReleased(int k, int x, int y) {
    setSpecialKeysState(k, false);
}

void Keyboard::onKeyReleased(unsigned char k, int x, int y) {
    // SIN IMPLEMENTAR.
}

void Keyboard::setSpecialKeysState(int key, bool state) {
    if (key == GLUT_KEY_UP) {
      keyState[Key::Up] = state;
    } else if (key == GLUT_KEY_DOWN) {
      keyState[Key::Down] = state;
    }
    else if (key == GLUT_KEY_LEFT) {
      keyState[Key::Left] = state;
    }
    else if (key == GLUT_KEY_RIGHT) {
      keyState[Key::Right] = state;
    }
}
