#include "headlightControl.h"

// Zeigerkopien auf die Scheinwerfer
static Light *leftHeadlight = &lights[0];
static Light *rightHeadlight = &lights[1];
// Speicheren die Standardeinstellungen der Scheinwerfer
static Light defaultLeftHeadlight;
static Light defaultRightHeadlight;
// Speichert die vorherige Einstellung der Scheinwerfer (nützlich für Licht aus/an)
static Light prevLeftHeadlight;
static Light prevRightHeadlight;

// Speichert den Zustand der Scheinwerfer
static bool isOff; // Darf nicht initial auf off gesetzt werden

// Maximum und Minimum der Scheinwerferneigung
static float maxHeadlightAngle = 0.4f;
static float minHeadlightAngle = -0.2f;

void initDefaultHeadlights() {
    defaultLeftHeadlight = *leftHeadlight;
    defaultRightHeadlight = *rightHeadlight;
}

void resetHeadlights() {
    *leftHeadlight = defaultLeftHeadlight;
    *rightHeadlight = defaultRightHeadlight;
}

void turnOffHeadlights() {
    // Guard, wenn das Licht schon aus ist
    if (isOff) { return; }

    prevLeftHeadlight = *leftHeadlight;
    prevRightHeadlight = *rightHeadlight;
    
    // Setzt die Lichteigenschaften auf 0, um das Licht auszuschalten
    for (int i = 0; i < 3; i++) {
        leftHeadlight->ambient[i] = 0.0f;
        leftHeadlight->diffuse[i] = 0.0f;
        leftHeadlight->specular[i] = 0.0f;
        rightHeadlight->ambient[i] = 0.0f;
        rightHeadlight->diffuse[i] = 0.0f;
        rightHeadlight->specular[i] = 0.0f;
    }
    isOff = true;
}

void turnOnHeadlights() {
    // Guard, wenn das Licht schon an ist
    if (!isOff) { return; }

    // Stellt die vorherigen Lichteigenschaften wieder her, um das Licht einzuschalten
    for (int i = 0; i < 3; i++) {
        leftHeadlight->ambient[i] = prevLeftHeadlight.ambient[i];
        leftHeadlight->diffuse[i] = prevLeftHeadlight.diffuse[i];
        leftHeadlight->specular[i] = prevLeftHeadlight.specular[i];
        rightHeadlight->ambient[i] = prevRightHeadlight.ambient[i];
        rightHeadlight->diffuse[i] = prevRightHeadlight.diffuse[i];
        rightHeadlight->specular[i] = prevRightHeadlight.specular[i];
    }
    isOff = false;
}

void offOnHeadlights() {
    // Schaltet das Licht ein oder aus, basierend auf dem aktuellen Zustand
    isOff ? turnOnHeadlights() : turnOffHeadlights();
}

void enlargeSpotlight() {
    float stepSize = 0.004f;
    // Vergrößert den Öffnungswinkel des Spotlights
    leftHeadlight->cutOff -= stepSize;
    leftHeadlight->outerCutOff -= stepSize;
    rightHeadlight->cutOff -= stepSize;
    rightHeadlight->outerCutOff -= stepSize; 
}

void shrinkSpotlight() {
    float step = 0.004f;
    // Verkleinert den Öffnungswinkel des Spotlights
    leftHeadlight->cutOff += step;
    leftHeadlight->outerCutOff += step;
    rightHeadlight->cutOff += step;
    rightHeadlight->outerCutOff += step; 
}

void sharpenHeadlightsFocus() {
    float step = 0.002f;
    // Schärft den Fokus der Scheinwerfer, indem der innere Öffnungswinkel verkleinert wird
    if ((leftHeadlight->cutOff - step * 2) >= leftHeadlight->outerCutOff) {
        leftHeadlight->cutOff -= step;
    }
    if ((rightHeadlight->cutOff - step * 2) >= rightHeadlight->outerCutOff) {
        rightHeadlight->cutOff -= step;
    }
}

void softenHeadlightsFocus() {
    float step = 0.002f;
    // Weitet den Fokus der Scheinwerfer auf, indem der innere Öffnungswinkel vergrößert wird
    leftHeadlight->cutOff += step;
    rightHeadlight->cutOff += step;
}

void raiseHeadlights() {
    float step = 0.03f;
    if (leftHeadlight->direction[1] + step > maxHeadlightAngle) { return; }
    if (rightHeadlight->direction[1] + step > maxHeadlightAngle) { return; }
    leftHeadlight->direction[1] += step;
    rightHeadlight->direction[1] += step;
}

void lowerHeadlights() {
    float step = 0.03f;
    if (leftHeadlight->direction[1] - step < minHeadlightAngle) { return; }
    if (rightHeadlight->direction[1] - step < minHeadlightAngle) { return; }
    leftHeadlight->direction[1] -= step;
    rightHeadlight->direction[1] -= step;
}