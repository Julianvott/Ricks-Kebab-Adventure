#include "matrix.h"

void printMatrix(const GLfloat *m) {
    printf("%f %f %f %f\n", m[0], m[4], m[8], m[12]);
    printf("%f %f %f %f\n", m[1], m[5], m[9], m[13]);
    printf("%f %f %f %f\n", m[2], m[6], m[10], m[14]);
    printf("%f %f %f %f\n", m[3], m[7], m[11], m[15]);
}

void perspective(GLfloat *out, GLfloat fovy, GLfloat aspect, GLfloat near, GLfloat far) {
    assert(fovy > 0 && fovy < 180 && "fovy must be in the range (0, 180)");
    assert(aspect > 0 && "aspect ratio must be positive");
    assert(near > 0 && "near plane must be positive");
    assert(far > near && "far plane must be greater than near plane");
    
    GLfloat t = (near) * (tanf((fovy) / 2.0f));
    GLfloat r = t * (aspect);
    out[0] = (near) / r;
    out[1] = 0.0f;
    out[2] = 0.0f;
    out[3] = 0.0f;
    out[4] = 0.0f;
    out[5] = (near) / t;
    out[6] = 0.0f;
    out[7] = 0.0f;
    out[8] = 0.0f;
    out[9] = 0.0f;
    out[10] = ((far) + (near)) / ((near) - (far));
    out[11] = -1.0f;
    out[12] = 0.0f;
    out[13] = 0.0f;
    out[14] = (2.0f * (far) * (near)) / ((near) - (far));
    out[15] = 0.0f;
}

void normalize(float *out) {
    GLfloat len = 0.0f;
    for (int i = 0; i < 3; i++) {
        len += (out[i] * out[i]);
    }
    len = sqrt(len);
    if(len > 0.0f) {
        for (int j = 0; j < 3; j++) {
            out[j] /= len;
        }
    }
}

void crossProduct(const GLfloat *a, const GLfloat *b, GLfloat *out) {
    out[0] = (a[1] * b[2]) - (a[2] * b[1]);
    out[1] = (a[2] * b[0]) - (a[0] * b[2]);
    out[2] = (a[0] * b[1]) - (a[1] * b[0]);
}

void lookAt(GLfloat *out, GLfloat *eye, GLfloat *target, GLfloat *up) {
    GLfloat viewDir[3] = { target[0] - eye[0], target[1] - eye[1], target[2] - eye[2] };
    assert((viewDir[0] != 0 || viewDir[1] != 0 || viewDir[2] != 0) && "Eye and target must not be the same point");
    
    GLfloat dotProduct = viewDir[0] * up[0] + viewDir[1] * up[1] + viewDir[2] * up[2];
    assert(dotProduct != 1 && dotProduct != -1 && "Up vector must not be collinear with the view direction");

    GLfloat u[3];
    GLfloat v[3];
    GLfloat n[3]; 

    for (int i = 0; i < 3; i++) {
        n[i] = eye[i] - target[i];
    }
    crossProduct(up, n, u);
    crossProduct(n, u, v);

    normalize(u);
    normalize(v);
    normalize(n);
    
    out[0] = u[0];
    out[1] = v[0];
    out[2] = n[0];
    out[3] = 0;
    out[4] = u[1];
    out[5] = v[1];
    out[6] = n[1];
    out[7] = 0;
    out[8] = u[2];
    out[9] = v[2];
    out[10] = n[2];
    out[11] = 0;
    out[12] = -(u[0] * eye[0] + u[1] * eye[1] + u[2] * eye[2]);
    out[13] = -(v[0] * eye[0] + v[1] * eye[1] + v[2] * eye[2]);
    out[14] = -(n[0] * eye[0] + n[1] * eye[1] + n[2] * eye[2]);
    out[15] = 1;
}

void identity(float *out) {
    // Setzt die Werte der Matrix so, dass sie eine Einheitsmatrix darstellt
    for (int i = 0; i <= 15; i++) {
        if (i % 5 == 0) {
            out[i] = 1.0f;
        } else {
            out[i] = 0.0f;
        }
    }
}

