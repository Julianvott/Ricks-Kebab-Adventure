#ifndef MATRIX_H
#define MATRIX_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "matrix.h"


/**
 * @brief Druckt eine Matrix auf die Konsole.
 * 
 * @param m Zeiger auf die Matrix, die gedruckt werden soll.
 */
void printMatrix(const GLfloat *m);

/**
 * @brief Erstellt eine perspektivische Projektionsmatrix.
 * 
 * @param out Ausgabe-Pointer, der die perspektivische Matrix speichert.
 * @param fovy Sichtfeld in der y-Richtung, in Grad.
 * @param aspect Seitenverhältnis der Viewport.
 * @param near Nahklippebene.
 * @param far Fernklippebene.
 */
void perspective(GLfloat *out, GLfloat fovy, GLfloat aspect, GLfloat near, GLfloat far);

/**
 * @brief Normalisiert einen Vektor.
 * 
 * @param out Ausgabe-Pointer, der den normalisierten Vektor speichert.
 */
void normalize(float *out);

/**
 * @brief Berechnet das Kreuzprodukt zweier Vektoren.
 * 
 * @param a Zeiger auf den ersten Vektor.
 * @param b Zeiger auf den zweiten Vektor.
 * @param out Ausgabe-Pointer, der das Ergebnis des Kreuzprodukts speichert.
 */
void crossProduct(const GLfloat *a, const GLfloat *b, GLfloat *out);

/**
 * @brief Erstellt eine LookAt-View-Matrix.
 * 
 * @param out Ausgabe-Pointer für die View-Matrix.
 * @param eye Zeiger auf den Augpunkt-Vektor.
 * @param target Zeiger auf den Zielpunkt-Vektor.
 * @param up Zeiger auf den Up-Vektor.
 */
void lookAt(GLfloat *out, GLfloat *eye, GLfloat *target, GLfloat *up);

/**
 * @brief Setzt eine Matrix auf die Identitätsmatrix.
 * 
 * @param out Ausgabe-Pointer für die Identitätsmatrix.
 */
void identity(float *out);

/**
 * @brief Führt eine Translationsoperation durch.
 * 
 * @param out Ausgabe-Pointer, der die transformierte Matrix speichert.
 * @param v Translationsvektor.
 */
void translate(float *out, float *v);

/**
 * @brief Skaliert eine Matrix mit einem Skalierungsfaktor.
 * 
 * @param out Ausgabe-Pointer, der die skalierte Matrix speichert.
 * @param v Skalierungsfaktor.
 */
void scale(float *out, float v);

/**
 * @brief Rotiert eine Matrix um die z-Achse.
 * 
 * @param out Ausgabe-Pointer, der die rotierte Matrix speichert.
 * @param angle Rotationswinkel in Grad.
 */
void rotatez(float *out, float angle);

/**
 * @brief Rotiert eine Matrix um die y-Achse.
 * 
 * @param out Ausgabe-Pointer, der die rotierte Matrix speichert.
 * @param angle Rotationswinkel in Grad.
 */
void rotatey(float *out, float angle);

/**
 * @brief Rotiert eine Matrix um die x-Achse.
 * 
 * @param out Ausgabe-Pointer, der die rotierte Matrix speichert.
 * @param angle Rotationswinkel in Grad.
 */
void rotatex(float *out, float angle);

/**
 * @brief Extrahiert eine 3x3 Matrix aus einer 4x4 Matrix.
 * 
 * @param dest Ziel-Array, das die 3x3 Matrix speichert.
 * @param src Quell-Array, das die 4x4 Matrix enthält.
 */
void extract3x3Matrix(GLfloat dest[9], const GLfloat src[16]);

/**
 * @brief Erstellt eine orthographische Projektionsmatrix.
 * 
 * @param out Ausgabe-Pointer für die orthographische Matrix.
 * @param left Linke Begrenzung des Viewports.
 * @param right Rechte Begrenzung des Viewports.
 * @param bottom Untere Begrenzung des Viewports.
 * @param top Obere Begrenzung des Viewports.
 * @param near Nahklippebene.
 * @param far Fernklippebene.
 */
void orthographic(GLfloat *out, GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far);

/**
 * @brief Multipliziert zwei Matrizen.
 * 
 * @param result Ausgabe-Pointer, der das Ergebnis der Multiplikation speichert.
 * @param a Zeiger auf die erste Matrix.
 * @param b Zeiger auf die zweite Matrix.
 */
void multiply(GLfloat *result, GLfloat *a, GLfloat *b);


#endif // MATRIX_H