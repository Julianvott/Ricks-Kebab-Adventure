#ifndef OBJECT_H
#define OBJECT_H

#include <GL/glew.h>
#include "mesh.h"
#include "material.h"
#include "shader.h"
#include "matrix.h"
#include "texture.h"
#include "global.h"


// Struktur für Objekte, die Geometrie (Mesh), Material, Textur und Transformationen enthält
typedef struct {
    Mesh mesh;
    Material material;
    Texture texture;
    Vector3 position;
    Vector3 rotation;
    GLfloat scale;
} Object;

/**
 * Initialisiert ein Objekt mit angegebenem Pfad, Material und Textur.
 * 
 * @param objectPath Pfad zur Mesh-Datei.
 * @param objectMaterial Material des Objekts.
 * @param texture Textur des Objekts.
 * @return Ein initialisiertes Objekt.
 */
Object initObject(char *objectPath, Material objectMaterial, Texture texture);

/**
 * Zeichnet ein Objekt unter Verwendung eines gegebenen Shader-Programms.
 * 
 * @param object Zeiger auf das zu zeichnende Objekt.
 * @param shaderProgram Zeiger auf das verwendete Shader-Programm.
 */
void drawObject(const Object *object, ShaderProgram *shaderProgram);

/**
 * Löscht ein Objekt, indem es die zugehörigen Ressourcen freigibt.
 * 
 * @param object Zeiger auf das zu löschende Objekt.
 */
void deleteObject(Object *object);

#endif // OBJECT_H