#ifndef POINT_HPP
#define POINT_HPP

#include "lib/GL.hpp"

class Point {
    
    public:
    
    Point(GLfloat x, GLfloat y, GLfloat z);
    void draw();
    
    bool collides(GLfloat x, GLfloat y, GLfloat z);
    bool manage(GLfloat x, GLfloat y, GLfloat z, float deltaTime);
    int getScored();
    private:
    
    GLfloat x, y, z;
    bool taken, removeable;
    float timeCounter;
    int scored;
    
};

#endif
