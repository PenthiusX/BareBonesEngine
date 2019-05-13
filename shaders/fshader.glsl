#version 330 core
out vec4 FragColor;
in vec4 ourColor;
in vec4 iMouse;
in float DEPTH;

void main()
{
       FragColor =  ourColor * vec4(DEPTH,DEPTH,DEPTH,1.0);

}

