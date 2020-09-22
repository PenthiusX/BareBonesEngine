#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;

uniform mat4 view;//this is the dir lights position
uniform mat4 projection;//this is set as ortho
uniform mat4 orthoProjection;//this is set as ortho

void main()
{
    mat4 lightSpaceMatrix = orthoProjection * view;//from the Lights perspective capture FBO
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
    //The position will be sent for vertexoperation and rasterisation ,
    //this will generate the raster,and the relative depthMap implicitly as well.
    //This will be captured in the FBO.
    //We dont need to asign any color to the buffer hence the fragment shader can be left empty

    //The FBO texture wich will have the depth attachment and no color attachemnt will now be passed
    //into the Regular scene object's shader for use in the Shadow shading calculations
}