void translate(float *out, float *v) {
    out[12] += v[0];
    out[13] += v[1];
    out[14] += v[2];
}

void scale(float *out, float v) {
    out[0] *= v;
    out[5] *= v;
    out[10] *= v;
}

void rotatez(float *out, float angle) {
    out[0] = cos(angle);
    out[1] = -sin(angle);
    out[4] = sin(angle);
    out[5] = cos(angle);
}

void rotatey(float *out, float angle) {
    out[0] = cos(angle);
    out[2] = -sin(angle);
    out[8] = sin(angle);
    out[10] = cos(angle);
}

void rotatex(float *out, float angle) {
    out[5] = cos(angle);
    out[6] = sin(angle);
    out[9] = -sin(angle);
    out[10] = cos(angle);
}

void extract3x3Matrix(GLfloat dest[9], const GLfloat src[16]) {
    dest[0] = src[0]; dest[1] = src[1]; dest[2] = src[2];
    dest[3] = src[4]; dest[4] = src[5]; dest[5] = src[6];
    dest[6] = src[8]; dest[7] = src[9]; dest[8] = src[10];
}

void orthographic(GLfloat *out, GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far) {
    out[0] = 2.0f / (right - left);
    out[1] = 0.0f;
    out[2] = 0.0f;
    out[3] = 0.0f;

    out[4] = 0.0f;
    out[5] = 2.0f / (top - bottom);
    out[6] = 0.0f;
    out[7] = 0.0f;

    out[8] = 0.0f;
    out[9] = 0.0f;
    out[10] = -2.0f / (far - near);
    out[11] = 0.0f;

    out[12] = -(right + left) / (right - left);
    out[13] = -(top + bottom) / (top - bottom);
    out[14] = -(far + near) / (far - near);
    out[15] = 1.0f;
}

void multiply(GLfloat *result, GLfloat *a, GLfloat *b) {
    GLfloat temp[16];

    temp[0] = a[0] * b[0] + a[1] * b[4] + a[2] * b[8] + a[3] * b[12];
    temp[1] = a[0] * b[1] + a[1] * b[5] + a[2] * b[9] + a[3] * b[13];
    temp[2] = a[0] * b[2] + a[1] * b[6] + a[2] * b[10] + a[3] * b[14];
    temp[3] = a[0] * b[3] + a[1] * b[7] + a[2] * b[11] + a[3] * b[15];

    temp[4] = a[4] * b[0] + a[5] * b[4] + a[6] * b[8] + a[7] * b[12];
    temp[5] = a[4] * b[1] + a[5] * b[5] + a[6] * b[9] + a[7] * b[13];
    temp[6] = a[4] * b[2] + a[5] * b[6] + a[6] * b[10] + a[7] * b[14];
    temp[7] = a[4] * b[3] + a[5] * b[7] + a[6] * b[11] + a[7] * b[15];

    temp[8] = a[8] * b[0] + a[9] * b[4] + a[10] * b[8] + a[11] * b[12];
    temp[9] = a[8] * b[1] + a[9] * b[5] + a[10] * b[9] + a[11] * b[13];
    temp[10] = a[8] * b[2] + a[9] * b[6] + a[10] * b[10] + a[11] * b[14];
    temp[11] = a[8] * b[3] + a[9] * b[7] + a[10] * b[11] + a[11] * b[15];

    temp[12] = a[12] * b[0] + a[13] * b[4] + a[14] * b[8] + a[15] * b[12];
    temp[13] = a[12] * b[1] + a[13] * b[5] + a[14] * b[9] + a[15] * b[13];
    temp[14] = a[12] * b[2] + a[13] * b[6] + a[14] * b[10] + a[15] * b[14];
    temp[15] = a[12] * b[3] + a[13] * b[7] + a[14] * b[11] + a[15] * b[15];

    for (int i = 0; i < 16; i++) {
        result[i] = temp[i];
    }
}