#include "light.h"

// Initialisierung des Licht-Arrays
Light lights[NUM_LIGHTS] = {
    { // spaceship headlight left
        {-1.0f, 0.0f, -1.0f},               // Direction
        0.98f,                               // Cut Off
        0.97f,                               // Outer Cut Off

        {350.0f, 109.0, 330.0f},      // Position
        {0.2f, 0.2f, 0.2f},                  // Ambient
        {0.2f, 0.2f, 0.2f},                  // Diffuse
        {0.2f, 0.2f, 0.2f}                   // Specular
    },
    { // spaceship headlight right
        {-1.0f, 0.0f, -1.0f},              // Direction
        0.98f,                              // Cut Off
        0.97f,                              // Outer Cut Off

        {330.0f, 109.0f, 350.0},     // Position
        {0.2f, 0.2f, 0.2f},                 // Ambient
        {0.2f, 0.2f, 0.2f},                 // Diffuse
        {0.2f, 0.2f, 0.2f}                  // Specular
    },
    { // heatpad
        {0.0f, 0.0f, 1.0f},                 // Direction
        0.1f,                               // Cut Off
        0.0f,                               // Outer Cut Off
        
        {0.0f, 120.0f, -60.0f},
        {0.2f, 0.2f, 0.2f},
        {0.6f, 0.15f, 0.025f},
        {0.01f, 0.01f, 0.01f}
    },
    { // heatpad
        {0.0f, 0.0f, 1.0f},                 // Direction
        0.1f,                               // Cut Off
        0.0f,                               // Outer Cut Off

        {0.0f, 194.0f, -60.0f},
        {0.2f, 0.2f, 0.2f},
        {0.6f, 0.15f, 0.025f},
        {0.01f, 0.01f, 0.01f}
    },
    { // heatpad
        {0.0f, 0.0f, 1.0f},                 // Direction
        0.1f,                               // Cut Off
        0.0f,                               // Outer Cut Off

        {0.0f, 270.0f, -60.0f},
        {0.2f, 0.2f, 0.2f},
        {0.6f, 0.15f, 0.025f},
        {0.01f, 0.01f, 0.01f}
    },
    { // heatpad
        {0.0f, 0.0f, 1.0f},                 // Direction
        0.1f,                               // Cut Off
        0.0f,                               // Outer Cut Off

        {0.0f, 363.0f, -60.0f},
        {0.2f, 0.2f, 0.2f},
        {0.6f, 0.15f, 0.025f},
        {0.01f, 0.01f, 0.01f}
    }
    
};


void setLightUniforms(ShaderProgram *shaderProgram) {
    for (int i = 0; i < NUM_LIGHTS; i++) {
        glUniform3fv(shaderProgram->lightDirectionLocations[i], 1, lights[i].direction);
        glUniform1f(shaderProgram->lightCutOffLocations[i], lights[i].cutOff);
        glUniform1f(shaderProgram->lightOuterCutOffLocations[i], lights[i].outerCutOff);

        glUniform3fv(shaderProgram->lightPositionLocations[i], 1, lights[i].position);
        glUniform3fv(shaderProgram->lightAmbientLocations[i], 1, lights[i].ambient);
        glUniform3fv(shaderProgram->lightDiffuseLocations[i], 1, lights[i].diffuse);
        glUniform3fv(shaderProgram->lightSpecularLocations[i], 1, lights[i].specular);
    }
}