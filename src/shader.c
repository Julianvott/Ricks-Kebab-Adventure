#include "shader.h"

/**
 * @brief Hilfsfunktion zum kompilieren des Shaders
 * 
 * @param source Der Quelltext des Shaders als nullterminierter Zeichenkette
 * @param shaderType Der Typ des Shaders (z.B. GL_VERTEX_SHADER oder GL_FRAGMENT_SHADER)
 * @return GLuint 
 */
static GLuint compileShader(const char* source, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        fprintf(stderr, "Shader compilation error: %s\n", infoLog);
    }

    return shader;
}

ShaderProgram createShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath) {
    ShaderProgram shader;
    char* vertexSource = readFile(vertexShaderPath);
    char* fragmentSource = readFile(fragmentShaderPath);

    if (!vertexSource || !fragmentSource) {
        fprintf(stderr, "Failed to read shader files\n");
        exit(EXIT_FAILURE);
    }

    shader.vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
    shader.fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);

    shader.program = glCreateProgram();
    glAttachShader(shader.program, shader.vertexShader);
    glAttachShader(shader.program, shader.fragmentShader);
    glLinkProgram(shader.program);

    GLint success;
    glGetProgramiv(shader.program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetProgramInfoLog(shader.program, 1024, NULL, infoLog);
        fprintf(stderr, "Shader program linking error: %s\n", infoLog);
        glDeleteProgram(shader.program);
        exit(EXIT_FAILURE);
    }

    free(vertexSource);
    free(fragmentSource);

    // Uniform-Locations initialisieren
    initUniformLocations(&shader);

    return shader;
}

void deleteShaderProgram(ShaderProgram* shader) {
    glDeleteShader(shader->vertexShader);
    glDeleteShader(shader->fragmentShader);
    glDeleteProgram(shader->program);
}

void initUniformLocations(ShaderProgram* shader) {
    shader->viewMatrixLocation = glGetUniformLocation(shader->program, "viewMatrix");
    shader->projectionMatrixLocation = glGetUniformLocation(shader->program, "projectionMatrix");
    shader->modelMatrixLocation = glGetUniformLocation(shader->program, "modelMatrix");
    shader->normalMatrixLocation = glGetUniformLocation(shader->program, "normalMatrix");
    shader->textureSamplerLocation = glGetUniformLocation(shader->program, "textureSampler");
    shader->textureScaleLocation = glGetUniformLocation(shader->program, "textureScale");
    shader->skyboxAmbientLocation = glGetUniformLocation(shader->program, "skyboxAmbient");
    shader->clipPlaneLocation = glGetUniformLocation(shader->program, "clipPlane");
    
    shader->cameraPositionLocation = glGetUniformLocation(shader->program, "cameraPosition");
    shader->materialEmissionLocation = glGetUniformLocation(shader->program, "Material.emission");
    shader->materialAmbientLocation = glGetUniformLocation(shader->program, "Material.ambient");
    shader->materialDiffuseLocation = glGetUniformLocation(shader->program, "Material.diffuse");
    shader->materialSpecularLocation = glGetUniformLocation(shader->program, "Material.specular");
    shader->materialShininessLocation = glGetUniformLocation(shader->program, "Material.shininess");
    shader->materialAlphaLocation = glGetUniformLocation(shader->program, "Material.alpha");
    
    // Uniform-Locations für Lichtquellen
    for (int i = 0; i < NUM_LIGHTS; i++) {
        char baseName[128];

        snprintf(baseName, sizeof(baseName), "LightSource[%d].direction", i);
        shader->lightDirectionLocations[i] = glGetUniformLocation(shader->program, baseName);

        snprintf(baseName, sizeof(baseName), "LightSource[%d].cutOff", i);
        shader->lightCutOffLocations[i] = glGetUniformLocation(shader->program, baseName);

        snprintf(baseName, sizeof(baseName), "LightSource[%d].outerCutOff", i);
        shader->lightOuterCutOffLocations[i] = glGetUniformLocation(shader->program, baseName);

        snprintf(baseName, sizeof(baseName), "LightSource[%d].position", i);
        shader->lightPositionLocations[i] = glGetUniformLocation(shader->program, baseName);

        snprintf(baseName, sizeof(baseName), "LightSource[%d].ambient", i);
        shader->lightAmbientLocations[i] = glGetUniformLocation(shader->program, baseName);

        snprintf(baseName, sizeof(baseName), "LightSource[%d].diffuse", i);
        shader->lightDiffuseLocations[i] = glGetUniformLocation(shader->program, baseName);

        snprintf(baseName, sizeof(baseName), "LightSource[%d].specular", i);
        shader->lightSpecularLocations[i] = glGetUniformLocation(shader->program, baseName);
    }

    // Uniform für die oil reflektion
    shader->reflectionTextureSamplerLocation = glGetUniformLocation(shader->program, "reflectionTextureSampler");
    shader->refractionTextureSamplerLocation = glGetUniformLocation(shader->program, "refractionTextureSampler");
}