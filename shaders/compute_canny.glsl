#version 430

layout (local_size_x =16, local_size_y = 16) in;
layout(binding=0, r8ui) uniform mediump uimage2D sobelMagImage;
layout(binding=1, r8ui) uniform mediump uimage2D sobelThetaImage;
layout(binding=2, r8ui) uniform mediump uimage2D outImage;

#define PI 3.1415926535897932384626433832795
#define FF_BY_PI 81.1690209769 //value of 255.0/pi

#include compute_basic_functions.glsl

void main()
{
       const ivec2 neigbour[4]=ivec2[4](
                   ivec2(1,0),
                   ivec2(1,-1),
                   ivec2(0,-1),
                   ivec2(-1,1)
                   );

       ivec2 image_pixel_cord = ivec2(gl_GlobalInvocationID.xy);

       float theta = getImagePixel(sobelThetaImage,image_pixel_cord);

       int theta_quantized = int(((theta*PI)+(PI/8.0))/(PI/4.0));//getting theta in 4 descrete levels (0,1,2,3)

       uint outf = getImagePixel(sobelMagImage,image_pixel_cord);

       if((outf > getImagePixel(sobelMagImage,image_pixel_cord+neigbour[theta_quantized])) && (outf > getImagePixel(sobelMagImage,image_pixel_cord-neigbour[theta_quantized])))
           outf = outf * 1;
       else
           outf = outf * 0;

       setImagePixel(outImage,image_pixel_cord,outf);

}
