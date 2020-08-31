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
    vec4 lightType;
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

//Lighting inputs
in vec3 Normal;
in vec3 FragPos;
uniform vec3 viewPos;
uniform float ambientStrength;

//Final Outcolor to the pixel
out vec4 FragColor;

//Texturing
in vec2 TexCoord;
uniform sampler2D diffuseTex;
uniform sampler2D specularTex;

void main()
{
    //--------------------------------------------------------------
    vec4 texColor = texture2D(diffuseTex,TexCoord);
    vec4 specColor = texture2D(specularTex,TexCoord);

    if(texColor.a < 0.1){//discarding pixels with value below 0.1 in the alpha component.
        discard;
        //texColor.a = 0.5;
    }
    //--------------------------------------------------------------
    float lightDist = distance(light.position , FragPos);//PointLight , Remove this for directonal light

    // ambient
    vec3 ambient = light.ambient * texColor.xyz * ambientStrength;

    // diffuse
    vec3 norm  = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texColor.xyz;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * specColor.xyz * material.shininess;

    //Final
    vec3 result = (ambient + diffuse + specular) / (lightDist) ;

    //Final color output
    FragColor = vec4(result.xyz, texColor.a);
    //--------------------------------------------------------------
}

