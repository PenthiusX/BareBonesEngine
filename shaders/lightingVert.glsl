#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
//
uniform mat4 mvp;
//
out vec4 ourColor;

void main()
{
    mat4 mvpx = projection * view * model;
    gl_Position =  mvpx * vec4(aPos, 1.0);
    ourColor = aColor;

        FragPos = vec3(model * vec4(position, 1.0f));
        Normal = mat3(transpose(inverse(model))) * normal;
}
