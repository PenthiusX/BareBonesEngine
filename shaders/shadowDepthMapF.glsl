#version 330 core

//out vec4 FragColor;
void main()
{
        //gl_FragDepth = gl_FragCoord.z;
    gl_FragColor = vec4(gl_FragCoord.z);//for debug
}
