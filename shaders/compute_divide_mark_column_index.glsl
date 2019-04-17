#version 430

//layout (local_size_x =17, local_size_y = 8,local_size_z = 5) in;
//layout (local_size_x =16, local_size_y = 16) in;
#local_size_define
layout(binding=0, r32i) uniform mediump iimage2D inputImage;
layout(binding=1, r32i) uniform mediump iimage2D inputImage2;
layout(binding=2, r8ui) uniform mediump uimage2D resultImage;

layout( location=0 ) uniform unsigned int imageWidth;
layout( location=1 ) uniform unsigned int imageHeight;

#include compute_basic_functions.glsl

ivec2 getPixelCord(){
    return ivec2(0,gl_LocalInvocationIndex);
}

void main()
{
       ivec2 image_pixel_cord = getPixelCord();
       //ivec2 image_pixel_cord = ivec2(gl_GlobalInvocationID.xy);

       int a = getImagePixel(inputImage,image_pixel_cord);
       int b = getImagePixel(inputImage2,image_pixel_cord);

       setImagePixel(resultImage,ivec2((a/b)+(imageWidth/2),image_pixel_cord.y),255);
}
