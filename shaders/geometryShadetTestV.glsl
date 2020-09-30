#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT {
    vec2 texCoords;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

//out vec2 TexCoords;

void main()
{
    vs_out.texCoords = vec2(aTexCoords.x,1.0 - aTexCoords.y);
    //TexCoords = aTexCoords;//vec2(aTexCoords.x,1. - aTexCoords.y);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
