#version 330 core
in vec4 iMouseO;


struct Material {
    sampler2D diffuseTex;
    sampler2D specularTex;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    //Base
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    //point Light
    float constant;
    float linear;
    float quadratic;
    //Spot
    vec3  direction;
    float cutOff;
};

uniform Material material;
uniform Light light;

//Lighting inputs
in vec3 Normal;
in vec3 FragPos;
uniform vec3 viewPos;

//Final Outcolor to the pixel
out vec4 FragColor;

//Texturing
in vec2 TexCoord;
uniform sampler2D diffuseTex;
uniform sampler2D specularTex;
//Time
uniform float time;
#define PI 3.14159265359

vec3 pointLight(vec4 diffuseTexture, vec4 specularTexture){

    float lightDist = distance(light.position , FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * lightDist + light.quadratic * (lightDist * lightDist));//PointLight

    // ambient
    vec3 ambient = light.ambient * material.ambient;

    // diffuse
    vec3 norm  = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * (diffuseTexture.xyz + material.diffuse);

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * (specularTexture.xyz + material.specular);
    //
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    //Final
    return (ambient + diffuse + specular);
}

vec3 directonalLight(vec4 diffuseTexture, vec4 specularTexture){

    // ambient
    vec3 ambient = light.ambient * material.ambient;
    // diffuse
    vec3 norm  = normalize(Normal);
    //vec3 lightDir = normalize(light.position - FragPos);
     vec3 lightDir = normalize(FragPos - light.position);//inverse light direction just for debug

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * (diffuseTexture.xyz * material.diffuse);

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * (specularTexture.xyz * material.specular);

    //Final
    return ambient + diffuse + specular ;
}

void blendingOp(vec4 mainTexture){
    if(mainTexture.a < 0.1){//discarding pixels with value below 0.1 in the alpha component.
        discard;
        //texColor.a = 0.5;
    }
}

void main()
{
    //Texture inputs
    vec4 texColor = texture2D(diffuseTex,TexCoord);
    vec4 specColor = texture2D(specularTex,TexCoord);
    //Blending
    blendingOp(texColor);
    //Light and Material inputs
    vec3 color;
    color =  directonalLight(texColor,specColor);
    color += pointLight(texColor,specColor);
    //Final color output
    FragColor = vec4(color.xyz, texColor.a);
}

