#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    vec4 fragColor = texture2D(ourTexture, TexCoord);
    FragColor.rgb = fragColor.rrr;
    FragColor.a = 1.0;
}
