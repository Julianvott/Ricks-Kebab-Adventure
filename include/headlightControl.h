#ifndef HEADLIGHT_CONTROL_H
#define HEADLIGHT_CONTROL_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <global.h>
#include "light.h"


/**
 * @brief Initialisiert die Standardeinstellungen der Scheinwerfer
 * 
 */
void initDefaultHeadlights();

/**
 * @brief Setzt die Scheinwerfer auf die Standardeinstellung zurück
 * 
 */
void resetHeadlights();

/**
 * @brief Aktiviert die Scheinwerfer und setzt die vorherigen Einstellungen
 * 
 */
void turnOnHeadlights();

/**
 * @brief Deaktiviert die Scheinwerfer und speichert die vorherigen Einstellungen
 * 
 */
void turnOffHeadlights();

/**
 * @brief Wechselt den An-Zustand der Scheinwerfer unter Verwendung von: turnOnHeadlights(), turnOffHeadlights()
 * 
 */
void offOnHeadlights();

/**
 * @brief Vergrößert den Winkel der Scheinwerfer
 * 
 */
void enlargeSpotlight();

/**
 * @brief Verkleinert den Winkel der Scheinwerfer
 * 
 */
void shrinkSpotlight();

/**
 * @brief Schärft den Fokus der Scheinwerfer
 * 
 */
void sharpenHeadlightsFocus();

/**
 * @brief Weitet den Fokus der Scheinwerfer
 * 
 */
void softenHeadlightsFocus();

/**
 * @brief Hebt die Scheinwerfer an bis zu einem Maximum
 * 
 */
void raiseHeadlights();

/**
 * @brief Senkt die Scheinwerfer bis zu einem Minimum
 * 
 */
void lowerHeadlights();

#endif // HEADLIGHT_CONTROL_H
