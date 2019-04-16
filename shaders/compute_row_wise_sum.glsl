#version 430

layout (local_size_x =17, local_size_y = 8,local_size_z = 5) in;
//layout (local_size_x =16, local_size_y = 16) in;

layout(binding=0, r8ui) uniform mediump uimage2D inputImage;
layout(binding=1, r32i) uniform mediump iimage2D resultImage;

layout( location=0 ) uniform unsigned int levels;

#include compute_basic_functions.glsl

//row operations

ivec2 getPixelCord(){

    return ivec2(gl_LocalInvocationIndex,dot(uvec3(1,gl_NumWorkGroups.x,gl_NumWorkGroups.x*gl_NumWorkGroups.y),gl_WorkGroupID));
}
bool is_inside(ivec2 val, ivec2 lower, ivec2 upper)
{
   return ((val.x > lower.x ) && (val.y > lower.y ) && (val.x < upper.x ) && (val.y < upper.y ));
}

void main()
{
       ivec2 image_pixel_cord = getPixelCord();
       image_pixel_cord.x=image_pixel_cord.x*2;
       //ivec2 image_pixel_cord = ivec2(gl_GlobalInvocationID.xy);

       int a = int(getImagePixel(inputImage,image_pixel_cord));
       int b = int(getImagePixel(inputImage,image_pixel_cord+ivec2(1,0)));

       setImagePixel(resultImage,image_pixel_cord,a);
       setImagePixel(resultImage,image_pixel_cord+ivec2(1,0),b);

       barrier();

       //process here
       uint stride = 1;
       for(int level = 0; level < levels ; level++)
       {
           if((mod(image_pixel_cord.x,stride)==0) && is_inside(image_pixel_cord,ivec2(0,0),ivec2(1360,1024)) && is_inside(image_pixel_cord+ivec2(stride,0),ivec2(0,0),ivec2(1360,1024)))
           {
               a = getImagePixel(resultImage,image_pixel_cord);
               b = getImagePixel(resultImage,image_pixel_cord+ivec2(stride,0));
               setImagePixel(resultImage,image_pixel_cord,(a+b));

           }
           stride = stride * 2;
           barrier();
       }
}

