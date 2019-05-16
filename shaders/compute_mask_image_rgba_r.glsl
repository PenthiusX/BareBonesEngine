#version 430

layout (local_size_x =16, local_size_y = 16) in;
layout(binding=0, r8ui) uniform mediump uimage2D inputImage;
layout(binding=1, r8ui) uniform mediump uimage2D maskImage;
layout(binding=2, rgba8ui) uniform mediump uimage2D resultImage;


#define PI 3.1415926535897932384626433832795
#define FF_BY_PI 81.1690209769 //value of 255.0/pi

#include compute_basic_functions.glsl

void main()
{
       ivec2 image_pixel_cord = ivec2(gl_GlobalInvocationID.xy);

       uint mask = getImagePixel(maskImage,image_pixel_cord);

       uint outf = getImagePixel(inputImage,image_pixel_cord) * (mask / 255);

       setImagePixel(resultImage,image_pixel_cord,outf);
}
