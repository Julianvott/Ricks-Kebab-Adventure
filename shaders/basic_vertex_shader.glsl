#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec3 fragNormal;
out vec2 fragTexCoord;
out vec3 fragPosition;
out vec4 clipSpace;
out vec3 toCameraVector;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;
uniform float textureScale;
uniform vec4 clipPlane; // Clipping Plane
uniform vec3 cameraPosition; // für den Fresnel Effect

void main() {
    fragNormal = normalize(normalMatrix * normal);
    fragTexCoord = texCoord * textureScale;
    fragPosition = vec3(modelMatrix * vec4(position, 1.0));

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);

    // Berechnen Sie die Clip-Distanz in View-Koordinaten 
    gl_ClipDistance[0] = dot(modelMatrix * vec4(position, 1.0), clipPlane);

    clipSpace = gl_Position;

    vec4 worldPosition = modelMatrix * vec4(position, 1.0);
    toCameraVector = cameraPosition - worldPosition.xyz;
}