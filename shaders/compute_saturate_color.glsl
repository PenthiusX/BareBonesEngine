#version 430

layout (local_size_x =16, local_size_y = 16, local_size_z = 1) in;
layout(binding=0, rgba8ui) uniform mediump uimage2D inputImage;
layout(binding=1, rgba8ui) uniform mediump uimage2D resultImage;

#include compute_basic_functions.glsl

void main()
{
    ivec2 image_pixel_cord = ivec2(gl_GlobalInvocationID.xy);

    uvec4 value = 255*imageLoad(inputImage,image_pixel_cord);

    imageStore(resultImage,image_pixel_cord,value);

}
