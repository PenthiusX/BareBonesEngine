#version 430

layout (local_size_x =16, local_size_y = 16) in;
layout(binding=0, r8) uniform mediump image2D inputImage;
layout(binding=1, r8) uniform mediump image2D resultImage;

#define theshold_value 0.5

float getInputImagePixel(ivec2 cord){
    return imageLoad(inputImage, cord).r;
}

#include compute_basic_functions.glsl

float compute_threshold(ivec2 image_cord)
{
    if(getInputImagePixel(image_cord) > theshold_value) return 1.0;
    else return 0.0;
}

float compute_threshold_inv(ivec2 image_cord)
{
    if(getInputImagePixel(image_cord) < theshold_value) return 1.0;
    else return 0.0;
}

void main()
{
       ivec2 image_pixel_cord = ivec2(gl_GlobalInvocationID.xy);

       float outf = compute_threshold_inv(image_pixel_cord);
       vec4 inv = vec4(outf,0.0,0.0,1.0);
       imageStore(resultImage,image_pixel_cord,inv);

}
