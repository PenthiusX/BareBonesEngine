#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
out vec2 iMouse;

void main()
{
    gl_Position = vec4(aPos.xy, 0.0, 1.0);
    TexCoords = aTexCoords;
    iMouse = vec2(500.0,500.0);
}
