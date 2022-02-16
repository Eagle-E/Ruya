#version 460 core
out vec4 FragColor;
in vec3 ourColor;

void main()
{
    FragColor = vec4(ourColor, 1.0f);
    //FragColor = vec4(0.2f, 0.5f, 1.0f, 1.0f);
} 




// btw.. nice color: 0.2f, 0.5f, 1.0f, 1.0f