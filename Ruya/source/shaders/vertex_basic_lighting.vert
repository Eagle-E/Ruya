#version 460 core

layout (location = 0) in vec3 vertexLocalPos; // coordinate of vertex in local space of its obj
layout (location = 1) in vec3 inpNormal;
layout (location = 2) in vec2 texCoords;

uniform mat4 MVP;
uniform mat4 ModelMat;

out vec3 normal;
out vec3 fragPosition;

void main()
{
    gl_Position = MVP * vec4(vertexLocalPos, 1.0);
    fragPosition = vec3(ModelMat * vec4(vertexLocalPos, 1.0));
    normal = mat3(transpose(inverse(ModelMat))) * inpNormal;
}