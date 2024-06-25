#include "scene.h"

// Haupt ShaderProgram
ShaderProgram shaderProgram;

// Objekte in der Scene
Object skyboxObj, maschineObj, doenerObj, spiessObj, feuerObj, 
lampeKabelObj, lampeGlasObject, lampeDrahtObj, knoepfeObj, oilObject, pommesObj,
carBottomObj, carTopObj, carDomeObj, carHeadlightBodyObj, carHeadlightGlassObj, carPillowsObj, carSteeringWheelObj, carStripesObj, carTankStrapObj, carTubesObj,
transitionCircleObject;

// Unterteilung der Objekte in der Scene
#define OPAQUE_OBJECTS_NUM 19
#define TRANSPARENT_OBJECTS_NUM 1
#define REFLECTIVE_OBJECTS_NUM 1
#define SPACESHIP_OBJECTS_NUM 10
Object *opaqueObjects[OPAQUE_OBJECTS_NUM] = {
    &skyboxObj, &maschineObj, &doenerObj, &spiessObj, &feuerObj, &lampeKabelObj, &lampeDrahtObj, &knoepfeObj, &pommesObj,
    &carBottomObj, &carTopObj, &carHeadlightBodyObj, &carHeadlightGlassObj, &carPillowsObj, &carSteeringWheelObj, &carStripesObj, &carTankStrapObj, &carTubesObj,
    &transitionCircleObject
};
Object *transparentObjects[TRANSPARENT_OBJECTS_NUM] = {&carDomeObj}; // ! für den Maleralgorythmus in umgekehrter Zeichenreihenfolge einfügen
Object *reflectiveObjects[REFLECTIVE_OBJECTS_NUM] = {&oilObject}; // skalierbar :)
Object *spaceshipObjects[SPACESHIP_OBJECTS_NUM] = {
    &carBottomObj, &carTopObj, &carDomeObj, &carHeadlightBodyObj, &carPillowsObj, &carSteeringWheelObj, &carStripesObj, &carTankStrapObj, &carTubesObj, &carHeadlightGlassObj
};

// Bildspeicher für den Oil(Wasser)-Effekt
FrameBuffer reflectionFrameBuffer;
FrameBuffer refractionFrameBuffer;

/**
 * @brief Setzt die aktuelle Projektionsmatrix als Uniform im Shader
 * 
 */
static void setProjectionMatrix(){
    GLfloat transform[16];
    identity(transform);

    // Setze die perspektivische Projektionsmatrix
    perspective(
        transform, 
        cameraPerspective.fovy, 
        cameraPerspective.aspect, 
        cameraPerspective.near, 
        cameraPerspective.far
    );
    // Setze 
    glUniformMatrix4fv(shaderProgram.projectionMatrixLocation, 1, GL_FALSE, transform);
}

/**
 * @brief Setzt die aktuelle Sichtmatrix für Uniform im Shader
 * 
 */
static void setViewMatrix() {
    // Erzeuge Sichtmatrix
    GLfloat transform[16];
    identity(transform);
    lookAt(transform, camera.eye, camera.target, camera.up);
    // Setze Sichtmatrx als Uniform
    glUniformMatrix4fv(shaderProgram.viewMatrixLocation, 1, GL_FALSE, transform);
}

/**
 * @brief Setzt die aktuelle Position der Kamera als Uniform im Shader
 * 
 */
static void setCameraPosition() {
    // Setze die Kameraposition als Uniform
    glUniform3fv(shaderProgram.cameraPositionLocation, 1, camera.eye);
}

/**
 * @brief Setzt die aktuelle Reflektions-Sichtmatrix für Uniform im Shader
 * 
 */
static void setReflectionViewMatrix() {
    GLfloat transform[16];
    
    // Spiegele die Kameraposition
    GLfloat distanceAboveReflection = camera.eye[1] - oilObject.position.y;
    GLfloat newEye[3] = {
        camera.eye[0], 
        oilObject.position.y - distanceAboveReflection, 
        camera.eye[2]
    };
    
    // Spiegele den Zielpunkt
    GLfloat targetDistanceAboveReflection = camera.target[1] - oilObject.position.y;
    GLfloat newTarget[3] = {
        camera.target[0], 
        oilObject.position.y - targetDistanceAboveReflection, 
        camera.target[2]
    };
    
    // Invertiere den Up-Vektor
    GLfloat newUP[3] = {camera.up[0], -camera.up[1], camera.up[2]};

    // Erzeuge Sichtmatrix
    identity(transform);
    lookAt(transform, newEye, newTarget, newUP);
    // Setze Sichtmatix als Uniform
    glUniformMatrix4fv(shaderProgram.viewMatrixLocation, 1, GL_FALSE, transform);
}

/**
 * @brief Zeichnet alle opaken und transparenten Objekte
 * 
 */
