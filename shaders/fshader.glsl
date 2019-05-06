#version 330 core
out vec4 FragColor;
in vec4 ourColor;
in vec4 iMouse;

void main()
{
    FragColor = ourColor;
}

