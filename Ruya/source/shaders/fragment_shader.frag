#version 460 core
in vec3 ourColor;
in vec2 ourTexCoord;
uniform sampler2D ourTexture;

out vec4 FragColor;

void main()
{
    FragColor = texture(ourTexture, ourTexCoord);
    //FragColor = vec4(0.2f, 0.5f, 1.0f, 1.0f);
} 




// btw.. nice color: 0.2f, 0.5f, 1.0f, 1.0f