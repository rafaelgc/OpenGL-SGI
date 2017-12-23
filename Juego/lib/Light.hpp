#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "GL.hpp"

class Light {
    public:
    
    enum Type { Directional, Positional };
    
    Light();
    Light(float x, float y, float z, Type type);
    
    bool isPositional();
    bool isDirectional();
    
    float getX();
    float getY();
    float getZ();
    
    void setType(Type type);
    void setPosition(float x, float y, float z);
    void setAmbient(float r, float g, float b);
    void setDiffuse(float r, float g, float b);
    void setSpecular(float r, float g, float b);
    void setDirection(float x, float y, float z);
    
    void setCutoff(float cutoff);
    void setExponent(float exponent);
    
    void enable(GLenum light);
    
    protected:
    void setDefaultAttrs();

    private:
    GLfloat position[4], ambient[3], diffuse[3], specular[3], direction[3];
    GLfloat cutoff, exponent;
};

#endif
