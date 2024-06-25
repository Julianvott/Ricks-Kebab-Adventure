#include "dimension.h"

// Definitionen der globalen Variablen
static Texture dimensionSkyboxTexture[DIMENSIONS_NUM];
static GLfloat3 dimensionSkyboxAmbient[DIMENSIONS_NUM];
static Texture dimensionDoenerTexture[DIMENSIONS_NUM];

static Object *skyboxObjPointer;
static Object *doenerObjPointer;

static GLfloat3 currentDimensionSkyboxAmbient;
static GLfloat3 targetDimensionSkyboxAmbient;

static int currentDimensionIndex = 0;
static bool transitionRunning;
static float transitionTime = 0.0f;
static float transitionDuration = 4.0f;


void initDimension(Object *skyboxObj, Object *doenerObj) {

    dimensionSkyboxTexture[0] = loadTexture("textures/dimension/skybox/Starfield 5 - 1024x1024.png", 6.0f);
    dimensionSkyboxAmbient[0] = (GLfloat3) {{0.1, 0.1, 0.1}};
    dimensionDoenerTexture[0] = loadTexture("textures/dimension/doener/default doener.jpg", 3.0f);
    
    dimensionSkyboxTexture[1] = loadTexture("textures/dimension/skybox/Blue Nebula 2 - 1024x1024.png", 6.0f);
    dimensionSkyboxAmbient[1] = (GLfloat3) {{0.0, 0.0, 0.5}};
    dimensionDoenerTexture[1] = loadTexture("textures/dimension/doener/steel doener.jpg", 4.0f);
    
    dimensionSkyboxTexture[2] = loadTexture("textures/dimension/skybox/Purple Nebula 2 - 1024x1024.png", 6.0f);
    dimensionSkyboxAmbient[2] = (GLfloat3) {{1.0, 0.0, 1.0}};
    dimensionDoenerTexture[2] = loadTexture("textures/dimension/doener/tomato doener.jpg", 6.0f);

    dimensionSkyboxTexture[3] = loadTexture("textures/dimension/skybox/Blue Nebula 1 - 1024x1024.png", 6.0f);
    dimensionSkyboxAmbient[3] = (GLfloat3) {{0.0, 0.3, 1.0}};
    dimensionDoenerTexture[3] = loadTexture("textures/dimension/doener/pickel doener.jpg", 6.0f);

    dimensionSkyboxTexture[4] = loadTexture("textures/dimension/skybox/Purple Nebula 5 - 1024x1024.png", 6.0f);
    dimensionSkyboxAmbient[4] = (GLfloat3) {{1.0, 0.0, 1.0}};
    dimensionDoenerTexture[4] = loadTexture("textures/dimension/doener/orange doener.jpg", 6.0f);

    *skyboxObj = initObject("models/skybox.obj", skyboxMaterial, dimensionSkyboxTexture[0]);
    *doenerObj = initObject("models/doener.obj", foodMaterial, dimensionDoenerTexture[0]);
    skyboxObjPointer = skyboxObj;
    doenerObjPointer = doenerObj;
}

static int counter = 0;
void nextDimension() {
    currentDimensionSkyboxAmbient = dimensionSkyboxAmbient[currentDimensionIndex];
    currentDimensionIndex = ++counter  % DIMENSIONS_NUM;
    targetDimensionSkyboxAmbient = dimensionSkyboxAmbient[currentDimensionIndex];
    skyboxObjPointer->texture = dimensionSkyboxTexture[currentDimensionIndex];
    doenerObjPointer->texture = dimensionDoenerTexture[currentDimensionIndex];

    transitionTime = 0.0f; // zurücksetzen
    transitionRunning = true;
}

void updateDimension(float deltaTime) {
    assert(deltaTime >= 0 && "deltaTime should not be negative");
    
    if(!transitionRunning){
        return;
    }

    transitionTime += deltaTime;
    float t = transitionTime / transitionDuration;
    // Interpolation 
    for (int i = 0; i < 3; i++) {
        currentDimensionSkyboxAmbient.rgb[i] = (1 - t) * currentDimensionSkyboxAmbient.rgb[i] + t * targetDimensionSkyboxAmbient.rgb[i];
    }

    if (transitionTime >= transitionDuration){
       transitionRunning = false;
    }
}

void setSkyboxAmbient(ShaderProgram *shader) {
    glUniform3fv(shader->skyboxAmbientLocation, 1, currentDimensionSkyboxAmbient.rgb);
}