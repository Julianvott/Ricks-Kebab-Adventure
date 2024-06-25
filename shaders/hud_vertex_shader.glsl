#version 330 core
layout (location = 0) in vec2 aPos; // Position des HUD-Elementes
layout (location = 1) in vec3 aColor; // Farbe des HUD-Elementes

out vec3 Color; // Übergabe der Farbe an den Fragment-Shader

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(aPos, 0.0, 1.0); // Projektion in den Bildschirmraum
    Color = aColor; // Farbe übergeben
}