static void drawOpaqueAndTransparentObjects() {
    // Zeiche opake Objekte
    for (int i = 0; i < OPAQUE_OBJECTS_NUM; i++) {
        drawObject(opaqueObjects[i], &shaderProgram);
    }

    // Sorge dafür, dass Z-Buffer nicht mehr beschreibbar, aber noch lesbar („read-only“):
    glDepthMask(GL_FALSE);
    
    // Zeichne die Forderseite der transparenten Objekte
    glCullFace(GL_FRONT);
    for (int i = 0; i < TRANSPARENT_OBJECTS_NUM; i++) {
        drawObject(transparentObjects[i], &shaderProgram);
    }

    // Zeichne die Rückseite der transprenten Objekte
    glCullFace(GL_BACK);
    for (int i = 0; i < TRANSPARENT_OBJECTS_NUM; i++) {
        drawObject(transparentObjects[i], &shaderProgram);
    }

    // Ermögliche das schreiben in den Z-Buffer
    glDepthMask(GL_TRUE);

}

/**
 * @brief Zeichne die Reflektions-Textur und setze diesen als Uniform
 * 
 */
static void renderReflectionTexture() {
    // Binde den Reflektions-Bildspeicher
    glBindFramebuffer(GL_FRAMEBUFFER, reflectionFrameBuffer.fbo);
    // Leere den Farb- und Tiefenspeicher
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Aktiviere das Clipping, um alle Objektteile basierend auf der Clipping-Ebene zu abzuschneiden
    glEnable(GL_CLIP_DISTANCE0);
    // Definiere die Clipping-Ebene (Normale nach oben)
    const GLfloat clipPlane[4] = {0.0f, 1.0f, 0.0f, -oilObject.position.y};
    // Setze die Clipping-Ebene
    glUniform4fv(shaderProgram.clipPlaneLocation, 1, clipPlane);

    // Setze die Uniforms
    setReflectionViewMatrix();
    setProjectionMatrix();
    setLightUniforms(&shaderProgram);
    setSkyboxAmbient(&shaderProgram);

    // Zeichne die opaken und transparenten Objekte
    drawOpaqueAndTransparentObjects();

    // Deaktiviere das Clipping wieder
    glDisable(GL_CLIP_DISTANCE0);

    // Setze die Kamera zurück
    setViewMatrix();

    // Setze die Gezeichnete Textur als Uniform
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, reflectionFrameBuffer.colorTexture);
    glUniform1i(shaderProgram.reflectionTextureSamplerLocation, 1);

    // Binde zurück zum Stanard-Bildspeicher
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
/**
 * @brief Zeichne die Brechungs-Textur und setze diesen als Uniform
 * 
 */
