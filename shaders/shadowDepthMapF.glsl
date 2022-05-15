#version 330 core

//out vec4 FragColor;
void main()
{
        //gl_FragDepth = gl_FragCoord.z;
    //Does not need any color info
    gl_FragColor = vec4(gl_FragCoord.z);//for debug//
}
