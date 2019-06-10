#version 430

//layout (local_size_x =17, local_size_y = 8,local_size_z = 5) in;
//layout (local_size_x =5, local_size_y = 5) in;
#local_size_define
layout(binding=0, r32i) uniform mediump iimage2D inputImage;
layout(binding=1, r32i) uniform mediump iimage2D resultImage;

layout( location=0 ) uniform ivec2 stage_center;
layout( location=1 ) uniform ivec2 mesh_wrap_resolution;
layout( location=2 ) uniform int rotation_step;
layout( location=3 ) uniform ivec2 image_resolution;

#include compute_basic_functions.glsl

#define PI 3.1415926535897932384626433832795

ivec2 getPixelCord(){

    return ivec2(gl_LocalInvocationIndex,dot(uvec3(1,gl_NumWorkGroups.x,gl_NumWorkGroups.x*gl_NumWorkGroups.y),gl_WorkGroupID));
}

void main()
{
       ivec2 image_pixel_cord = getPixelCord();

       float theta = float(image_pixel_cord.x-rotation_step) * 2 * PI / mesh_wrap_resolution.x;

       float cosine=cos(theta);
       int side = (cosine>0.0) ? 0 : 1;

       int value = getImagePixel(inputImage,ivec2(side,image_pixel_cord.y));



       if(side == 0)
       {
//       if(value < (image_resolution.x-10))
//           {
               float r_max = (float(stage_center.x - value)/cosine);
               if(abs(r_max) < 50)
               {
                   int r = int(ceil(r_max));

                   imageAtomicMin(resultImage,image_pixel_cord,r);
                   //setImagePixel(resultImage,image_pixel_cord,100);
               }

//           }
//           else
//           {
//               //imageAtomicMin(resultImage,image_pixel_cord,0);
//           }
       }
//       else
//       {
//           setImagePixel(resultImage,image_pixel_cord,200);
//       }


}
