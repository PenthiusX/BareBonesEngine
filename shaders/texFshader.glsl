#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
    FragColor = texture2D(screenTexture, TexCoords) * vec4(0.0,0.5,0.0,1.0);
}
