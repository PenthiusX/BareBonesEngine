#version 430

layout (local_size_x =16, local_size_y = 16) in;
layout(binding=0, r32i) uniform mediump iimage2D inputImage;
layout(binding=1, r8ui) uniform mediump uimage2D resultImage;

layout( location=0 ) uniform unsigned int devide_value;

#include compute_basic_functions.glsl

void main()
{
    ivec2 image_pixel_cord = ivec2(gl_GlobalInvocationID.xy);

    uint inv = getImagePixel(inputImage,image_pixel_cord)/devide_value;

    setImagePixel(resultImage,image_pixel_cord,inv);

}