static void renderRefrationTexture() {
    // Binde den Brechungs-Bildspeicher
    glBindFramebuffer(GL_FRAMEBUFFER, refractionFrameBuffer.fbo);
    // Leere den Farb- und Tiefenspeicher
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Aktiviere das Clipping, um alle Objektteile basierend auf der Clipping-Ebene zu abzuschneiden
    glEnable(GL_CLIP_DISTANCE0);
    // Definiere die Clipping-Ebene (Normale nach unten)
    const GLfloat clipPlane[4] = {0.0f, -1.0f, 0.0f, oilObject.position.y};
    // Setze die Clipping-Ebene
    glUniform4fv(shaderProgram.clipPlaneLocation, 1, clipPlane);

    // Setze die Uniforms
    setViewMatrix();
    setProjectionMatrix();
    setLightUniforms(&shaderProgram);
    setSkyboxAmbient(&shaderProgram);
    drawOpaqueAndTransparentObjects();

    // Deaktiviere das Clipping wieder
    glDisable(GL_CLIP_DISTANCE0);

    // Setze die Gezeichnete Textur als Uniform
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, refractionFrameBuffer.colorTexture);
    glUniform1i(shaderProgram.refractionTextureSamplerLocation, 2);

    // Binde zurück zum Stanard-Bildspeicher
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void initScene() {
    // Erstelle den Shader
    const char *vertexShaderPath = "shaders/basic_vertex_shader.glsl";
    const char *fragmentShaderPath = "shaders/basic_fragment_shader.glsl";
    shaderProgram = createShaderProgram(vertexShaderPath, fragmentShaderPath);
    
    // Initialisiere Kamera
    initCamera();
    // Initialisiere Hud
    initHUD();
    // Initialisiere alle Objekte der Szene
    initDimension(&skyboxObj, &doenerObj);
    // Döner Maschine
    maschineObj      = initObject("models/maschine.obj", metalMaterial, loadTexture("textures/metall.jpg", 0.5f));
    spiessObj        = initObject("models/spiess.obj", metalMaterial, loadTexture("textures/metall.jpg", 1.0f));
    feuerObj         = initObject("models/feuer.obj", heatpadMaterial, loadTexture("textures/feuer.jpg", 0.4f));
    knoepfeObj       = initObject("models/knoepfe.obj",  plasticMaterial, loadTexture("textures/black.jpg", 1.0f));
    oilObject           = initObject("models/oil.obj", oilMaterial, loadTexture("textures/oil.jpg", 1.0f));
    pommesObj        = initObject("models/pommes.obj", foodMaterial, loadTexture("textures/pommes.jpg", 3.0f));
    // Raumschiff Objekte
    carBottomObj        = initObject("models/spaceship/carBottom.obj", plasticMaterial, loadTexture("textures/spaceship/lightgrey.jpg", 1.0f));
    carTopObj           = initObject("models/spaceship/carTop.obj", spaceshipMetal, loadTexture("textures/spaceship/spacegrey.jpg", 1.0f));
    carDomeObj          = initObject("models/spaceship/carDome.obj", clearglassMaterial, loadTexture("textures/spaceship/glass.jpg", 3.0f));
    carHeadlightBodyObj = initObject("models/spaceship/carHeadlightBody.obj", spaceshipMetal, loadTexture("textures/spaceship/forestgreen.jpg", 1.0f));
    carHeadlightGlassObj= initObject("models/spaceship/carHeadlightGlass.obj", mattglassMaterial, loadTexture("textures/spaceship/white.jpg", 1.0f));
    carPillowsObj       = initObject("models/spaceship/carPillows.obj", fabricMaterial, loadTexture("textures/spaceship/brown.jpg", 1.0f));
    carSteeringWheelObj = initObject("models/spaceship/carSteeringWheel.obj", plasticMaterial, loadTexture("textures/spaceship/lightgrey.jpg", 1.0f));
    carStripesObj       = initObject("models/spaceship/carStripes.obj", mattglassMaterial, loadTexture("textures/spaceship/yellow.jpg", 1.0f));
    carTankStrapObj     = initObject("models/spaceship/carTankStrap.obj", spaceshipMetal, loadTexture("textures/spaceship/spacegrey.jpg", 1.0f));
    carTubesObj         = initObject("models/spaceship/carTubes.obj", rubberMaterial, loadTexture("textures/spaceship/darksand.jpg", 1.0f));
    // Reisen-Übergangs Objekt
    transitionCircleObject    = initObject("models/spaceship/transitionCircle.obj", transitionCircleMaterial, loadTexture("textures/spaceship/white.jpg", 1.0f));
    
    // Initialisiere die Animation für das Raumschiff
    initSpaceshipAnimation(spaceshipObjects, SPACESHIP_OBJECTS_NUM, &transitionCircleObject);

    // Initialisiere die Standard Scheinwerfer der Scheinwerfersteuerung
    initDefaultHeadlights();

    // Initialisiere Öl Höhe
    oilObject.position.y = 39.79f;

    // Initialisiere Speicher für den Oil(Wasser)-Effekt
    reflectionFrameBuffer = initFrameBuffer();
    refractionFrameBuffer = initFrameBuffer();
}

void renderScene() {
    // Binde den Standard-Bildspeicher
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // Leere den Farb- und Tiefenspeicher
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Verwende das Shaderprogramm
    glUseProgram(shaderProgram.program);
    
    // Setze Uniforms
    setViewMatrix();
    setCameraPosition();
    setProjectionMatrix();
    setLightUniforms(&shaderProgram);
    setSkyboxAmbient(&shaderProgram);

    // Zeichne die Reflektions- und Brechungstextur, setze sie als Uniform
    renderReflectionTexture();
    renderRefrationTexture();
    
    // Zeichne alle opaken und transparenten Objekte
    drawOpaqueAndTransparentObjects();

    // Zeichne das Oil-Objekt
    glUniform1i(glGetUniformLocation(shaderProgram.program, "isReflective"), true);
    drawObject(&oilObject, &shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram.program, "isReflective"), false);

    // Zeichne das HUD
    drawHUD();
}

void updateScene(float deltaTime) {
    assert(deltaTime >= 0 && "deltaTime should not be negative");
    
    // Aktualisiere dynamische Elemente der Szene
    updateCamera(deltaTime);
    updateCameraFovy(deltaTime);
    updateSpaceshipTransition(&transitionCircleObject, deltaTime);
    updateSpaceshipMovement(spaceshipObjects, SPACESHIP_OBJECTS_NUM, deltaTime);
    updateDimension(deltaTime);

    // Rotiere den Döner
    spiessObj.rotation.y = (float)glfwGetTime() * 1.0f; // 1 Grad pro Sekunde um die Y-Achse rotieren
    doenerObj.rotation.y = (float)glfwGetTime() * 1.0f; // 1 Grad pro Sekunde um die Y-Achse rotiere
}

void deleteScene() {
    for (int i = 0; i < OPAQUE_OBJECTS_NUM; i++) { deleteObject(opaqueObjects[i]); }
    for (int i = 0; i < TRANSPARENT_OBJECTS_NUM; i++) { deleteObject(transparentObjects[i]); }
    for (int i = 0; i < REFLECTIVE_OBJECTS_NUM; i++) { deleteObject(reflectiveObjects[i]); }
    deleteHUD();
    deleteShaderProgram(&shaderProgram);
    deleteFrameBuffer(&reflectionFrameBuffer);
    deleteFrameBuffer(&refractionFrameBuffer);
}