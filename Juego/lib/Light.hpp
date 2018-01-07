///////////////////////////////////////////
/// RAFAEL GONZÁLEZ CARRIZO - DIC. 2017 ///
///////////////////////////////////////////

#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "GL.hpp"

class Light {
    public:
    
    enum Type { Directional, Positional };
    
    Light();
    Light(float x, float y, float z, Type type);
    
    bool isPositional() const;
    bool isDirectional() const;
    
    float getX() const;
    float getY() const;
    float getZ() const;
    
    void setType(Type type);
    void setPosition(float x, float y, float z);
    void setAmbient(float r, float g, float b);
    void setDiffuse(float r, float g, float b);
    void setSpecular(float r, float g, float b);
    void setDirection(float x, float y, float z);
    
    void setCutoff(float cutoff);
    void setExponent(float exponent);
    
    void update(GLenum light);
    void enable(GLenum light);
    
    bool needsUpdate() const;
    void updated();
    
    protected:
    void setDefaultAttrs();

    private:
    GLfloat position[4], ambient[3], diffuse[3], specular[3], direction[3];
    GLfloat cutoff, exponent;
    bool mNeedsUpdate;
};

#endif
