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

float compute_kernal_filter_5_5(ivec2 image_cord,float kernal[5*5])
{
    float val = 0;
    for(int i = 0;i<5;i++)
    {
        for(int j = 0;j<5;j++)
        {
            val += (kernal[i*5+j] * getInputImagePixel(image_cord+ivec2(i-2,j-2)));
        }
    }
   return val;
}

void main()
{
       const float guassian_kernal[25] = float[25](1.0/273.0,4.0/273.0,7.0/273.0,4.0/273.0,1.0/273.0,4.0/273.0,16.0/273.0,26.0/273.0,16.0/273.0,4.0/273.0,7.0/273.0,26.0/273.0,41.0/273.0,26.0/273.0,7.0/273.0,4.0/273.0,16.0/273.0,26.0/273.0,16.0/273.0,4.0/273.0,1.0/273.0,4.0/273.0,7.0/273.0,4.0/273.0,1.0/273.0);

       ivec2 image_pixel_cord = ivec2(gl_GlobalInvocationID.x,gl_GlobalInvocationID.y);

       float outf = compute_kernal_filter_5_5(image_pixel_cord,guassian_kernal);

       vec4 inv = vec4(outf,0.0,0.0,1.0);

       imageStore(resultImage,image_pixel_cord,inv);
       //else imageStore(resultImage,image_pixel_cord,getInputImagePixel(image_pixel_cord));

}
