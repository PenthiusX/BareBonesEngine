#version 330 core
layout (location = 0) in vec3 aPos;
//
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
//
uniform vec4 aColor;
out vec4 ourColor;
//
uniform vec2 iMouse;
out vec2 iMouseO;
//
out float DEPTH ;

void main()
{
    mat4 mvpx = projection * view * model;
    gl_Position =  mvpx * vec4(aPos, 1.0);

    //vec4 ndc = mvpx * vec4(aPos, 1);
    //float sz = ndc.w;

    //gl_Position= mvpx * vec4(aPos.xy * sz, aPos.z, 1.0);

    //gl_FragDepth = 0.99;

    ourColor = aColor;
    iMouseO = iMouse;
    DEPTH = gl_Position.z / 50.0;
}
