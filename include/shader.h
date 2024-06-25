#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "global.h"


// Struktur zur Verwaltung von Shader-Programmen
typedef struct {
    GLuint program;
    GLuint vertexShader;
    GLuint fragmentShader;

    // Uniform-Locations
    GLint viewMatrixLocation;
    GLint projectionMatrixLocation;
    GLint modelMatrixLocation;
    GLint normalMatrixLocation;
    GLint textureSamplerLocation;
    GLint textureScaleLocation;
    GLint skyboxAmbientLocation;
    GLint clipPlaneLocation;

    GLint cameraPositionLocation;
    GLint materialEmissionLocation;
    GLint materialAmbientLocation;
    GLint materialDiffuseLocation;
    GLint materialSpecularLocation;
    GLint materialShininessLocation;
    GLint materialAlphaLocation;

    // Uniform-Locations für Lichtquellen
    GLint lightDirectionLocations[NUM_LIGHTS];
    GLint lightCutOffLocations[NUM_LIGHTS];
    GLint lightOuterCutOffLocations[NUM_LIGHTS];
    
    GLint lightPositionLocations[NUM_LIGHTS];
    GLint lightAmbientLocations[NUM_LIGHTS];
    GLint lightDiffuseLocations[NUM_LIGHTS];
    GLint lightSpecularLocations[NUM_LIGHTS];

    GLint reflectionTextureSamplerLocation;
    GLint refractionTextureSamplerLocation;
} ShaderProgram;

/**
 * @brief Erstellt ein Shader-Programm aus den Dateipfaden der Vertex- und Fragment-Shader.
 * 
 * @param vertexShaderPath Pfad zur Datei des Vertex-Shaders.
 * @param fragmentShaderPath Pfad zur Datei des Fragment-Shaders.
 * @return ShaderProgram Eine Struktur, die das Shader-Programm und weitere Zustandsinformationen enthält.
 */
ShaderProgram createShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath);

/**
 * @brief ein Shader-Programm und gibt alle zugehörigen Ressourcen frei.
 * 
 * @param shader Zeiger auf das ShaderProgram, das gelöscht werden soll.
 */
void deleteShaderProgram(ShaderProgram* shader);

/**
 * @brief Initialisiert die Uniform-Locations für das gegebene Shader-Programm.
 * 
 * @param shader Zeiger auf das ShaderProgram, dessen Uniform-Locations initialisiert werden sollen.
 */
void initUniformLocations(ShaderProgram* shader);


#endif // SHADER_H
