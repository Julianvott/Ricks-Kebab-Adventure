#include "spaceshipAnimation.h"

static Light *leftHeadlight = &lights[0];
static Light *rightHeadlight = &lights[1];
static Vector3 spaceshipPos;
static bool spaceshipForward;
static bool transitionRunning;
static bool transitionAnimationPhase2;
bool spaceshipAnimationRunning;


void initSpaceshipAnimation(Object *spaceshipObjects[], int spaceshipObjectsNum, Object *transitionCircleObject) {
    // Spaceship Position
    spaceshipPos                = (Vector3){356.0f, 107.0f, 356.0f};
    Vector3 spaceshipRotation   = (Vector3){0.0f, M_PI * -(3.0f/4.0f), 0.0f};
    for (int i = 0; i < spaceshipObjectsNum; i++) {
        spaceshipObjects[i]->position = spaceshipPos;
        spaceshipObjects[i]->rotation = spaceshipRotation;
    }

    // Transition Circle Position
    transitionCircleObject->rotation.x = M_PI / 2;
    transitionCircleObject->rotation.y = M_PI / 4;
    transitionCircleObject->position.x = 100;
    transitionCircleObject->position.y = 200;
    transitionCircleObject->position.z = 50;
    transitionCircleObject->scale = 0.0f;
}

/**
 * @brief Setzt die Position des Spaceships zurück
 * 
 * @param spaceshipObjects Array der spaceship Objekte
 * @param spaceshipObjectsNum Anzahl der spaceship Objekte
 */
static void resetSpaceshipPositions(Object *spaceshipObjects[],  int spaceshipObjectsNum){
    printf("reset");
    for (int i=0; i<spaceshipObjectsNum; i++){
        spaceshipObjects[i]->position = spaceshipPos;
    }

    resetHeadlights();
    setPovCamera(false);
}

/**
 * @brief Startet den Reise-Übergang Effekt
 * 
 */
static void startSpaceshipTransition(){
    transitionRunning = true;
    disableHud();
}

void updateSpaceshipTransition(Object *transitionObject, float deltaTime){
    assert(deltaTime >= 0 && "deltaTime should not be negative");
    if (!transitionRunning){
        return;
    } 

    
    if(transitionObject->scale > 1.0f){
        transitionAnimationPhase2 = true;
        nextDimension();
    }
    
    // Phase 1 der Transition, der kreis wird größer
    if(!transitionAnimationPhase2){
        transitionObject->scale += 1.7 * deltaTime;
        return;
    // Phase 2 der Transition, der kreis schrumpft
    } else {
        transitionObject->scale -= 1.7 * deltaTime;
    }
    

    if (transitionObject->scale <= 0.0f){ // Animation beendet
        transitionObject->scale = 0.0f;
        transitionRunning = false;
        transitionAnimationPhase2 = false;
        spaceshipAnimationRunning = false;
        enableHud();
    }
}

void startSpaceship(){
    if (!isOutsideSpaceship){
        spaceshipAnimationRunning = true;
        spaceshipForward = true;
    }
}

void updateSpaceshipMovement(Object *spaceshipObjects[], int spaceshipObjectsNum, float deltaTime) {
    assert(deltaTime >= 0 && "deltaTime should not be negative");
    
    static float velocityX = 0;
    static float velocityZ = 0;

    if (!spaceshipForward){
        return;
    }

    if (velocityX >= MAX_VELOCITY/2) {
        startSpaceshipTransition();
    }
    if (velocityX >= MAX_VELOCITY){
        velocityX = 0;
        velocityZ = 0;
        spaceshipForward = false;
        resetSpaceshipPositions(spaceshipObjects, spaceshipObjectsNum);
        return;
    }

    velocityX += ACCELERATION * deltaTime;
    velocityZ += ACCELERATION * deltaTime;
    
    // Spaceship Veschiebung
    for (int i = 0; i < spaceshipObjectsNum; i++) {
        // Update Position basierend auf Geschwindigkeit
        spaceshipObjects[i]->position.x -= velocityX * deltaTime;
        spaceshipObjects[i]->position.z -= velocityZ * deltaTime;
    }

    // Lichter Verschiebung
    leftHeadlight->position[0] -= velocityX * deltaTime;
    leftHeadlight->position[2] -= velocityZ * deltaTime;
    rightHeadlight->position[0] -= velocityX * deltaTime;
    rightHeadlight->position[2] -= velocityZ * deltaTime;

    // Kamera Verschiebung
    camera.eye[0] -= (GLfloat) (velocityX * deltaTime);
    camera.eye[2] -= (GLfloat) (velocityZ * deltaTime);

    // Kamera Blickrichtung
    camera.target[0] -= (GLfloat) (velocityX * deltaTime);
    camera.target[2] -= (GLfloat) (velocityZ * deltaTime);
    
    // Kamera Blickrichtung
    //camera.target[0] -= 0.1f;
    //camera.target[1] -= 0.3f;
    //camera.target[2] -= 0.1f;
    
}