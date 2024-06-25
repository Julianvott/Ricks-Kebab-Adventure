#include "hud.h"

static HUD hud;
static bool hudEnabled = true;

void initHUD() {
    const char *vertexShaderPath = "shaders/hud_vertex_shader.glsl";
    const char *fragmentShaderPath = "shaders/hud_fragment_shader.glsl";
    hud.shaderProgram = createShaderProgram(vertexShaderPath, fragmentShaderPath);

    glGenVertexArrays(1, &hud.VAO);
    glGenBuffers(1, &hud.VBO); 
    glGenBuffers(1, &hud.EBO);
}

/**
 * @brief Erstellt ein Kreis Objekt
 * 
 * @param vertices Die vertices des Kreises
 * @param indices Die indices des Kreises
 * @param x Die x Position des Kreismittelpunktes
 * @param y Die y Position des Kreismittelpunktes
 * @param radius Der Radius des Kreises
 * @param filled Wenn true wird der Kreis gefüllt
 * @param redGradient Wenn true wird ein Gradient vom Mittelpunkt des Kreises erstellt
 */
static void createCircle(GLfloat *vertices, GLuint *indices, float x, float y, float radius, bool filled, bool redGradient) {
    // Zentrum für gefüllte Kreise
    if (filled) {
        vertices[0] = x;
        vertices[1] = y;
        vertices[2] = 1.0f; // Farbe weiß bzw. rot wenn redGradient==true
        vertices[3] = redGradient ? 0.3f : 1.0f;
        vertices[4] = redGradient ? 0.0f : 1.0f;
    }

    for (int i = 0; i < CIRCLE_SEGMENTS; ++i) {
        float theta = 2.0f * M_PI * (float) (i) / (float) (CIRCLE_SEGMENTS);
        float dx = radius * cosf(theta);
        float dy = radius * sinf(theta);

        int vertexIndex = (filled ? 1 : 0) + i;
        vertices[vertexIndex * 5] = x + dx;
        vertices[vertexIndex * 5 + 1] = y + dy;
        vertices[vertexIndex * 5 + 2] = 1.0f; // Farbe weiß
        vertices[vertexIndex * 5 + 3] = 1.0f;
        vertices[vertexIndex * 5 + 4] = 1.0f;

        if (filled) {
            indices[i * 3] = 0;
            indices[i * 3 + 1] = vertexIndex;
            indices[i * 3 + 2] = vertexIndex + 1;
        } else {
            indices[i * 2] = vertexIndex;
            indices[i * 2 + 1] = (vertexIndex + 1) % CIRCLE_SEGMENTS;
        }
    }

    if (filled) {
        indices[(CIRCLE_SEGMENTS - 1) * 3 + 2] = 1;
    }
}

/**
 * @brief Zeichnet ein Kreis
 * 
 * @param x Die x Position des Kreismittelpunktes
 * @param y Die y Position des Kreismittelpunktes
 * @param radius Der Radius des Kreises
 * @param filled Wenn true wird der Kreis gefüllt
 * @param redGradient Wenn true wird ein Gradient vom Mittelpunkt des Kreises erstellt
 */
static void drawCircle(float x, float y, float radius, bool filled, bool redGradient) {
    int numVertices = filled ? CIRCLE_SEGMENTS + 1 : CIRCLE_SEGMENTS;
    int numIndices = filled ? CIRCLE_SEGMENTS * 3 : CIRCLE_SEGMENTS * 2;

    GLfloat *vertices = (GLfloat*)malloc(numVertices * 5 * sizeof(GLfloat));
    GLuint *indices = (GLuint*)malloc(numIndices * sizeof(GLuint));

    createCircle(vertices, indices, x, y, radius, filled, redGradient);

    glBindVertexArray(hud.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, hud.VBO);
    glBufferData(GL_ARRAY_BUFFER, numVertices * 5 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, hud.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glDrawElements(filled ? GL_TRIANGLES : GL_LINES, numIndices, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);

    free(vertices);
    free(indices);
}

/**
 * @brief Zeichnet die Control Elemente
 * 
 */
static void drawControlElements(){
    // Nicht ausgefüllte Kreise und ihre ausgefüllten Kreise zeichnen
    float centerX = WIDTH / 1.15f;
    float centerY = HEIGHT / 1.3f;
    float minRadius = 30.0f; // Beispielwert für den kleinsten Radius
    float radiusStep = 30.0f; // Abstand zwischen den Kreisen

    for (int i = 0; i < NUM_PRESETS_DISTANCE; ++i) {
        float radius = minRadius + (NUM_PRESETS_DISTANCE - i - 1) * radiusStep;

        // Definieren eines Winkels Offsets, um unten zu beginnen
        float thetaOffset = M_PI / 2;

        for (int j = 0; j < NUM_PRESETS_SURROUND; ++j) {
            // Verschieben des Startwinkels um thetaOffset
            float theta = 2.0f * M_PI * (float)j / (float)NUM_PRESETS_SURROUND + thetaOffset;
            float dx = radius * cosf(theta);
            float dy = radius * sinf(theta);

            bool isCurrentPosition = isOutsideSpaceship && j == targetPresetSurroundIndex && i == targetPresetDistanceIndex;

            drawCircle(centerX + dx, centerY + dy, 10.0f, true, isCurrentPosition); // Ausgefüllte Kreise mit festem Radius
        }
        
        drawCircle(centerX, centerY, radius, false, false);
    }
    
    // Spezial Kreis Zeichnen
    float specialCenterX = WIDTH - 60.0f; // X-Position der rechten unteren Ecke minus Radius
    float specialCenterY = HEIGHT - 60.0f; // Y-Position der rechten unteren Ecke minus Radius
    float specialRadius = 20.0f; // Radius des speziellen Kreises

    drawCircle(specialCenterX, specialCenterY, specialRadius, true, !isOutsideSpaceship ? true : false); // spezieller Ausgefüllter Kreis
}

void drawHUD() {
    if (!hudEnabled){
        return;
    }

    glUseProgram(hud.shaderProgram.program);
    glDisable(GL_CULL_FACE); // da 2d nicht 3d, alles muss gerendert werden
    // 2D-Orthoprojektion für HUD
    GLfloat orthoProjection[16];
    identity(orthoProjection);
    orthographic(orthoProjection, 0.0f, (float)WIDTH, (float)HEIGHT, 0.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(glGetUniformLocation(hud.shaderProgram.program, "projection"), 1, GL_FALSE, orthoProjection);
    drawControlElements();
    glEnable(GL_CULL_FACE);
}

void enableHud(){
    hudEnabled = true;
}

void disableHud(){
    hudEnabled = false;
}

void deleteHUD() {
    glDeleteVertexArrays(1, &hud.VAO);  
    glDeleteBuffers(1, &hud.VBO);      
    glDeleteBuffers(1, &hud.EBO);       
    deleteShaderProgram(&hud.shaderProgram);
}
