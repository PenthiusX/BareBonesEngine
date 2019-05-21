#version 330 core
layout (location = 0) in vec3 tPos;

uniform sampler2D wrapTexture;

out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 mvp;

#define PI 3.1415926535897932384626433832795

void main()
{
    float r = 2.94 * texture(wrapTexture,tPos.xy).r;
    float theta = 2 * PI * tPos.x;
    vec3 aPos = vec3(r*cos(theta),r*sin(theta),tPos.y);
    //gl_Position = vec4(-aPos.x,1.0-aPos.z,aPos.y, 1.0);
    FragPos = vec3(-aPos.x,4*(1.0-aPos.z),aPos.y);
    gl_Position = projection * view * model * vec4(-aPos.x,4*(1.0-aPos.z),aPos.y, 1.0);


    //gl_Position = projection * view * model * vec4(aPos, 1.0);
};
