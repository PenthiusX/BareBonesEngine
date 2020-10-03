//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
#version 330 core
//-----------------------------------------
in vec4 iMouseO;
//-----------------------------------------
struct Material {
    sampler2D diffuseTex;
    sampler2D specularTex;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
//-----------------------------------------
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
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    //Spot
    vec3  direction;
    float cutOff;
    float outerCutOff;
};
//-----------------------------------------
//Lights
uniform Light light;
uniform SpotLight spot;

uniform bool blinn;

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
//-----------------------------------------
//Lighting inputs
in vec3 Normal;
in vec3 FragPos;
uniform vec3 viewPos;
//-----------------------------------------
//Final Outcolor to the pixel
out vec4 FragColor;
//-----------------------------------------
//Texturing
in vec2 TexCoord;
in vec2 TexCoords;
uniform sampler2D diffuseTex;//1
uniform sampler2D specularTex;//2
uniform sampler2D bumpTex;//3 //not in use yet
//-----------------------------------------
uniform Material material;
//-----------------------------------------
//Shadow
uniform sampler2D shadowDepthTex;//4
in vec4 fragPosLightSpace;
//-----------------------------------------
//Time
uniform float time;
#define PI 3.14159265359
//---------------------------------------------------------------------------------------
float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture2D(shadowDepthTex, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowDepthTex, 0);
    for(int x = -1; x <= 1; ++x){
        for(int y = -1; y <= 1; ++y){
            float pcfDepth = texture2D(shadowDepthTex, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}
//---------------------------------------------------------------------------------------
vec3 compPointLight(PointLight pl ,vec4 diffuseTexture, vec4 specularTexture){

    // ambient
    vec3 ambient = pl.ambient * material.ambient;

    // diffuse
    vec3 norm  = normalize(Normal);
    vec3 lightDir = normalize(pl.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = pl.diffuse * diff * (diffuseTexture.xyz + material.diffuse);

    //Binn
    //  halfwayDir = normalize(lightDir + viewDir);
    //  specular = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
    //Blin-Phong
    //  reflectDir = reflect(-lightDir, norm);
    //  specular = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);

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
//---------------------------------------------------------------------------------------
vec3 compDirectonalLight(vec4 diffuseTexture, vec4 specularTexture){

    // ambient
    vec3 ambient = light.ambient * material.ambient;
    // diffuse
    vec3 norm  = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
     //vec3 lightDir = normalize(FragPos - light.position);//inverse light direction just for debug

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * (diffuseTexture.xyz * material.diffuse);

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * (specularTexture.xyz * material.specular);
    //shadow
    float sd = ShadowCalculation(fragPosLightSpace);
    //Final
    return (ambient + (1.0 - sd)) * (diffuse + specular) ;
}
//---------------------------------------------------------------------------------------
vec3 compSpotLight(SpotLight spt,vec4 diffuseTexture, vec4 specularTexture){

    // ambient
    vec3 ambient = spt.ambient * material.ambient;
    // diffuse
    vec3 norm  = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = spt.diffuse * diff * (diffuseTexture.xyz * material.diffuse);

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spt.specular * spec ;//* (specularTexture.xyz * material.specular);

    // spotlight (soft edges)
    float theta = dot(lightDir, normalize(-spt.direction));
    float epsilon = (spt.cutOff - spt.outerCutOff);
    float intensity = clamp((theta - spt.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;

    //Final
    return ambient + diffuse + specular;

}
//---------------------------------------------------------------------------------------
void blendingOp(vec4 mainTexture){
    if(mainTexture.a < 0.1){//discarding pixels with value below 0.1 in the alpha component.
        discard;
        //texColor.a = 0.5;
    }
}
//---------------------------------------------------------------------------------------
void main()
{
    //Texture inputs
    vec4 texColor = texture2D(diffuseTex,TexCoord);
    vec4 specColor = texture2D(specularTex,TexCoord);
    //Blending
    blendingOp(texColor);
    //Light and Material inputs
    vec3 color;

    color +=  compDirectonalLight(texColor,specColor);//finalise seperate class
    for(int i = 0; i < 2; i++){color +=  compPointLight(pointLights[i],texColor,specColor);}
    color += compSpotLight(spot,texColor,specColor);//finalise sperate class

    FragColor = vec4(color.xyz, texColor.a);//Final color output
//    FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
    //FragColor = texture2D(shadowDepthTex,TexCoord);
    //FragColor = texture2D(diffuseTex,TexCoords);
}
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
