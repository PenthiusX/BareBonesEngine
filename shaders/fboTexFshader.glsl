#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec2 iMouseO;
in vec2 iMouse1;
in float iTime;

uniform sampler2D screenTexture;

//Edge detection shader
//mat3 sx = mat3(
//            1.0, 2.0, 1.0,
//            0.0, 0.0, 0.0,
//            -1.0, -2.0, -1.0
//            );
//mat3 sy = mat3(
//            1.0, 0.0, -1.0,
//            2.0, 0.0, -2.0,
//            1.0, 0.0, -1.0
//            );

void main()
{
    //shows a cursor on screen
    vec4 col = vec4(0.0,0.0,0.0,1.0);

    //Makes a screen change shaders based on mouse pos
    if(gl_FragCoord.x >= iMouseO.x){
        col = vec4(vec3(1.0 - texture2D(screenTexture, TexCoords)), 1.0);}//invertColors
    else{
        col =  texture2D(screenTexture,TexCoords) * vec4(1.0,1.0,1.0,1.0);}
    FragColor = col;

    //-----------------Storing some common usecases------------------------------
    // texture multiplied ot custom value
    //    FragColor = texture2D(screenTexture, TexCoords) * vec4(1.00,1.0,1.0,1.0);
    //---------------------------------------------------------------------------
    // Invert colors
    //    FragColor = vec4(vec3(1.0 - texture2D(screenTexture, TexCoords)), 1.0);
    //---------------------------------------------------------------------------
    // Blur
    //      FragColor = texture2D(screenTexture, TexCoords);
    //      float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
    //      FragColor = vec4(average, average, average, 1.0);
    //---------------------------------------------------------------------------
    //Edge Detection shader
    //    vec3 diffuse = texture(screenTexture, TexCoords.st).rgb;
    //    mat3 I;
    //    for (int i=0; i<3; i++) {
    //        for (int j=0; j<3; j++) {
    //            vec3 sample  = texelFetch(screenTexture, ivec2(gl_FragCoord) + ivec2(i-1,j-1), 0 ).rgb;
    //            I[i][j] = length(sample);
    //        }
    //    }
    //    float gx = dot(sx[0], I[0]) + dot(sx[1], I[1]) + dot(sx[2], I[2]);
    //    float gy = dot(sy[0], I[0]) + dot(sy[1], I[1]) + dot(sy[2], I[2]);
    //    //float g = sqrt(pow(gx, 2.0)+pow(gy, 2.0));
    //    //FragColor = vec4(diffuse - vec3(g), 1.0);
    //    float g = sqrt(pow(gx, 2.0)+pow(gy, 2.0));
    //    // Try different values and see what happens
    //    g = smoothstep(.9, .9, g);
    //    vec3 edgeColor = vec3(0., 1., 0.0);
    //    col = vec4(mix(diffuse, edgeColor, g), 1.);
    //----------------------------------------------------------------------------
}

