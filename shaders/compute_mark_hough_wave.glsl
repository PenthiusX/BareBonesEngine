#version 430

//layout (local_size_x =17, local_size_y = 8,local_size_z = 5) in;
//layout (local_size_x =16, local_size_y = 16) in;
#local_size_define
layout(binding=0, r32i) uniform mediump iimage2D houghImage;

layout( location=0 ) uniform ivec2 cordinate;
layout( location=1 ) uniform ivec2 image_resolution;

#include compute_basic_functions.glsl

#define PI 3.1415926535897932384626433832795

ivec2 getPixelCord(){
    return ivec2(gl_LocalInvocationIndex,0);
}

void main()
{
       ivec2 image_pixel_cord = getPixelCord();
       float theta = float(image_pixel_cord.x+(image_resolution.x/4)) * PI / image_resolution.x;
       int r = (int((cordinate.x*cos(theta))+(cordinate.y*sin(theta))+image_resolution.y)/2);
       imageAtomicAdd(houghImage,ivec2(image_pixel_cord.x,r),1);
}
