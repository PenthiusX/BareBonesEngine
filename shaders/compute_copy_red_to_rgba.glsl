#version 430

#local_size_define
//layout (local_size_x =16, local_size_y = 16) in;
layout(binding=0, r8ui) uniform mediump uimage2D inputImage;
layout(binding=1, rgba8ui) uniform mediump uimage2D resultImage;

#include compute_basic_functions.glsl

void setImagePixel(uimage2D4x8_bindless img,ivec2 cord,uint value){
    imageStore(img, cord,uvec4(value,value,value,255));
}

void main()
{
    ivec2 image_pixel_cord = ivec2(gl_GlobalInvocationID.xy);

    uint inv = getImagePixel(inputImage,image_pixel_cord);

    setImagePixel(resultImage,image_pixel_cord,inv);

}
