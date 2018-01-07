#include "Light.hpp"

Light::Light() {
    position[0] = .0; position[1] = .0; position[2] = .0; position[3] = .0;
    setDefaultAttrs();
    
}

Light::Light(float x, float y, float z, Type type) {
    setDefaultAttrs();
    setPosition(x, y, z);
    
    position[3] = type == Type::Directional ? .0 : 1.0;
}

void Light::setDefaultAttrs() {    
    mNeedsUpdate = true;
    ambient[0] = .0; ambient[1] = .0; ambient[2] = .0;
    diffuse[0] = .0; diffuse[1] = .0; diffuse[2] = .0;
    specular[0] = .0; specular[1] = .0; specular[2] = .0;
    direction[0] = .0; direction[1] = .0; direction[2] = -1.0;
    cutoff = 180.f;
    exponent = .0;
}

bool Light::isPositional() const {
    return !isDirectional();
}

bool Light::isDirectional() const {
    return position[3] == .0;
}

float Light::getX() const {
    return position[0];
}

float Light::getY() const {
    return position[1];
}

float Light::getZ() const {
    return position[2];
}

void Light::setType(Type type) {
    mNeedsUpdate = true;
    if (type == Type::Directional) {
        position[3] = 0.f;
    }
    else {
        position[3] = 1.0;
    }
}

void Light::setPosition(float x, float y, float z) {
    position[0] = x; position[1] = y; position[2] = z;
    mNeedsUpdate = true;
}

void Light::setAmbient(float r, float g, float b) {
    ambient[0] = r; ambient[1] = g; ambient[2] = b;
    mNeedsUpdate = true;
}

void Light::setDiffuse(float r, float g, float b) {
    diffuse[0] = r; diffuse[1] = g; diffuse[2] = b;
    mNeedsUpdate = true;
}

void Light::setSpecular(float r, float g, float b) {
    specular[0] = r; specular[1] = g; specular[2] = b;
    mNeedsUpdate = true;
}

void Light::setDirection(float x, float y, float z) {
    direction[0] = x; direction[1] = y; direction[2] = z;
    mNeedsUpdate = true;
}

void Light::setCutoff(float cutoff) {
    this->cutoff = cutoff;
    mNeedsUpdate = true;
}

void Light::setExponent(float exponent) {
    this->exponent = exponent;
    mNeedsUpdate = true;
}

void Light::update(GLenum light) {
    glLightfv(light, GL_POSITION, position);
    glLightfv(light, GL_DIFFUSE, diffuse);
    glLightfv(light, GL_SPECULAR, specular);
    glLightfv(light, GL_AMBIENT, ambient);
    
    
    glLightfv(light,GL_SPOT_DIRECTION,direction);
    glLightf(light, GL_SPOT_CUTOFF, cutoff);
    glLightf(light, GL_SPOT_EXPONENT, exponent);
    
    updated();
}

void Light::enable(GLenum light) {
    update(light);
    glEnable(light);
}

bool Light::needsUpdate() const {
    return mNeedsUpdate;
}

void Light::updated() {
    mNeedsUpdate = false;
}
