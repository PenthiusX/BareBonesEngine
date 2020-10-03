#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 aTexCoords;

//if using a Geometry shader
//all varibles need to be passed through it
//direct access via unifroms or in out / variying to the fragment shader
//get blocked if geometry shader is attached to the shaderprogram instance.
out VS_OUT {
    vec2 gTexCoords;
    vec3 gNormals;
}vs_out;
//
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    //mat4 normalMatrix = mat4(mat3(transpose(inverse(model))));
    vs_out.gNormals = normalize(projection * view * model * vec4(aNormals,0.0)).xyz;
    vs_out.gTexCoords = aTexCoords;
}
