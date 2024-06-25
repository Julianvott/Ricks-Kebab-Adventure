#ifndef MATERIAL_H
#define MATERIAL_H

#include <GL/glew.h>


typedef struct {
    GLfloat emission[3];
    GLfloat ambient[3];
    GLfloat diffuse[3];
    GLfloat specular[3];
    GLfloat shininess;
    GLfloat alpha;
} Material;

extern Material metalMaterial;
extern Material spaceshipMetal;
extern Material foodMaterial;
extern Material heatpadMaterial;
extern Material clearglassMaterial;
extern Material mattglassMaterial;
extern Material plasticMaterial;
extern Material oilMaterial;
extern Material skyboxMaterial;
extern Material fabricMaterial;
extern Material rubberMaterial;
extern Material transitionCircleMaterial;

#endif // MATERIAL_H
