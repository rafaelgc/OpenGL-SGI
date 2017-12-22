#include "Keyboard.hpp"

#ifdef __GNUC__
#include <GL/freeglut.h>
#else
#include <GL\freeglut.h>
#endif

#include <cctype>
#include <iostream>

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
    setKeyState(k, true);
}

void Keyboard::onKeyReleased(unsigned char k, int x, int y) {
    setKeyState(k, false);
}

void Keyboard::setKeyState(unsigned char k, bool pressed) {
    k = std::tolower(k);
    
    if (k == ' ') {
        keyState[Key::Space] = pressed;
    }
    
    for (int i = 'a'; i <= 'z'; i++) {
        if (k == i) {
            keyState[Key::A + i - 'a'] = pressed;
            break;
        }
    }
}

void Keyboard::onSpecialKeyPressed(int k, int x, int y) {
    setSpecialKeyState(k, true);
}

void Keyboard::onSpecialKeyReleased(int k, int x, int y) {
    setSpecialKeyState(k, false);
}


void Keyboard::setSpecialKeyState(int key, bool state) {
    /*if ((glutGetModifiers() & GLUT_ACTIVE_CTRL)) { 
    std::cout << "CTRL " << state << std::endl; keyState[Key::Ctrl] = state; }
    if ((glutGetModifiers() & GLUT_ACTIVE_ALT)) { std::cout << "ALT " << state << std::endl; keyState[Key::Alt] = state; }
    if ((glutGetModifiers() & GLUT_ACTIVE_SHIFT)) { std::cout << "SHIFT " << state << std::endl; keyState[Key::Shift] = state; }*/

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
