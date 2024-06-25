#ifndef LIGHT_H
#define LIGHT_H
    
#include <math.h>
#include <GL/glew.h>
#include "global.h"
#include "shader.h"


typedef struct {
    GLfloat direction[3];
    GLfloat cutOff;
    GLfloat outerCutOff;

    GLfloat position[3];
    GLfloat ambient[3];
    GLfloat diffuse[3];
    GLfloat specular[3];
} Light;

extern Light lights[NUM_LIGHTS];

/**
 * @brief Setzt die Light Uniform Werte
 * 
 * @param shaderProgram 
 */
void setLightUniforms(ShaderProgram *shaderProgram);

#endif