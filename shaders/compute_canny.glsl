#version 430

layout (local_size_x =16, local_size_y = 16) in;
layout(binding=0, r8) uniform mediump image2D inputImage;
layout(binding=1, r8) uniform mediump image2D inputImage2;
layout(binding=2, r8) uniform mediump image2D resultImage;


float getInputImagePixel(ivec2 cord){
    return imageLoad(inputImage, cord).r;
}

float getInputImagePixel2(ivec2 cord){
    return imageLoad(inputImage2, cord).r;
}


void main()
{

       const float PI = 3.1415926535897932384626433832795;

       const ivec2 neigbour[4]=ivec2[4](
                   ivec2(1,0),
                   ivec2(1,-1),
                   ivec2(0,-1),
                   ivec2(-1,1)
                   );

       ivec2 image_pixel_cord = ivec2(gl_GlobalInvocationID.xy);

       float l2gradienttheta = getInputImagePixel2(image_pixel_cord);

       int outt = int(((l2gradienttheta*PI)+(PI/8.0))/(PI/4.0));

       float outf = getInputImagePixel(image_pixel_cord);

       if((outf > getInputImagePixel(image_pixel_cord+neigbour[outt])) && (outf > getInputImagePixel(image_pixel_cord-neigbour[outt])))
           outf = outf * 1.0;
       else
           outf = outf * 0.0;

       vec4 inv = vec4(outf,0.0,0.0,1.0);
       imageStore(resultImage,image_pixel_cord,inv);

}
