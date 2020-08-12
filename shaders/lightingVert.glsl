#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
//
uniform vec4 aColor;//in
out vec4 ourColor;//to fragment
//
uniform vec2 iMouse;//in
out vec2 iMouseO;//to fragment


void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * normal;  //apply model transformation on the normal info

    ourColor = aColor;
    mat4 mvpx = projection * view * model;
    gl_Position =  mvpx * vec4(aPos, 1.0);
}
