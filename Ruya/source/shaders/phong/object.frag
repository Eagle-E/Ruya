#version 460 core

uniform vec3 objColor;
uniform vec3 lightColor;
uniform vec3 lightPosInObjSpace;
uniform vec3 cameraPosInObjSpace;
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
    vec3 lightDir = normalize(fragPositionInObjSpace - lightPosInObjSpace);
    float diff = max(dot(-lightDir, norm), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular component
    float specularStrength = 0.5;
    vec3 viewDir = normalize(fragPositionInObjSpace - cameraPosInObjSpace);
    vec3 reflectionDir = reflect(lightDir, norm);
    float specular = pow(max(dot(reflectionDir, -viewDir), 0.0), 32);

    // resulting fragment color
    vec3 result = (ambient + diffuse + specular) * objColor;
    FragColor = vec4(result, 1.0);

} 


/*
    
    */