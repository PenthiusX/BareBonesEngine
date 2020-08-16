#version 330 core
in vec4 iMouseO;

in vec3 Normal;
in vec3 FragPos;
in vec4 ourColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

uniform float ambientStrength;
uniform float specularStrength;

out vec4 FragColor;

void main()
{
    // ambient
    // ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(FragPos - lightPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;
    
    // specular
    // specularStrength = 2.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), .0), 32);
    vec3 specular = specularStrength * spec * lightColor;
        
    vec3 result = (ambient + diffuse + specular) * ourColor.xyz;

    FragColor = vec4(result.xyz, 1.0);
//  FragColor = vec4(result.xyz, 0.5) + ourColor;
//  FragColor = ourColor.xyz + vec3(0.0,0.0,0.5);
}

