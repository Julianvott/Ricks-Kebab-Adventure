#version 330 core
out vec4 FragColor;

in vec3 Color; // Farbe vom Vertex-Shader

void main()
{
    FragColor = vec4(Color, 1.0); // Festlegen der Farbe des Pixels
}
