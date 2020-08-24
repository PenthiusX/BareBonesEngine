#version 330 core
in vec4 iMouseO;

//Lighting inputs
in vec3 Normal;
in vec3 FragPos;
uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float ambientStrength;
uniform float specularStrength;

//Final Outcolor to the pixel
out vec4 FragColor;

//Texturing
in vec2 TexCoord;
uniform sampler2D ourTexture;

void main()
{
    // ambient
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), .0), 32);
    vec3 specular = specularStrength * spec * lightColor;
        
    vec3 result = (ambient + diffuse + specular);// * ourColor.xyz;


    //discarding pixels with value below 0.1 in the alpha component.
    vec4 texColor = texture2D(ourTexture,TexCoord);
    if(texColor.a < 0.1){
        discard;
    }

    //Final color output
    FragColor = vec4(result.xyz, 1.0) *  texColor;
}

