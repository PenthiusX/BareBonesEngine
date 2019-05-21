#version 330 core

in vec3 FragPos;

out vec4 FragColor;


void main()
{
    vec3 lightPos = vec3(4,4,-3);

    vec3 norm = normalize(FragPos);

    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(0.5,0.5,0.5);

    vec4 color = vec4(0.8,0.6,0.1,0.6);

    float ambientStrength = 0.7;
    vec3 ambient = ambientStrength*vec3(1.0,1.0,1.0) ;

    vec3 result = (ambient + diffuse) * vec3(color[0],color[1],color[2]);

    FragColor = vec4(result, 0.5);
}
