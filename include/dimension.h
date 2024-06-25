#ifndef DIMENSION_H
#define DIMENSION_H  // Korrektur hier

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>
#include "object.h"
#include "global.h"


#define DIMENSIONS_NUM 5

typedef struct {
    GLfloat rgb[3];
} GLfloat3;


/**
 * @brief Initialisiert die Dimensionen mit ihren Skyboxen
 * 
 * @param skyboxObj  Das Objekt worauf die Skybox Textur gemapped wird
 * @param doenerObj  Das Döner Objekt worauf die verschiedenen Texturen je Dimension gemapped werden
 */
void initDimension(Object *skyboxObj, Object *doener);

/**
 * @brief Wechselt zyklisch zur nächsten Dimension
 * 
 */
void nextDimension();

/**
 * @brief Aktualisiert den Dimensions Übergang. Weicher übergang der ambienten Komponente.
 * 
 * @param deltaTime Zeit in Sekunden seit dem letzten Update
 */
void updateDimension(float deltaTime);

/**
 * @brief Setzt die Ambiente Komponente der Dimension in die Uniform
 * 
 * @param shader Shaderprogram zum Setzen der ambienten Komponente
 */
void setSkyboxAmbient(ShaderProgram *shader);

#endif // DIMENSION_H