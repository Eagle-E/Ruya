#version 460 core
in vec4 ourColor;
uniform vec3 objColor;
uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;

out vec4 FragColor;

void main()
{
    FragColor = ourColor;
} 
