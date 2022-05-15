#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
//Skeletal Animation in
layout (location = 3) in ivec4 BoneIDs;
layout (location = 4) in vec4 Weights;

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

void main()
{
    vec4 c = vec4(normalize(vec3(Weights.xyz)).xyz,1.0);//debug

    mat4 mvpx = projection * view * model;
    gl_Position =  mvpx * vec4(aPos.xyz, 1.0);

    mat4 BoneTransform;
        for(int i = 0 ; i < 4 ; i++){
            float v = Weights[i];
            if(Weights[i] == -1){
                continue;
            }
            BoneTransform += gBones[BoneIDs[i]] * v;
        }
    vec4 bonetransformPos = BoneTransform * vec4(aPos, 1.0);
    gl_Position = projection * view * model * bonetransformPos;

    ourColor = vec4(c.xyz,1.0);
    iMouseO = iMouse;
    DEPTH = gl_Position.z / 50.0;
}
