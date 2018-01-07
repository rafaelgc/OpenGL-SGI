#ifndef POINT_HPP
#define POINT_HPP

#include "lib/GL.hpp"

class Point {
    
    public:
    
    Point(GLfloat x, GLfloat y, GLfloat z);
    void draw(bool wired = false);
    
    bool collides(GLfloat x, GLfloat y, GLfloat z);
    bool manage(GLfloat x, GLfloat y, GLfloat z, float deltaTime);
    int getScored();
    void setX(GLfloat x);
    void setY(GLfloat y);
    void setZ(GLfloat z);
        
    bool isRemovable();
    
    private:
    
    GLfloat originalX, originalY, originalZ;
    GLfloat x, y, z;
    bool taken, removable;
    float timeCounter, spawnCounter;
    int scored;
    
};

#endif
