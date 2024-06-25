#include "framebuffer.h"

/**
 * @brief Erstellt ein Bildspeicher-Objekt
 * 
 * @param fbo Zeiger auf ein Bildspeicher-Objekt, welches initialisiert wird.
 */
static void createFrameBuffer(GLuint *fbo) {
    glGenFramebuffers(1, fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, *fbo);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

/**
 * @brief Erstellt ein Farbtexturanhang-Objekt
 * 
 * @param colorTexture Zeiger auf ein GLuint, wo das Farbtexturanhang-Objekt reingeschireben wird
 */
static void createTextureAttachment(GLuint *colorTexture) {
    glGenTextures(1, colorTexture);
    glBindTexture(GL_TEXTURE_2D, *colorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, *colorTexture, 0);
}

/**
 * @brief Erstellt ein Tiefentexturanhang-Objekt
 * 
 * @param depthTexture Zeiger auf ein GLuint, wo das Tiefentexturanhang-Objekt reingeschireben wird
 */
static void createDepthTextureAttachment(GLuint *depthTexture) {
    glGenTextures(1, depthTexture);
    glBindTexture(GL_TEXTURE_2D, *depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, WIDTH, HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, *depthTexture, 0);
}

/**
 * @brief Erstellt ein Tiefenpufferanhang-Objekt
 * 
 * @param depthBuffer Zeiger auf ein GLuint, wo das Tiefenpufferanhang-Objekt reingeschireben wird
 */
static void createDepthBufferAttachment(GLuint *depthBuffer) {
    glGenTextures(1, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, *depthBuffer);
}

void deleteFrameBuffer(FrameBuffer *frameBuffer) {
    glDeleteFramebuffers(1, &frameBuffer->fbo);
    glDeleteTextures(1, &frameBuffer->colorTexture);
    glDeleteTextures(1, &frameBuffer->depthTexture);
    glDeleteRenderbuffers(1, &frameBuffer->depthBuffer);
}

FrameBuffer initFrameBuffer() {
    GLuint fbo;
    GLuint colorTexture;
    GLuint depthTexture;
    GLuint depthBuffer;

    createFrameBuffer(&fbo);
    createTextureAttachment(&colorTexture);
    createDepthTextureAttachment(&depthTexture);
    createDepthBufferAttachment(&depthBuffer);

    return (FrameBuffer) {fbo, colorTexture, depthTexture, depthBuffer};
}