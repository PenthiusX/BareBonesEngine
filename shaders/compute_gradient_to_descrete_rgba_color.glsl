#version 430

#local_size_define
layout(binding=0, r8ui) uniform mediump uimage2D sobelMagImage;
layout(binding=1, r8ui) uniform mediump uimage2D sobelThetaImage;
layout(binding=2, rgba8ui) uniform mediump uimage2D resultImage;

#define PI 3.1415926535897932384626433832795
#define FF_BY_PI 81.1690209769 //value of 255.0/pi

#include compute_basic_functions.glsl

void main()
{
       const uvec4 descrete_color[4]=uvec4[4](
                   uvec4(1,0,0,1),
                   uvec4(0,1,0,1),
                   uvec4(0,0,1,1),
                   uvec4(1,1,0,1)
                   );

       ivec2 image_pixel_cord = ivec2(gl_GlobalInvocationID.xy);

       float theta = getImagePixel(sobelThetaImage,image_pixel_cord);

       int theta_quantized = int(theta/64.0);//getting theta in 4 descrete levels (0,1,2,3)

       uvec4 outf = descrete_color[theta_quantized]*getImagePixel(sobelMagImage,image_pixel_cord);

       outf.a=255;

       imageStore(resultImage,image_pixel_cord,outf);

}
