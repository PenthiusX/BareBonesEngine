#version 430

#local_size_define
layout(binding=0, r8ui) uniform mediump uimage2D resultImage;

layout( location=0 ) uniform unsigned int value;

#include compute_basic_functions.glsl


void main()
{
       ivec2 image_pixel_cord = ivec2(gl_GlobalInvocationID.xy);
       setImagePixel(resultImage,image_pixel_cord,value);
}
