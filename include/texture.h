#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <GL/glew.h>
#include <stdio.h>


typedef struct {
    char* path;
    float scale;
    GLuint id;
} Texture;

/**
 * Lädt eine Textur aus einer Datei und skaliert sie entsprechend.
 * 
 * @param filename Der Pfad zur Texturdatei.
 * @param scale Der Skalierungsfaktor, der auf die Texturgröße angewendet wird.
 * @return Texture Eine Texturstruktur, die die geladene Textur und zugehörige Informationen enthält.
 */
Texture loadTexture(char* filename, float scale);

#endif // TEXTURE_LOADER_H
