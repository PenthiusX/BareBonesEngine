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
uniform bool hasAnimation;

//Geometry shader
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
    //Model view projection combined
    mat4 mvpx = projection * view * model;
    gl_Position =  mvpx * vec4(aPos.xyz, 1.0);

    //get the Vertex fragment positons, for Vertex lighting
    FragPos = vec3(model * vec4(aPos, 1.0));

    //apply the transforms applied on the model position data to update the normal data as well
    //Normal = mat3(transpose(inverse(model))) * normal;//why do ths????????
    Normal  = normalize(mat3(model) * normal);//for lighting calcs

    // if the relevatn entity has animations only then mul the bone tranfroms to the orignal matrices
    //else if no animation is presesnet the null anim transform data will cause issue with the entites
    //with no animation information.
    if(hasAnimation){
        mat4 boneTransform;
            for(int i = 0 ; i < 4 ; i++){
                float v = Weights[i];
                if(Weights[i] == -1){continue;}
                boneTransform += gBones[BoneIDs[i]] * v;
            }
            mat4x4 animTransformed = model * boneTransform;
        // Overrite the vertex positions wit the relevant animation tranfroms applied.
        gl_Position = projection * view * animTransformed * vec4(aPos, 1.0);
        // Also update the Normals for correct lighting wrt the tranforming/animating entity.
        Normal = normalize(mat3(animTransformed) * normal);
        //Update the info for Vertex Ligting as well.
        FragPos = vec3(animTransformed * vec4(aPos, 1.0));
    }

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
