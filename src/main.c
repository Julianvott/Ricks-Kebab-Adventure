#include "scene.h"
#include "light.h"
#include "matrixTest.h"

void setupOpenGLDefaults() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW); // Counter-Clockwise
    //Transparenz aktivieren
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glViewport(0, 0, WIDTH, HEIGHT);
}

// Callback-Funktion für die Größenänderung des Framebuffers
void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (spaceshipAnimationRunning){
            return;
        }

        if (!isOutsideSpaceship){
            if (key == GLFW_KEY_SPACE) {
            startSpaceship();
            }
            // Headlight Control
            else if (key == GLFW_KEY_Q) {
                enlargeSpotlight();
            }
            else if (key == GLFW_KEY_E) {
                shrinkSpotlight();
            }
            else if (key == GLFW_KEY_F) {
                offOnHeadlights();
            }
            else if (key == GLFW_KEY_A) {
                sharpenHeadlightsFocus();
            }
            else if (key == GLFW_KEY_D) {
                softenHeadlightsFocus();
            }
            else if (key == GLFW_KEY_R) {
                resetHeadlights();
            }
            else if (key == GLFW_KEY_W) {
                raiseHeadlights();
            }
            else if (key == GLFW_KEY_S) {
                lowerHeadlights();
            }
        }
        
        // Kamera
        if (key == GLFW_KEY_L) {
            setPovCamera(true);
            switchCameraFovy(true);
            turnOnHeadlights();
            isOutsideSpaceship = false;
        }
        else if (key == GLFW_KEY_RIGHT) {
            nextSurroundPosition(false);
            isOutsideSpaceship = true;
        }
        else if (key == GLFW_KEY_LEFT) {
            nextSurroundPosition(true);
            isOutsideSpaceship = true;
        }
        else if (key == GLFW_KEY_UP) {
            nextDistancePosition(true);
            isOutsideSpaceship = true;
        }
        else if (key == GLFW_KEY_DOWN) {
            nextDistancePosition(false);
            isOutsideSpaceship = true;
        }

        if (isOutsideSpaceship){
            turnOffHeadlights();
            switchCameraFovy(false);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-test") == 0) {  
                printf("Tests werden ausgeführt\n");
                executeTests();
            }
        }
    } else {
        printf("Um einen Test auszufuehren benutze Parameter '-test'\n");
    }

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Rick's Kebab Adventure", NULL, NULL);
    if (!window) {
        printf("Fenster konnte nicht erstellt werden");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("GLEW konnte nicht initialisiert werden");
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);

    setupOpenGLDefaults();
    initScene();

    // Hauptschleife
    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        float deltaTime = (float)(currentTime - lastTime);
        lastTime = currentTime;

        glfwPollEvents();
        updateScene(deltaTime);
        renderScene();
        glfwSwapBuffers(window);
    }

    deleteScene();
    glfwTerminate();
    return 0;
}