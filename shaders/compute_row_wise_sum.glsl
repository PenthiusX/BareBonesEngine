#version 430

//layout (local_size_x =17, local_size_y = 8,local_size_z = 5) in;
//layout (local_size_x =16, local_size_y = 16) in;
#local_size_define

layout(binding=0, r8ui) uniform mediump uimage2D inputImage;
layout(binding=1, r32i) uniform mediump iimage2D resultImage;

layout( location=0 ) uniform unsigned int levels;
layout( location=1 ) uniform unsigned int imageWidth;
layout( location=2 ) uniform unsigned int imageHeight;

#include compute_basic_functions.glsl

ivec2 getPixelCord(){

    return ivec2(gl_LocalInvocationIndex,dot(uvec3(1,gl_NumWorkGroups.x,gl_NumWorkGroups.x*gl_NumWorkGroups.y),gl_WorkGroupID));
}

bool is_inside(ivec2 val, ivec2 lower, ivec2 upper)
{
   return ((val.x > lower.x ) && (val.y > lower.y ) && (val.x < upper.x ) && (val.y < upper.y ));
}

uint combine(uint p,uint q)
{
    return (65536*p)+q;
}

void main()
{
       ivec2 image_pixel_cord = getPixelCord();
       image_pixel_cord.x=image_pixel_cord.x*2;
       //ivec2 image_pixel_cord = ivec2(gl_GlobalInvocationID.xy);

       int a = int(getImagePixel(inputImage,image_pixel_cord));
       int b = int(getImagePixel(inputImage,image_pixel_cord+ivec2(1,0)));

       setImagePixel(resultImage,image_pixel_cord,int(combine(a,image_pixel_cord.x)));
       setImagePixel(resultImage,image_pixel_cord+ivec2(1,0),int(combine(a,image_pixel_cord.x+1)));

       barrier();

       //process here
       uint stride = 1;
       for(int level = 0; level < levels ; level++)
       {
           if((mod(image_pixel_cord.x,stride)==0) && is_inside(image_pixel_cord,ivec2(-1,-1),ivec2(imageWidth+1,imageHeight+1)) && is_inside(image_pixel_cord+ivec2(stride,0),ivec2(-1,-1),ivec2(imageWidth+1,imageHeight+1)))
           {
               a = getImagePixel(resultImage,image_pixel_cord);
               b = getImagePixel(resultImage,image_pixel_cord+ivec2(stride,0));
               setImagePixel(resultImage,image_pixel_cord,(a+b));
           }
           stride = stride * 2;
           barrier();
       }
       //setOutputImagePixel_0(image_pixel_cord,outf);
}
