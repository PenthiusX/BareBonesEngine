#version 330 core

in vec3 FragPos;

out vec4 FragColor;

void main()
{

    vec4 color = vec4(0.8,0.6,0.1,1.0) * (1.0-gl_FragCoord.z);

    FragColor = vec4(color.xyz, 0.8);
}
