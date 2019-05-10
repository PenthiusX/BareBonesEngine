#version 330 core
out vec4 FragColor;
in vec4 ourColor;
in vec4 iMouse;

void main()
{
    vec4 col;
    float zw = gl_FragCoord.z / gl_FragCoord.w;
    col = vec4(zw,zw,zw,1.0);

    FragColor =  col;
}

