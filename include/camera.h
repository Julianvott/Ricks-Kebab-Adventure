#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <assert.h>
#include <stdbool.h>
#include "shader.h"
#include "matrix.h"
#include "global.h"
#include "headlightControl.h"


// Anzahl der Kamera-Presets
#define NUM_PRESETS_SURROUND 8
#define NUM_PRESETS_DISTANCE 3

typedef struct {
    GLfloat eye[3];
    GLfloat target[3];
    GLfloat up[3];
} Camera;

typedef struct {
    GLfloat fovy;
    GLfloat aspect; 
    GLfloat near;
    GLfloat far;
} CameraPerspective;

extern Camera camera;
extern CameraPerspective cameraPerspective;

extern int targetPresetSurroundIndex;
extern int targetPresetDistanceIndex;

/**
 * @brief Initialisiert die Kameraposition
 * 
 */
void initCamera();

/**
 * @brief Aktualisiert die Kamera fovy
 * 
 * @param deltaTime Zeit in Sekunden seit dem letzten Update
 *
 */
void updateCameraFovy(float deltaTime);

/**
 * @brief wechselt die Kamera fovy
 * 
 * @param pov wechselt zur spaceship pov Ansicht wenn true
 */
void switchCameraFovy(bool pov);

/**
 * @brief Aktualisiert die Kameraposition durch Interpolation zwischen Anfangs- und Zielzustand über die Animationsdauer.
 *
 * Beendet die Aktualisierung, wenn die gesamte Animationsdauer erreicht ist, um Überläufe zu vermeiden.
 *
 * @param deltaTime Zeit in Sekunden seit dem letzten Update, beeinflusst die Interpolationsrate.
 */
void updateCamera(float deltaTime);

/**
 * @brief Wechselt die Position um den Döner herum im oder gegen den Uhrzeigersinn.
 * 
 * @param clockwise clockwise Bestimmt die Drehrichtung: true für Uhrzeigersinn, false für gegen den Uhrzeigersinn.
 */
void nextSurroundPosition(bool clockwise);

/**
 * @brief Wechselt die Entfernungsposition der Kamera näher oder weiter weg, basierend auf der Eingabe. 
 * 
 * @param closer Bestimmt die Änderungsrichtung der Entfernung: true für näher zur Position, false für weiter weg.
 */
void nextDistancePosition(bool closer);

/**
 * @brief Setzt die Kamera auf POV
 * 
 *  @param animation Wenn true wird der Übergang interpoliert
 */
void setPovCamera(bool animation);

#endif // CAMERA_H