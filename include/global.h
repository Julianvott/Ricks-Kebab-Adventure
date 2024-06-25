#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdbool.h>


#define M_PI 3.14159265358979323846

// Definition der Konstanten für die Breite und Höhe
#define WIDTH 1440
#define HEIGHT 900

#define NUM_LIGHTS 6 // Definiert die maximale Anzahl der Lichtquellen

typedef struct {
    float x, y, z;
} Vector3;

extern bool isOutsideSpaceship;

#endif // GLOBAL_H