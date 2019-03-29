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
       float guassian_kernal[9] = float[9](1.0/16,2.0/16,1.0/16,2.0/16,4.0/16,2.0/16,1.0/16,2.0/16,1.0/16);

       ivec2 image_pixel_cord = ivec2(gl_GlobalInvocationID.x,gl_GlobalInvocationID.y);

       float outf = compute_kernal_filter_3_3(image_pixel_cord,guassian_kernal);

       vec4 inv = vec4(outf,0.0,0.0,1.0);

       imageStore(resultImage,image_pixel_cord,inv);

}
