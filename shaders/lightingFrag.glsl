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
    float outerCutOff;
};

struct DirLight{
    vec3 position;
    //Base
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct PointLight{
    vec3 position;

    //Base
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    //
    float constant;
    float linear;
    float quadratic;
};
struct SpotLight{
    vec3 position;
    //Base
    vec3  direction;
    float cutOff;
    float outerCutOff;
};

#define NR_POINT_LIGHTS 4

//Lights
uniform Material material;
uniform Light light;
uniform PointLight pointLights[NR_POINT_LIGHTS];


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

vec3 compPointLight(PointLight pl ,vec4 diffuseTexture, vec4 specularTexture){

    // ambient
    vec3 ambient = pl.ambient * material.ambient;

    // diffuse
    vec3 norm  = normalize(Normal);
    vec3 lightDir = normalize(pl.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = pl.diffuse * diff * (diffuseTexture.xyz + material.diffuse);

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = pl.specular * spec * (specularTexture.xyz + material.specular);

    //PointLight stuff
    float lightDist = distance(pl.position , FragPos);
    float attenuation = 1.0 / (pl.constant + pl.linear * lightDist + pl.quadratic * (lightDist * lightDist));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    //Final
    return (ambient + diffuse + specular);
}

vec3 compDirectonalLight(vec4 diffuseTexture, vec4 specularTexture){

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

vec3 compSpotLight(vec4 diffuseTexture, vec4 specularTexture){

    // ambient
    vec3 ambient = light.ambient * material.ambient;
    // diffuse
    vec3 norm  = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * (diffuseTexture.xyz * material.diffuse);

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * (specularTexture.xyz * material.specular);

    // spotlight (soft edges)
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;

    //Final
    return ambient + diffuse + specular;
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
    PointLight p;

    color +=  compDirectonalLight(texColor,specColor);

    for(int i = 0; i < 2; i++){
       color +=  compPointLight(pointLights[i],texColor,specColor);
    }
    //color += compSpotLight(texColor,specColor);
    //Final color output
    FragColor = vec4(color.xyz, texColor.a);
}

