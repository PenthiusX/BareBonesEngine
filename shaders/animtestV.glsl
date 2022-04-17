#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
//Skeletal Animation in
layout(location = 3) in ivec4 boneIds;
layout(location = 4) in vec4 weights;

//Skeletal Animation Bone transforms //in
const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];
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
    //Skeletal Animation
    vec4 totalPosition = vec4(0.0f);
    vec4 c = vec4(0.0f);
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        if(boneIds[i] == -1){
            continue;
        }
        if(boneIds[i] >=MAX_BONES){
            totalPosition = vec4(aPos,1.0f);
            break;
        }

        //debug
        c = finalBonesMatrices[boneIds[i]] * vec4(normalize(weights).xyz,1.0);
//        c = vec4(normalize(boneIds).xyz,1.0);

        vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(aPos,1.0f);
        totalPosition += localPosition * weights[i];
        vec3 localNormal = mat3(finalBonesMatrices[boneIds[i]]) * normal;
    }

    mat4 mvpx = projection * view * model;
    gl_Position =  mvpx * vec4(totalPosition.xyz, 1.0);

    //vec4 ndc = mvpx * vec4(aPos, 1);
    //float sz = ndc.w;

    //gl_Position= mvpx * vec4(aPos.xy * sz, aPos.z, 1.0);

    //gl_FragDepth = 0.99;

    ourColor = vec4(c.xyz,1.0);
    iMouseO = iMouse;
    DEPTH = gl_Position.z / 50.0;
}
