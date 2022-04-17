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

////for geometry shader
//out VS_OUT {
//    vec2 texCoords;AA
//} vs_out;

//Lighting Calcs out
out vec3 Normal;
out vec3 FragPos;

//MVP transform info // in
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//shadow info
uniform mat4 shadowLightSpace;//dirLightOrthoProjMatrix * lightViewMatrix , V*P 
out vec4 fragPosLightSpace;
//Color
uniform vec4 aColor;//in
out vec4 ourColor;//to fragment
//Texturing
out vec2 TexCoord;
//MouseInputs //not utilised WIP
uniform vec2 iMouse;//in
out vec2 iMouseO;//to fragment


void main()
{	
    //Skeletal Animation
        vec4 totalPosition = vec4(0.0f);
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        if(boneIds[i] == -1)
            continue;
        if(boneIds[i] >=MAX_BONES)
        {
            totalPosition = vec4(aPos,1.0f);
            break;
        }
        vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(aPos,1.0f);
        totalPosition += localPosition * weights[i];
        vec3 localNormal = mat3(finalBonesMatrices[boneIds[i]]) * normal;
    }

    //Model view projection combined
    mat4 mvpx = projection * view * model;
    gl_Position =  mvpx * vec4(aPos.xyz, 1.0);
    // gl_Position =  mvpx * vec4(totalPosition.xyz, 1.0);

    //get the Vertex fragment positons, for Vertex lighting
    FragPos = vec3(model * vec4(aPos, 1.0));
    //apply the transforms applied on the model position data to update the normal data as well

    //Normal = mat3(transpose(inverse(model))) * normal;//why do ths????????
    Normal  = normalize(mat3(model) * normal);//for lighting calcs

    //For shadows
    fragPosLightSpace = shadowLightSpace * vec4(FragPos, 1.0);
    //
    ourColor = aColor;//not being used in this instance
    //Default uv cords for model editors that match coordinate order with opengl
    //TexCoord = uv;
    //if BlenderAssets wich have oposite coord order , force flip of the uV coords
    TexCoord = vec2(uv.x,1.-uv.y);
    //vs_out.texCoords = uv;
}
