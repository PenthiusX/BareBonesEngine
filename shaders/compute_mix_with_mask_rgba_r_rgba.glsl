#version 430

#local_size_define
layout(binding=0, rgba8ui) uniform mediump uimage2D inputImage1;
layout(binding=1, rgba8ui) uniform mediump uimage2D inputImage2;
layout(binding=2, r8ui) uniform mediump uimage2D maskImage;
layout(binding=3, rgba8ui) uniform mediump uimage2D resultImage;

#include compute_basic_functions.glsl

uvec4 mymix(uvec4 vc1,uvec4 vc2,uint v)
{
    return ((vc1*v)+(vc2*(255-v)))/255;
}

void main()
{
       ivec2 image_pixel_cord = ivec2(gl_GlobalInvocationID.xy);

       uint mask = getImagePixel(maskImage,image_pixel_cord);

       uvec4 out1 = imageLoad(inputImage1,image_pixel_cord);
       uvec4 out2 = imageLoad(inputImage2,image_pixel_cord);

       uvec4 outv = mymix(out1,out2,mask);//(float(mask)/255.0)

       imageStore(resultImage,image_pixel_cord,outv);
       //iauscrynoaigtcnogucor7tgnr
}
