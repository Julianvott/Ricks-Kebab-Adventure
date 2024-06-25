#ifndef HUD_H
#define HUD_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include "shader.h"
#include "camera.h"
#include "global.h"


#define CIRCLE_SEGMENTS 100

typedef struct {
    ShaderProgram shaderProgram;
    GLuint VAO, VBO, EBO;  // OpenGL Objekte für das gesamte HUD
} HUD;

/**
 * @brief Initialisiert die HUD
 * 
 */
void initHUD();

/**
 * @brief Zeichnet die HUD
 * 
 */
void drawHUD();

/**
 * @brief Löscht HUD und gibt Ressourcen frei
 * 
 */
void deleteHUD();

/**
 * @brief Aktiviert die HUD
 * 
 */
void enableHud();  

/**
 * @brief Deaktiviert die HUD
 * 
 */
void disableHud();  

#endif // HUD_H
