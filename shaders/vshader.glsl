#version 330 core
layout (location = 0) in vec3 aPos;
uniform vec4 aColor;
uniform mat4 mvp;
//
out vec4 ourColor; 

void main()
{
    gl_Position =  vec4(aPos, 1.0) * mvp;
	ourColor = aColor;
}
