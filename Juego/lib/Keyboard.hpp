///////////////////////////////////////////
/// RAFAEL GONZÁLEZ CARRIZO - DIC. 2017 ///
///////////////////////////////////////////

/// LIBRERÍA PARA CONSULTAR EN TIEMPO REAL EL
/// ESTADO DE UNA TECLA.

#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

class Keyboard {
    public:
    
    // No están implementadas todas las teclas.
    enum Key {Up, Down, Left, Right, Space, Escape, Enter, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z};
    
    /*Inicializa los callbacks y el buffer.*/
    static void init();
    
    /*Funciones para consultar el teclado. */
    static bool isKeyPressed(Key key);
    static bool isKeyPressedNoRepeat(Key key);
    
    protected:
    /// Buffer para almacenar el estado de las teclas.
    static const unsigned int KEYBOARD_BUFFER_SIZE = 64;
    static bool keyState[KEYBOARD_BUFFER_SIZE];
    static bool prevKeyState[KEYBOARD_BUFFER_SIZE];
    
    static void setSpecialKeyState(int key, bool pressed);
    static void setKeyState(unsigned char k, bool pressed);

    //Callbacks que deben ser ejecutados por GLUT.
    static void onKeyPressed(unsigned char k, int x, int y);
    static void onSpecialKeyPressed(int k, int x, int y);
    static void onSpecialKeyReleased(int key, int x, int y);
    static void onKeyReleased(unsigned char key, int x, int y);
};








#endif
