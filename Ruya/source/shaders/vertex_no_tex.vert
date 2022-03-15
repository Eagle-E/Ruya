#version 460 core
layout (location = 0) in vec3 pos;

uniform mat4 MVP;

out vec4 ourColor;

void main()
{
    gl_Position = MVP * vec4(pos.x, pos.y, pos.z, 1.0);
    ourColor = vec4(0.2f, 0.5f, 1.0f, 1.0f);
}