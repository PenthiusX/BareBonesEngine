#version 330 core

flat in vec3 FragPos;

out vec4 FragColor;

void main()
{
    vec3 lightPos = vec3(-3.0,-3.0,-4.0);
    vec3 viewPos = vec3(0.0,0.0,-4.0);

    vec3 norm = normalize(-FragPos-0.5*noise3(FragPos.x+FragPos.y+FragPos.z));

    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);

    vec3 diffuse = diff * vec3(0.6,0.6,0.6);

    vec3 lightColor = vec3(0.8,0.8,0.8);

    float specularStrength = 0.5;

    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1);

    vec3 specular = specularStrength * spec * lightColor;

    vec4 color = vec4(0.8,0.8,0.8,0.6);
    //color=vec4(newTexture[1]*0.607,newTexture[1]*0.607,newTexture[1]*0.61,0.4);
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength*vec3(1.0,1.0,1.0) ;

    vec3 result = (ambient + diffuse + specular) * color.xyz;

    FragColor = vec4(result, 0.8);
}
