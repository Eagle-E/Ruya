#version 460 core

uniform vec3 objColor;

out vec4 FragColor;
in vec3 fragPositionInObjSpace;
in vec3 normalInLocalSpace;
void main()
{
    FragColor = vec4(objColor, 1.0f);
} 
