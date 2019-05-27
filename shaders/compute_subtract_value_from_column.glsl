#version 430

//layout (local_size_x =17, local_size_y = 8,local_size_z = 5) in;
//layout (local_size_x =16, local_size_y = 16) in;
#local_size_define
layout(binding=0, r32i) uniform mediump iimage2D inputImage;
layout(binding=1, r32i) uniform mediump iimage2D resultImage;

layout( location=0 ) uniform int value;

void setImagePixel(uimage2D4x8_bindless img,ivec2 cord,uint value){
    imageStore(img, cord,uvec4(value,0,0,255));
}


#include compute_basic_functions.glsl

ivec2 getPixelCord(){
    return ivec2(0,gl_LocalInvocationIndex);
}

void main()
{
       ivec2 image_pixel_cord = getPixelCord();

       int a = getImagePixel(inputImage,image_pixel_cord);

       setImagePixel(resultImage,image_pixel_cord,value-a);

}
