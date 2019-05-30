#version 430

//layout (local_size_x =17, local_size_y = 8,local_size_z = 5) in;
//layout (local_size_x =5, local_size_y = 5) in;
#local_size_define
layout(binding=0, r8ui) uniform mediump uimage2D inputImage;
layout(binding=1, r8ui) uniform mediump uimage2D resultImage;

layout( location=0 ) uniform ivec2 stage_center;
layout( location=1 ) uniform ivec2 mesh_wrap_resolution;
layout( location=2 ) uniform int rotation_step;
layout( location=3 ) uniform ivec2 image_resolution;
layout( location=4 ) uniform float cosine;

#include compute_basic_functions.glsl

#define PI 3.1415926535897932384626433832795

ivec2 getPixelCord(){
    return ivec2(gl_LocalInvocationIndex,dot(uvec3(1,gl_NumWorkGroups.x,gl_NumWorkGroups.x*gl_NumWorkGroups.y),gl_WorkGroupID));
}

void main()
{
       ivec2 image_pixel_cord = ivec2(gl_GlobalInvocationID.xy);

       uint value = getImagePixel(inputImage,image_pixel_cord);

       setImagePixel(resultImage,image_pixel_cord,value);
       //imageAtomicMin(resultImage,image_pixel_cord,0);
}
