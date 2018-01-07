#include "Point.hpp"
#include <cmath>

#include <iostream>

Point::Point(GLfloat x, GLfloat y, GLfloat z) {
    originalX = x; originalY = y; originalZ = z;
    this->x = x; this->y = y; this->z = z;
    taken = removable = false;
    timeCounter = 0.f;
}

void Point::draw(bool wired) {
    if (removable) return;
    
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(2 * ((2 + sin(timeCounter)) / 3), 2 * ((2 + sin(timeCounter)) / 3), 2 * ((2 + sin(timeCounter)) / 3));
    
    if (wired) {
        glColor3f(1.0, 1.0, 1.0);
        glutWireSphere(0.3, 15, 15);
    }
    else {
        glColor3f(0.8, 0.8, 0.1);
        glutSolidSphere(0.3, 15, 15);
    }
    
    glPopMatrix();
}

bool Point::collides(GLfloat x, GLfloat y, GLfloat z) {
    
    return sqrt((this->x - x) * (this->x - x) + (this->y - y) * (this->y - y)) <= 1.0 && std::abs(this->z - z) < 3.5;
    
}

bool Point::manage(GLfloat x, GLfloat y, GLfloat z, float deltaTime) {
    spawnCounter += deltaTime;
    if (removable && spawnCounter > 5) {
        removable = false;
        
        this->x = originalX + (rand() % 3) - 1;
        this->y = originalY;
        this->z = originalZ;
        
        if (collides(x, y, z)) {
            removable = true;
            spawnCounter = 0;
        }
        
        timeCounter = 0;
        taken = false;
    }
    
    //
    if (removable) return false;
    
    timeCounter += deltaTime * 4;
    
    bool res = false;
    if (collides(x, y, z) && !taken) {
        scored = (1 - sqrt((this->x - x) * (this->x - x) + (this->y - y) * (this->y - y))) * 1000;
        taken = res = true;
    }
    
    if (taken) {
        this->y += 3 * deltaTime;
        if (this->y > 4) { removable = true; spawnCounter = 0.f; }
    }
    
    return res;
}

int Point::getScored() {
    return scored;
}

void Point::setX(GLfloat x) {
    this->x = x;
}

void Point::setY(GLfloat y) {
    this->y = y;
}

void Point::setZ(GLfloat z) {
    this->z = z;
}
