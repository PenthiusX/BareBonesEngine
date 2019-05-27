#version 430

layout (local_size_x =16, local_size_y = 16) in;
layout(binding=0, r8ui) uniform mediump uimage2D inputImage;
layout(binding=1, r8ui) uniform mediump uimage2D resultImage;

#include compute_basic_functions.glsl

void main()
{
       ivec2 image_pixel_cord = ivec2(gl_GlobalInvocationID.xy);

       float sobel_x_kernal[9] = float[9](-1,-2,-1,0,0,0,1,2,1);
       float sobel_y_kernal[9] = float[9](-1,0,1,-2,0,2,-1,0,1);

       float sobel_x = compute_kernal_filter_3_3(inputImage,image_pixel_cord,sobel_x_kernal);
       float sobel_y = compute_kernal_filter_3_3(inputImage,image_pixel_cord,sobel_y_kernal);

       uint sobel_mag = uint(length(vec2(sobel_x, sobel_y))/8.0);

       setImagePixel(resultImage,image_pixel_cord,sobel_mag);
}
