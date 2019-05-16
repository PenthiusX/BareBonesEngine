#version 430

#local_size_define
layout(binding=0, r8ui) uniform mediump uimage2D resultImage;

#include compute_basic_functions.glsl

layout( location=0 ) uniform ivec4 region;

void main()
{

    ivec2 image_pixel_cord = ivec2(gl_GlobalInvocationID.xy);

    uint outv = 0;

    if(is_inside(image_pixel_cord,region.xy,region.zw))
        outv=255;

    setImagePixel(resultImage,image_pixel_cord,outv);

}
