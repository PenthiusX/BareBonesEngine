#version 430

//layout (local_size_x =17, local_size_y = 8,local_size_z = 5) in;
//layout (local_size_x =16, local_size_y = 16) in;
#local_size_define
layout(binding=0, r32i) uniform mediump iimage2D inputImage;
layout(binding=0, r32i) uniform mediump iimage2D resultImage;

layout( location=0 ) uniform ivec2 stage_center;
layout( location=1 ) uniform ivec2 image_resolution;
layout( location=2 ) uniform int rotation_step;

#include compute_basic_functions.glsl

#define PI 3.1415926535897932384626433832795


void main()
{
       ivec2 image_pixel_cord = ivec2(gl_GlobalInvocationID.xy);

       float theta = float(image_pixel_cord.x-rotation_step) * 2 * PI / image_resolution.x;

       int r = ceil(float(getImagePixel(inputImage,ivec2(0,image_pixel_cord.h)))/cos(theta));

       imageAtomicMin(resultImage,ivec2(image_pixel_cord.x,r),r);
}
