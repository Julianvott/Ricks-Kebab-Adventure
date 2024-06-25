#include "mesh.h"

void loadMesh(const char *filename, Mesh *mesh) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return;
    }

    // Zählvariable für die Anzahl der Vertices
    int numVerticesCount = 0;
    int numNormalsCount = 0;
    int numTexCoordsCount = 0;
    int numFacesCount = 0;

    // Zeile für das Einlesen der Datei
    char line[256];

    // Zählen der Vertices
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "v ", 2) == 0) {
            numVerticesCount++;
        }
        else if (strncmp(line, "vt ", 3) == 0) {
            numTexCoordsCount++;
        } else if (strncmp(line, "vn ", 3) == 0) {
            numNormalsCount++;
        } else if (strncmp(line, "f ", 2) == 0) {
            numFacesCount++;
        }
    }

    // Zurückspulen der Datei, um sie erneut zu lesen
    rewind(file);

    // Speicher für Vertices, Normalen und Texturkoordinaten reservieren
    Position *positions = (Position *)malloc(numVerticesCount * sizeof(Position));
    Normal *normals = (Normal *)malloc(numNormalsCount * sizeof(Normal));
    TexCoord *texCoords = (TexCoord *)malloc(numTexCoordsCount * sizeof(TexCoord));
    Vertex *vertices = (Vertex *)malloc(numFacesCount * 3 * sizeof(Vertex));

    int numPositionsUsed = 0;
    int numNormalsUsed = 0;
    int numTexCoordsUsed = 0;
    int numVerticesUsed = 0;

    // Erneutes Lesen der Datei, um die Daten zu lesen
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "v ", 2) == 0) {
            // Position-Position
            Position position;
            sscanf(line + 2, "%f %f %f", &position.x, &position.y, &position.z);
            positions[numPositionsUsed++] = position;
        } else if (strncmp(line, "vt ", 3) == 0) {
            // Texturkoordinaten
            TexCoord texCoord;
            sscanf(line + 3, "%f %f", &texCoord.u, &texCoord.v);
            texCoords[numTexCoordsUsed++] = texCoord;
        } else if (strncmp(line, "vn ", 3) == 0) {
            // Normale
            Normal normal;
            sscanf(line + 3, "%f %f %f", &normal.x, &normal.y, &normal.z);
            normals[numNormalsUsed++] = normal;
        } else if (strncmp(line, "f ", 2) == 0) {
            // Face (Dreieck)
            int indices[3][3]; // Indizes für Position, Texturkoordinaten und Normalen
            sscanf(line + 2, "%d/%d/%d %d/%d/%d %d/%d/%d",
                   &indices[0][0], &indices[0][1], &indices[0][2],
                   &indices[1][0], &indices[1][1], &indices[1][2],
                   &indices[2][0], &indices[2][1], &indices[2][2]);

            vertices[numVerticesUsed++] = (Vertex){positions[indices[0][0] - 1], texCoords[indices[0][1] - 1], normals[indices[0][2] - 1]};
            vertices[numVerticesUsed++] = (Vertex){positions[indices[1][0] - 1], texCoords[indices[1][1] - 1], normals[indices[1][2] - 1]};
            vertices[numVerticesUsed++] = (Vertex){positions[indices[2][0] - 1], texCoords[indices[2][1] - 1], normals[indices[2][2] - 1]};
        }
    }

    fclose(file);

    mesh->numVertices = numVerticesUsed;

    // Erzeugen des VBO
    glGenBuffers(1, &mesh->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, numVerticesUsed * sizeof(Vertex), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Speicher freigeben
    free(positions);
    free(normals);
    free(texCoords);
    free(vertices);
}

void setupMeshVAO(Mesh *mesh) {
    glGenVertexArrays(1, &mesh->vao);
    glBindVertexArray(mesh->vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);

    // Positions-Attribut
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(0);

    // Normalen-Attribut
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, n)));
    glEnableVertexAttribArray(1);

    // Texturkoordinaten-Attribut
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, t)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void drawMesh(const Mesh *mesh) {
    glBindVertexArray(mesh->vao);
    glDrawArrays(GL_TRIANGLES, 0, mesh->numVertices);
}

void freeMesh(Mesh *mesh) {
    glDeleteVertexArrays(1, &mesh->vao);
    glDeleteBuffers(1, &mesh->vbo);
}