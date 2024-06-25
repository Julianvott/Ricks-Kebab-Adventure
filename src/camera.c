#include "camera.h"

bool isOutsideSpaceship;

// Perspektive Einstellen
static float defaultFovy = M_PI / 3.0f;
static float povFovy = M_PI / 2.0;
static float targetFovy = M_PI / 2.0;
CameraPerspective cameraPerspective = {
    M_PI / 3.0f,                        // fovy
    (float) (WIDTH) / (float) (HEIGHT), // Seitenverhältnis der Bildschirmauflösung
    1.0f,                               // near plane
    15000.0f                            // far plane
};

// Pov Kameraposition
static Camera presetPOV =  {{352.0f, 117.0f, 357.0f}, {0.0f, 200.0f, 0.0f}, {0.0f, 1.0f, 0.0f}};

// Kamerapositionen
static Camera presets[NUM_PRESETS_SURROUND][NUM_PRESETS_DISTANCE] = {
    {
        {{0.0f, 100.0f, 900.0f}, {0.0f, 200.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.0f, 300.0f, 500.0f}, {0.0f, 200.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.0f, 100.0f, 400.0f}, {0.0f, 130.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}
    },
    {
        {{-550.0f, 100.0f, 550.0f}, {0.0f, 200.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{-350.0f, 300.0f, 350.0f}, {0.0f, 200.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{-300.0f, 100.0f, 300.0f}, {0.0f, 130.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}
    },
    {
        {{-900.0f, 100.0f, 0.0f}, {0.0f, 200.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{-500.0f, 300.0f, 0.0f}, {0.0f, 200.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{-400.0f, 100.0f, 0.0f}, {0.0f, 130.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}
    },
    {
        {{-550.0f, 100.0f, -550.0f}, {0.0f, 200.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{-350.0f, 300.0f, -350.0f}, {0.0f, 200.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{-300.0f, 100.0f, -300.0f}, {0.0f, 130.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}
    },
    {
        {{0.0f, 100.0f, -900.0f}, {0.0f, 200.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.0f, 300.0f, -500.0f}, {0.0f, 200.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.0f, 100.0f, -300.0f}, {0.0f, 130.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}
    },
    {
        {{550.0f, 100.0f, -550.0f}, {0.0f, 200.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{350.0f, 300.0f, -350.0f}, {0.0f, 200.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{300.0f, 100.0f, -300.0f}, {0.0f, 130.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}
    },
    {
        {{900.0f, 100.0f, 0.0f}, {0.0f, 200.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{500.0f, 300.0f, 0.0f}, {0.0f, 200.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{400.0f, 100.0f, 0.0f}, {0.0f, 130.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}
    },
    {
        {{410.0f, 135.0f, 420.0f}, {0.0f, 120.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{350.0f, 300.0f, 350.0f}, {0.0f, 200.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{300.0f, 100.0f, 300.0f}, {0.0f, 130.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}
    },
    
};

// aktuelle Kamera
Camera camera;
// Für Kamera Position Interpolation
static Camera targetCamera;
static Camera interpolationStartCamera;

// Aktuelle und Zielindices der Kamerapositionen
// Wird von der HUD verwendet
int currentPresetSurroundIndex = 0;
int currentPresetDistanceIndex = 0;
int targetPresetSurroundIndex = 0;
int targetPresetDistanceIndex = 0;

// Animationszeit
static float animationTime = 0.0f;
static float animationDuration = 0.5f;

void initCamera() {
    camera = presetPOV;
    targetCamera = presetPOV;
    isOutsideSpaceship = false;
}


void updateCameraFovy(float deltaTime){
    assert(deltaTime >= 0 && "deltaTime should not be negative");

    if (targetFovy == cameraPerspective.fovy){
        return;
    }
    // Berechnung der Differenz und Einstellen des Inkrements
    float increment = 2.0 * deltaTime;
    
    // Erhöhen oder Verringern des 'fovy' basierend auf der Differenz
    if (targetFovy > cameraPerspective.fovy) {
        cameraPerspective.fovy += increment;
        if (cameraPerspective.fovy > targetFovy) { // Korrektur, falls über Zielwert hinaus
            cameraPerspective.fovy = targetFovy;
        }
    } else if (targetFovy < cameraPerspective.fovy) {
        cameraPerspective.fovy -= increment;
        if (cameraPerspective.fovy < targetFovy) { // Korrektur, falls unter Zielwert
            cameraPerspective.fovy = targetFovy;
        }
    }
}

void switchCameraFovy(bool pov){
    if (pov){
        targetFovy = povFovy; 
    } else {
        targetFovy = defaultFovy;
    }
}

void updateCamera(float deltaTime) {
    assert(deltaTime >= 0 && "deltaTime should not be negative");
    
    if (animationTime >= animationDuration) {
        return;
    }
    
    animationTime += deltaTime;
    if (animationTime > animationDuration) {
        animationTime = animationDuration;  // Sicherstellen, dass wir nicht über das Ziel hinausschießen
    }
    float t = animationTime / animationDuration;

    // Interpolation der Position und Blickrichtung
    for (int i = 0; i < 3; i++) {
        camera.eye[i] = (1 - t) * interpolationStartCamera.eye[i] + t * targetCamera.eye[i];
        camera.target[i] = (1 - t) * interpolationStartCamera.target[i] + t * targetCamera.target[i];
        camera.up[i] = (1 - t) * interpolationStartCamera.up[i] + t * targetCamera.up[i];
    }
}

/**
 * @brief Startet die Interpolation der Kamera Position
 * 
 * @param preset 
 */
static void startCameraTransition(Camera preset){
    animationTime = 0.0f;
    targetCamera = preset;
    interpolationStartCamera = camera;
}

void nextSurroundPosition(bool clockwise){
    if (clockwise){
        targetPresetSurroundIndex = (currentPresetSurroundIndex + 1) % NUM_PRESETS_SURROUND;
    } else {
        targetPresetSurroundIndex = (currentPresetSurroundIndex - 1 + NUM_PRESETS_SURROUND) % NUM_PRESETS_SURROUND;
    }
    if (targetPresetSurroundIndex != currentPresetSurroundIndex){
        currentPresetSurroundIndex = targetPresetSurroundIndex;
        startCameraTransition(presets[targetPresetSurroundIndex][targetPresetDistanceIndex]);
    }
}

void nextDistancePosition(bool closer){
    if (closer && currentPresetDistanceIndex < NUM_PRESETS_DISTANCE - 1){
        targetPresetDistanceIndex = currentPresetDistanceIndex + 1;
    } else if (currentPresetDistanceIndex > 0){
        targetPresetDistanceIndex = currentPresetDistanceIndex - 1;
    }
    if (targetPresetDistanceIndex != currentPresetDistanceIndex){
        currentPresetDistanceIndex = targetPresetDistanceIndex;
        startCameraTransition(presets[targetPresetSurroundIndex][targetPresetDistanceIndex]);
    }
}

void setPovCamera(bool animation) {
    if (animation){
        startCameraTransition(presetPOV);
    } else {
        camera = presetPOV;
    }
}