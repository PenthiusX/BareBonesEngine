#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
//Skeletal Animation in
layout (location = 3) in ivec4 BoneIDs;
layout (location = 4) in vec4 Weights;

// layout (location = 5) in ivec4 mBoneIDs;
// layout (location = 6) in vec4 mWeights;

//Skeletal Animation Bone transforms //in
const int MAX_BONES = 100;
uniform mat4 gBones[MAX_BONES];
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

//const vec4 colorAr = vec4[3](vec4(1,0,0,1),vec4(0,1,0,1),vec4(0,0,1,1));

void main()
{
    //Skeletal Animation
    //debug
    vec4 c = vec4(normalize(vec3(Weights.xyz)).xyz,1.0);//debug
    //vec4 c = vec4(normalize(aPos).xyz,0.5);//debug
    // c = finalBonesMatrices[boneIds[i]] * vec4(normalize(weights).xyz,1.0);
    // c += vec4(normal.xyz,1.0);

    mat4 mvpx = projection * view * model;
    gl_Position =  mvpx * vec4(aPos.xyz, 1.0);


    mat4 BoneTransform;// = gBones[BoneIDs[0]] * Weights[0];
        // BoneTransform += gBones[BoneIDs[1]] * Weights[1];
        // BoneTransform += gBones[BoneIDs[2]] * Weights[2];
        // BoneTransform += gBones[BoneIDs[3]] * Weights[3];

        for(int i = 0 ; i < 4 ; i++){
            float v = Weights[i];
            if(Weights[i] == -1){
                v = 0.0;
            }
            BoneTransform += gBones[BoneIDs[i]] * v;
        }
    vec4 bonetransformPos = BoneTransform * vec4(aPos, 1.0);
    gl_Position = projection * view * model * bonetransformPos;
    c += vec4(normalize((mvpx * bonetransformPos)).xyz,1.0);


    //vec4 ndc = mvpx * vec4(aPos, 1);
    //float sz = ndc.w;

    //gl_Position= mvpx * vec4(aPos.xy * sz, aPos.z, 1.0);
    //gl_FragDepth = 0.99;

    ourColor = /*vec4(aColor.xyz,1.0) +*/ vec4(c.xyz,1.0);
    iMouseO = iMouse;
    DEPTH = gl_Position.z / 50.0;
}
