#version 330 core

const int MAX_JOINTS = 50;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;


////for geometry shader
//out VS_OUT {
//    vec2 texCoords;
//} vs_out;

//Lighting Calcs out
out vec3 Normal;
out vec3 FragPos;

//MVP transform info // in
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//Bone transforms //in
uniform mat4 jointTransforms[MAX_JOINTS];

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
    gl_Position =  mvpx * vec4(aPos, 1.0);

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
