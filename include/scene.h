#ifndef SCENE_H
#define SCENE_H

#include "global.h"
#include "camera.h"
#include "light.h"
#include "matrix.h"
#include "mesh.h"
#include "object.h"
#include "shader.h"
#include "texture.h"
#include "material.h"
#include "utils.h"
#include "hud.h"
#include "framebuffer.h"
#include "light.h"
#include "dimension.h"
#include "spaceshipAnimation.h"
#include "headlightControl.h"


/**
 * @brief Initialisiert die Szene: Shader, Camera, Hud, Objekte, Licht, Bildspeicher
 * 
 */
void initScene();

/**
 * @brief Rendert die komplette Szene
 * 
 */
void renderScene();

/**
 * @brief Aktualisiere die Daten der Szene
 * 
 * @param deltaTime Zeitspanne, die seit dem letzten Aufruf vergangen ist
 */
void updateScene(float deltaTime);

/**
 * @brief Löscht die verwendeten Ressourcen der Szene
 * 
 */
void deleteScene();

#endif // SCENE_H