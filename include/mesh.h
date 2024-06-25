#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Struktur für Position-Daten
typedef struct {
    float x, y, z;
} Position;

// Struktur für Normale-Daten
typedef struct {
    float x, y, z;
} Normal;

// Struktur für Texturkoordinaten-Daten
typedef struct {
    float u, v;
} TexCoord;

// Struktur für Vertices-Daten, die Position, Texturkoordinaten und Normalen kombiniert
typedef struct {
    Position p;
    TexCoord t;
    Normal n;
} Vertex;

// Struktur für Geometrie-Daten, die das VAO, VBO und die Anzahl der Vertices enthält
typedef struct {
    GLuint vao;
    GLuint vbo;
    int numVertices;
} Mesh;

/**
 * @brief Lädt ein Mesh aus einer Datei und initialisiert die zugehörigen Datenstrukturen.
 * 
 * Liest Vertex-, Normalen- und Texturkoordinaten-Daten aus der angegebenen Datei und speichert sie im angegebenen Mesh-Objekt.
 * 
 * @param filename Pfad zur Datei, die das Mesh enthält.
 * @param mesh Zeiger auf das Mesh-Objekt, in das die Daten geladen werden.
 */
void loadMesh(const char *filename, Mesh *mesh);

/**
 * @brief Initialisiert das Vertex Array Object (VAO) für ein Mesh.
 * 
 * Bindet das VAO an den zuvor erstellten Vertex Buffer und konfiguriert die Vertex Attribute Pointer für Position, Normalen und Texturkoordinaten.
 * 
 * @param mesh Zeiger auf das Mesh-Objekt, dessen VAO konfiguriert wird.
 */
void setupMeshVAO(Mesh *mesh);

/**
 * @brief Zeichnet das Mesh.
 * 
 * Bindet das VAO des Meshes und führt einen Draw-Call aus, um das Mesh als Dreiecke zu rendern.
 * 
 * @param mesh Zeiger auf das Mesh-Objekt, das gezeichnet werden soll.
 */
void drawMesh(const Mesh *mesh);

/**
 * @brief Gibt die Ressourcen eines Meshes frei.
 * 
 * Löscht das VAO und den VBO des Meshes und gibt alle zugehörigen Ressourcen frei.
 * 
 * @param mesh Zeiger auf das Mesh-Objekt, dessen Ressourcen freigegeben werden sollen.
 */
void freeMesh(Mesh *mesh);


#endif // MESH_H