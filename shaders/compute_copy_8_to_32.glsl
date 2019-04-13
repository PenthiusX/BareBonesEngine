#version 430

layout (local_size_x =16, local_size_y = 16) in;
layout(binding=0, r8ui) uniform mediump uimage2D inputImage;
layout(binding=1, r32i) uniform mediump iimage2D resultImage;

#include compute_basic_functions.glsl



void main()
{
    ivec2 image_pixel_cord = ivec2(gl_GlobalInvocationID.xy);

    int inv = 3*int(getImagePixel(inputImage,image_pixel_cord));

    setImagePixel(resultImage,image_pixel_cord,inv);

}
