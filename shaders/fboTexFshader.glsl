#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
    FragColor = texture2D(screenTexture, TexCoords) * vec4(0.5,0.5,0.5,1.0);
//---------------------------------------------------------------------------
//    FragColor = vec4(vec3(1.0 - texture2D(screenTexture, TexCoords)), 1.0);
//---------------------------------------------------------------------------
//      FragColor = texture2D(screenTexture, TexCoords);
//      float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
//      FragColor = vec4(average, average, average, 1.0);
//---------------------------------------------------------------------------
}
