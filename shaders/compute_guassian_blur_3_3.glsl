#version 430

layout (local_size_x =16, local_size_y = 16) in;
layout(binding=0, r8ui) uniform mediump uimage2D inputImage;
layout(binding=1, r8ui) uniform mediump uimage2D resultImage;

#include compute_basic_functions.glsl

void main()
{
       float guassian_kernal[9] = float[9](1.0/16,2.0/16,1.0/16,2.0/16,4.0/16,2.0/16,1.0/16,2.0/16,1.0/16);

       ivec2 image_pixel_cord = ivec2(gl_GlobalInvocationID.x,gl_GlobalInvocationID.y);

       float uout = compute_kernal_filter_3_3(inputImage,image_pixel_cord,guassian_kernal);

       setImagePixel(resultImage,image_pixel_cord,uint(uout));

}
