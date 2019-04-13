#version 430

layout (local_size_x =16, local_size_y = 16) in;
layout(binding=0, r8ui) uniform mediump uimage2D inputImage;
layout(binding=1, r8ui) uniform mediump uimage2D resultImage;

#include compute_basic_functions.glsl

void main()
{
       const float guassian_kernal[25] = float[25](1.0/273.0,4.0/273.0,7.0/273.0,4.0/273.0,1.0/273.0,4.0/273.0,16.0/273.0,26.0/273.0,16.0/273.0,4.0/273.0,7.0/273.0,26.0/273.0,41.0/273.0,26.0/273.0,7.0/273.0,4.0/273.0,16.0/273.0,26.0/273.0,16.0/273.0,4.0/273.0,1.0/273.0,4.0/273.0,7.0/273.0,4.0/273.0,1.0/273.0);

       ivec2 image_pixel_cord = ivec2(gl_GlobalInvocationID.x,gl_GlobalInvocationID.y);

       uint uout = uint(compute_kernal_filter_5_5(inputImage,image_pixel_cord,guassian_kernal));

       setImagePixel(resultImage,image_pixel_cord,uout);

}
