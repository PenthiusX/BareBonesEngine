#version 430

layout (local_size_x =16, local_size_y = 16) in;
layout(binding=0, r8) uniform mediump image2D inputImage;
layout(binding=1, r8) uniform mediump image2D resultImage;


float getInputImagePixel(ivec2 cord){
    return imageLoad(inputImage, cord).r;
}

float compute_invert(ivec2 image_cord)
{
    return 1.0 - getInputImagePixel(image_cord);
}

float compute_kernal_filter_3_3(ivec2 image_cord,float kernal[3*3])
{
    float val = 0;
    for(int i = 0;i<3;i++)
    {
        for(int j = 0;j<3;j++)
        {
            val += (kernal[i*3+j] * getInputImagePixel(image_cord+ivec2(i-1,j-1)));
        }
    }
   return val;
}

void main()
{
        uint u = gl_GlobalInvocationID.x;
        uint v = gl_GlobalInvocationID.y;

       float sobel_x_kernal[9] = float[9](-1,-2,-1,0,0,0,1,2,1);
       float sobel_y_kernal[9] = float[9](-1,0,1,-2,0,2,-1,0,1);

       float sobel_x = compute_kernal_filter_3_3(ivec2(u,v),sobel_x_kernal);
       float sobel_y = compute_kernal_filter_3_3(ivec2(u,v),sobel_y_kernal);

       float outf = length(vec2(sobel_x, sobel_y))/8.0;

       vec4 inv = vec4(outf,0.0,0.0,1.0);
        imageStore(resultImage, ivec2(u,v),inv);

}
