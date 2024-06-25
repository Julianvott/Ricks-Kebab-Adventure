#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "global.h"
#include "shader.h"


typedef struct {
    GLuint fbo;
    GLuint colorTexture;
    GLuint depthTexture;
    GLuint depthBuffer;
} FrameBuffer;

/**
 * @brief Initialisiert ein Bildspeicher
 * 
 * @return Gibt den Bildspeicher zurück
 */
FrameBuffer initFrameBuffer();

/**
 * @brief Löscht die Bildspeicher Ressourcen
 * 
 * @param frameBuffer Zeiger auf den Bildspeicher, dessen Ressourcen gelöscht werden sollen
 */
void deleteFrameBuffer(FrameBuffer *frameBuffer);

#endif // FRAMEBUFFER_H