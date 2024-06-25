#include "matrixTest.h"


/**
 * @brief Vergleicht die beiden Matrizen elementweise, unter Verwendung eines definierten Epsilon als Toleranz für Gleitkommavergleiche.
 * 
 * @param a Zeiger auf das erste Array von GLfloats, das die erste Matrix darstellt.
 * @param b Zeiger auf das zweite Array von GLfloats, das die zweite Matrix darstellt.
 * @param size Die Anzahl der Elemente in jeder Matrix, die verglichen werden sollen.
 * @return int Gibt 1 zurück, wenn alle korrespondierenden Elemente der beiden Matrizen innerhalb der Epsilon-Toleranz gleich sind; andernfalls 0.
 */
static int compareMatrices(const GLfloat *a, const GLfloat *b, int size) {
    for (int i = 0; i < size; i++) {
        if (fabs(a[i] - b[i]) > EPSILON) {
            return 0;
        }
    }
    return 1;
}

void testIdentity() {
    GLfloat m[16];
    GLfloat expected[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    identity(m);
    printf("testIdentity: %s\n", compareMatrices(m, expected, 16) ? "OK" : "Nicht OK");
}

void testTranslate() {
    GLfloat m[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    GLfloat v[3] = {1, 2, 3};
    GLfloat expected[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        1, 2, 3, 1
    };
    translate(m, v);
    printf("testTranslate: %s\n", compareMatrices(m, expected, 16) ? "OK" : "Nicht OK");
}

void testScale() {
    GLfloat m[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    GLfloat factor = 2.0f;
    GLfloat expected[16] = {
        2, 0, 0, 0,
        0, 2, 0, 0,
        0, 0, 2, 0,
        0, 0, 0, 1
    };
    scale(m, factor);
    printf("testScale: %s\n", compareMatrices(m, expected, 16) ? "OK" : "Nicht OK");
    
}

void testRotateZ() {
    GLfloat m[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    GLfloat angle = M_PI / 2; // 90 degrees
    GLfloat expected[16] = {
        0, 1, 0, 0,
        -1, 0, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    rotatez(m, angle);
    printf("testRotateZ: %s\n", compareMatrices(m, expected, 16) ? "OK" : "Nicht OK");
}

void testRotateY() {
    GLfloat m[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    GLfloat angle = M_PI / 2; // 90 degrees
    GLfloat expected[16] = {
        0, 0, -1, 0,
        0, 1, 0, 0,
        1, 0, 0, 0,
        0, 0, 0, 1
    };
    rotatey(m, angle);
    printf("testRotateY: %s\n", compareMatrices(m, expected, 16) ? "OK" : "Nicht OK");
}

void testRotateX() {
    GLfloat m[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    GLfloat angle = M_PI / 2; // 90 degrees
    GLfloat expected[16] = {
        1, 0, 0, 0,
        0, 0, 1, 0,
        0, -1, 0, 0,
        0, 0, 0, 1
    };
    rotatex(m, angle);
    printf("testRotateX: %s\n", compareMatrices(m, expected, 16) ? "OK" : "Nicht OK");
}

void testPerspective() {
    GLfloat m[16];
    GLfloat fovy = M_PI / 4; // 45 degrees
    GLfloat aspect = 1.0f;
    GLfloat near = 1.0f;
    GLfloat far = 10.0f;
    GLfloat expected[16] = {
        2.414213, 0, 0, 0,
        0, 2.414213, 0, 0,
        0, 0, -1.222222, -1,
        0, 0, -2.222222, 0
    };
    perspective(m, fovy, aspect, near, far);
    printf("testPerspective: %s\n", compareMatrices(m, expected, 16) ? "OK" : "Nicht OK");
}

void testNormalize() {
    GLfloat v[3] = {1, 2, 3};
    GLfloat expected[3] = {
        0.267261, 0.534522, 0.801784
    };
    normalize(v);
    printf("testNormalize: %s\n", compareMatrices(v, expected, 3) ? "OK" : "Nicht OK");
}

void testCrossProduct() {
    GLfloat a[3] = {1, 0, 0};
    GLfloat b[3] = {0, 1, 0};
    GLfloat result[3];
    GLfloat expected1[3] = {0, 0, 1};
    crossProduct(a, b, result);
    printf("testCrossProduct: %s\n", compareMatrices(result, expected1, 3) ? "OK" : "Nicht OK");

    GLfloat expected2[3] = {0, 0, 0};
    crossProduct(a, a, result);
    printf("testCrossProduct (same array): %s\n", compareMatrices(result, expected2, 3) ? "OK" : "Nicht OK");
}

void testLookAt() {
    GLfloat m[16];
    GLfloat eye[3] = {0, 0, 1};
    GLfloat origin[3] = {0, 0, 0};
    GLfloat up[3] = {0, 1, 0};
    GLfloat expected[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, -1, 1
    };
    lookAt(m, eye, origin, up);
    printf("testLookAt: %s\n", compareMatrices(m, expected, 16) ? "OK" : "Nicht OK");
}

void testOrthographic() {
    GLfloat m[16];
    GLfloat left = -1, right = 1, bottom = -1, top = 1, near = -1, far = 1;
    GLfloat expected[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, -1, 0,
        0, 0, 0, 1
    };
    orthographic(m, left, right, bottom, top, near, far);
    printf("testOrthographic: %s\n", compareMatrices(m, expected, 16) ? "OK" : "Nicht OK");
}

void testMultiply() {
    GLfloat a[16] = {
        1, 0, 0, 0,
        0, -1, 0, 0,
        0, 0, -1, 0,
        0, 0, 0, 1
    };
    GLfloat b[16] = {
        2, 0, 0, 0,
        0, 2, 0, 0,
        0, 0, -2, 0,
        0, 0, 0, 1
    };
    GLfloat result[16];
    GLfloat expected1[16] = {
        2, 0, 0, 0,
        0, -2, 0, 0,
        0, 0, 2, 0,
        0, 0, 0, 1
    };
    multiply(result, a, b);
    printf("testMultiply: %s\n", compareMatrices(result, expected1, 16) ? "OK" : "Nicht OK");

    GLfloat expected2[16] = {
        4, 0, 0, 0,
        0, 4, 0, 0,
        0, 0, 4, 0,
        0, 0, 0, 1
    };
    multiply(result, b, b);
    printf("testMultiply (same array): %s\n", compareMatrices(result, expected2, 16) ? "OK" : "Nicht OK");
}

void executeTests() {
    testIdentity();
    testTranslate();
    testScale();
    testRotateZ();
    testRotateY();
    testRotateX();
    testPerspective();
    testNormalize();
    testCrossProduct();
    testLookAt();
    testOrthographic();
    testMultiply();
}
