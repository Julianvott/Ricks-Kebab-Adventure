#ifndef MATRIX_TESTS_H
#define MATRIX_TESTS_H

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "matrix.h" 

#define EPSILON 0.00001
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * @brief Überprüft, ob die Identitätsmatrix korrekt erstellt wird.
 */
void testIdentity();

/**
 * @brief Überprüft, ob die Translationsmatrix korrekt auf eine Matrix angewendet wird.
 */
void testTranslate();

/**
 * @brief Überprüft, ob die Skalierungsmatrix korrekt auf eine Matrix angewendet wird.
 */
void testScale();

/**
 * @brief Überprüft, ob die Rotationsmatrix um die Z-Achse korrekt angewendet wird.
 */
void testRotateZ();

/**
 * @brief Überprüft, ob die Rotationsmatrix um die Y-Achse korrekt angewendet wird.
 */
void testRotateY();

/**
 * @brief Überprüft, ob die Rotationsmatrix um die X-Achse korrekt angewendet wird.
 */
void testRotateX();

/**
 * @brief Überprüft, ob die perspektivische Projektionsmatrix korrekt erstellt wird.
 */
void testPerspective();

/**
 * @brief Überprüft, ob die Normalisierung eines Vektors korrekt durchgeführt wird.
 */
void testNormalize();

/**
 * @brief Überprüft das Ergebnis des Kreuzprodukts zweier Vektoren.
 */
void testCrossProduct();

/**
 * @brief Überprüft, ob die LookAt-Transformationsmatrix korrekt erstellt wird.
 */
void testLookAt();

/**
 * @brief Überprüft, ob die orthografische Projektionsmatrix korrekt erstellt wird.
 */
void testOrthographic();

/**
 * @brief Überprüft, ob das Matrix-Multiplikationsergebnis korrekt ist.
 */
void testMultiply();

/**
 * @brief Führt alle Tests aus
 * 
 */
void executeTests();

#endif // MATRIX_TESTS_H
