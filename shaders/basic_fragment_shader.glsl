#version 330 core

in vec3 fragNormal;
in vec2 fragTexCoord;
in vec3 fragPosition;
in vec4 clipSpace;
in vec3 toCameraVector;

out vec4 fragColor;
vec4 textureColor;

uniform sampler2D textureSampler;
uniform sampler2D reflectionTextureSampler;
uniform sampler2D refractionTextureSampler;
uniform bool isReflective;

uniform vec3 skyboxAmbient;
struct MaterialParams {
    vec3 emission;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    float alpha;
};

uniform MaterialParams Material;

struct LightParams {
    vec3 direction;
    float cutOff;
    float outerCutOff;
    
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NUM_LIGHTS 6
uniform LightParams LightSource[NUM_LIGHTS];

uniform vec3 cameraPosition;

vec3 calcLight(LightParams light, vec3 normal, vec3 viewDir, bool backface) {
    vec3 lightDir = normalize(light.position - fragPosition);

    // Ambiente Komponente
    vec3 ambient = Material.ambient * mix(skyboxAmbient, light.ambient * textureColor.rgb, 0.9);

    // Diffuse Komponente
    float diff;
    if (backface) { diff = abs(min(dot(normal, lightDir), 0.0)); }
    else { diff = max(dot(normal, lightDir), 0.0); }
    vec3 diffuse = diff * light.diffuse * Material.diffuse * textureColor.rgb;
    
    // Spekulare Komponente
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec;
    if (backface) { spec = pow(abs(min(dot(normal, halfwayDir), 0.0)), Material.shininess); }
    else { spec = pow(max(dot(normal, halfwayDir), 0.0), Material.shininess); }
    vec3 specular = spec * light.specular * Material.specular;

    // Spotlight
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse *= intensity;
    specular *= intensity;

    // Entfernungsabfall für das Licht
    float distance = length(light.position - fragPosition);
    float attenuation = 1.0 / (1.0 + 0.00000005 * distance + 0.000000005 * distance * distance);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

void main() {
    vec3 norm = normalize(fragNormal);
    vec3 viewDir = normalize(cameraPosition - fragPosition);

    textureColor = texture(textureSampler, fragTexCoord);

    // Initial Farbe mit der emissiven Komponente
    vec3 Color = Material.emission;

    // Setze den Lichteinfluss
    for (int i = 0; i < NUM_LIGHTS; i++) {
        if (Material.alpha < 1.0) { // Material is transparent
            Color += mix(calcLight(LightSource[i], norm, viewDir, true), calcLight(LightSource[i], norm, viewDir, false), 0.5);
        }
        Color += calcLight(LightSource[i], norm, viewDir, false);
    }

    // Reflektive Komponente
    if (isReflective) {
        vec2 ndc = (clipSpace.xy/clipSpace.w) / 2.0 + 0.5; // normalized device coords
        vec2 reflectTextCoords = vec2(-ndc.x, ndc.y);
        vec2 refractTextCoords = vec2(ndc.x, ndc.y);

        vec4 reflectColor = texture(reflectionTextureSampler, reflectTextCoords);
        vec4 refractColor = texture(refractionTextureSampler, refractTextCoords);

        // Berechung für den Fresnel Effect
        vec3 viewVector = normalize(toCameraVector); // Vektor richtung Kamera
        float refractiveFactor = dot(viewVector, vec3(0.0, 1.0, 0.0)); // vec3: Normalen-Vektor der Öl-Ebene
        refractiveFactor = pow(refractiveFactor, 1.0);

        vec4 combindedEffect = mix(reflectColor, refractColor, refractiveFactor);
        fragColor = mix(combindedEffect, vec4(Color, 1.0), 0.3);

    } else {
        fragColor = vec4(Color, Material.alpha) * textureColor;
    }
}