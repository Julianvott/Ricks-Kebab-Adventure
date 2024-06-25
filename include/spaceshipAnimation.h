#ifndef MOVE_CONTROL_H
#define MOVE_CONTROL_H

// Anzahl der Kamera-Presets
#define ACCELERATION 200
#define MAX_VELOCITY 250

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include "object.h"
#include "light.h"
#include "camera.h"
#include "hud.h"
#include "dimension.h"


extern bool spaceshipAnimationRunning;

/**
 * @brief Setzt Die Ausgansposition des Spaceship und des Reisen-Übergangs Objekts
 * 
 * @param spaceshipObjects Array der Spaceship Objekte
 * @param spaceshipObjectsNum Anzahl der Spaceship Objekte
 * @param transitionCircleObject Pointer auf das Animation Übergangs Objekt
 */
void initSpaceshipAnimation(Object *spaceshipObjects[], int spaceshipObjectsNum, Object *transitionCircleObject);

/**
 * @brief Startet die spaceship Animation inklusive Übergangseffekt
 * 
 */
void startSpaceship();

/**
 * @brief Aktualisiert den Reise-Übergangs Effekt
 * 
 * @param transitionObject Reise-Übergangs Objekt
 * @param deltaTime Zeit in Sekunden seit dem letzten Update
 */
void updateSpaceshipTransition(Object *transitionObject, float deltaTime);

/**
 * @brief Aktualisiert die spaceship Bewegung
 * 
 * @param spaceshipObjects Array der spaceship Objekte
 * @param spaceshipObjectsNum Anzahl der spaceship Objekte
 * @param deltaTime Zeit in Sekunden seit dem letzten Update
 */
void updateSpaceshipMovement(Object *spaceshipObjects[], int spaceshipObjectsNum, float deltaTime);

#endif // MOVE_CONTROL_H