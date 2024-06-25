#include "object.h"


Object initObject(char *objectPath, Material objectMaterial, Texture texture) {
    Object object;
    
    // create mesh, add mesh
    Mesh objectMesh;
    loadMesh(objectPath, &objectMesh);
    setupMeshVAO(&objectMesh);
    object.mesh = objectMesh;

    // add material
    object.material = objectMaterial;

    // add texture
    object.texture = texture;

    // add default position
    object.position = (Vector3){0.0f, 0.0f, 0.0f};

    // add default rotation
    object.rotation = (Vector3){ 0.0f, 0.0f, 0.0f};

    // add default scale
    object.scale = 1.0f;

    return object;
}

void drawObject(const Object *object, ShaderProgram *shaderProgram) {
    // Aktivieren der Textur

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, object->texture.id);
    glUniform1i(shaderProgram->textureSamplerLocation, 0);
    // Texturskalierung setzen
    glUniform1f(shaderProgram->textureScaleLocation, object->texture.scale);

    // Materialparameter setzen
    glUniform3fv(shaderProgram->materialEmissionLocation, 1, object->material.emission);
    glUniform3fv(shaderProgram->materialAmbientLocation, 1, object->material.ambient);
    glUniform3fv(shaderProgram->materialDiffuseLocation, 1, object->material.diffuse);
    glUniform3fv(shaderProgram->materialSpecularLocation, 1, object->material.specular);
    glUniform1f(shaderProgram->materialShininessLocation, object->material.shininess);
    glUniform1f(shaderProgram->materialAlphaLocation, object->material.alpha);

    // Transformationen setzen
    GLfloat rotateXTransform[16];
    GLfloat rotateYTransform[16];
    GLfloat rotateZTransform[16];
    GLfloat scaleTransform[16];
    GLfloat translateTransform[16];
    GLfloat transform[16];


    identity(rotateXTransform);
    identity(rotateYTransform);
    identity(rotateZTransform);
    identity(scaleTransform);
    identity(translateTransform);
    
     // skalieren
    scale(scaleTransform, object->scale);

    // rotieren
    rotatex(rotateXTransform, object->rotation.x);
    rotatez(rotateYTransform, object->rotation.z);
    rotatey(rotateZTransform, object->rotation.y);
    
    // verschieben
    translate(translateTransform, (float[]){object->position.x, object->position.y, object->position.z});


    multiply(transform, rotateXTransform, rotateYTransform);
    multiply(transform, transform, rotateZTransform);
    multiply(transform, transform, scaleTransform);
    multiply(transform, transform, translateTransform);


    glUniformMatrix4fv(shaderProgram->modelMatrixLocation, 1, GL_FALSE, transform);
    
    GLfloat normalMatrix[9];
    extract3x3Matrix(normalMatrix, transform);
    glUniformMatrix3fv(shaderProgram->normalMatrixLocation, 1, GL_FALSE, normalMatrix);

    drawMesh(&object->mesh);

    glBindTexture(GL_TEXTURE_2D, 0); // unbind texture
}

void deleteObject(Object *object) {
    // Löschen der Vertex Array Object
    if (object->mesh.vao != 0 && object->mesh.vbo != 0) {
        freeMesh(&object->mesh);
        object->mesh.vao = 0;
        object->mesh.vbo = 0;
    }

    // Löschen der Textur
    if (object->texture.id != 0) {
        glDeleteTextures(1, &object->texture.id);
        object->texture.id = 0;
    }
}