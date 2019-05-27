#version 430

layout (local_size_x =16, local_size_y = 16, local_size_z = 1) in;
layout(binding=0, r8ui) uniform mediump uimage2D inputImage;
layout(binding=1, r8ui) uniform mediump uimage2D resultImage;

layout( location=0 ) uniform unsigned int theshold_value;

#include compute_basic_functions.glsl

uint compute_threshold(uimage2D1x8_bindless img,ivec2 image_cord)
{
    if(getImagePixel(img,image_cord) > theshold_value) return 255;
    else return 0;
}

uint compute_threshold_inv(uimage2D1x8_bindless img,ivec2 image_cord)
{
    if(getImagePixel(img,image_cord) < theshold_value) return 255;
    else return 0;
}

uint compute_invert(uimage2D1x8_bindless img,ivec2 image_cord)
{
    return 255 - getImagePixel(img, image_cord);
}

void main()
{
    ivec2 image_pixel_cord = ivec2(gl_GlobalInvocationID.xy);

    uint thres = compute_threshold(inputImage,image_pixel_cord);

    setImagePixel(resultImage,image_pixel_cord,thres);

}
