#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;//Xnot needed for debug
uniform mat4 projection;//X

//Animate the Shadows
//Skeletal Animation in
layout (location = 3) in ivec4 boneIDs;
layout (location = 4) in vec4 weights;
//Skeletal Animation Bone transforms //in
const int MAX_BONES = 100;
uniform mat4 gBones[MAX_BONES];
uniform bool hasAnimation;


uniform mat4 shadowLightSpace;//orthoProjection matrix * viewMat from light pos

void main()
{
    //The shadowLightSpace is the VP from the LightSource perspective.
    gl_Position = shadowLightSpace * model * vec4(aPos, 1.0);
  //gl_Position = projection * view * model * vec4(aPos, 1.0);

    if(hasAnimation){
        mat4 boneTransform;
            for(int i = 0 ; i < 4 ; i++){
                float w = weights[i];
                if(weights[i] == -1){continue;}
                boneTransform += gBones[boneIDs[i]] * w;
            }
        // Overrite the vertex positions wit the relevant animation tranfroms applied.
        gl_Position = shadowLightSpace * model * boneTransform * vec4(aPos, 1.0);
    };


    //The position will be sent for vertexoperation and rasterisation ,
    //this will generate the raster,and the relative depthMap implicitly as well.
    //This will be captured in the FBO.
    //We dont need to asign any color to the buffer hence the fragment shader can be left empty

    //The FBO texture wich will have the depth attachment and no color attachemnt will now be passed
    //into the Regular scene object's shader for use in the Shadow shading calculations
}
