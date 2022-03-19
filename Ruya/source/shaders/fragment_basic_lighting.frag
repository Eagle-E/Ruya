#version 460 core

uniform vec3 objColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
in vec3 normal;
in vec3 fragPosition;

out vec4 FragColor;

void main()
{
    // ambient color
    float ambientStrength = 0.15;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse color
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPosition - fragPosition);
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objColor;
    FragColor = vec4(result, 1.0);
} 
