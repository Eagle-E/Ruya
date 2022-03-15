#version 460 core

uniform vec3 lightSourceColor;

out vec4 FragColor;

void main()
{
    FragColor = vec4(lightSourceColor, 1.0f);
} 
