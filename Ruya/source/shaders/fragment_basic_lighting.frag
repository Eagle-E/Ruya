#version 460 core

uniform vec3 objColor;
uniform vec3 lightColor;
uniform vec3 LightPosInObjSpace;
in vec3 fragPositionInObjSpace;
in vec3 normalInLocalSpace;

out vec4 FragColor;

void main()
{
    // ambient color
    float ambientStrength = 0.15;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse color
    vec3 norm = normalize(normalInLocalSpace);
    vec3 lightDir = normalize(LightPosInObjSpace - fragPositionInObjSpace);
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = diff * lightColor;

    // resulting fragment color
    vec3 result = (ambient + diffuse) * objColor;
    FragColor = vec4(result, 1.0);

} 
