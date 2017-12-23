#include "Point.hpp"
#include <cmath>

#include <iostream>

Point::Point(GLfloat x, GLfloat y, GLfloat z) {
    this->x = x; this->y = y; this->z = z;
    taken = removeable = false;
    timeCounter = 0.f;
}

void Point::draw() {
    if (removeable) return;
    
    glPushMatrix();
    /*GLfloat matSpecular[] = { 1.0, 0.3, 0.3, 0.3 };
    GLfloat matDiffuse[] = { 0.8, 0.8, 0.8 };
    GLfloat matShininess[] = { 3.0 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);*/
    glColor3f(0.8, 0.8, 0.1);
    glTranslatef(x, y, z);
    glScalef(2 + sin(timeCounter), 2 +sin(timeCounter), 2 + sin(timeCounter));
    glutSolidSphere(0.3, 5, 5);
    glPopMatrix();
}

bool Point::collides(GLfloat x, GLfloat y, GLfloat z) {
    
    return sqrt((this->x - x) * (this->x - x) + (this->y - y) * (this->y - y)) <= 1.0 && std::abs(this->z - z) < 3.5;
    
}

bool Point::manage(GLfloat x, GLfloat y, GLfloat z, float deltaTime) {
    if (removeable) return false;
    
    timeCounter += deltaTime;
    
    bool res = false;
    if (collides(x, y, z) && !taken) {
        scored = (1 - sqrt((this->x - x) * (this->x - x) + (this->y - y) * (this->y - y))) * 1000;
        taken = res = true;
    }
    
    if (taken) {
        this->y += 3 * deltaTime;
        if (this->y > 4) { removeable = true; }
    }
    
    return res;
}

int Point::getScored() {
    return scored;
}
