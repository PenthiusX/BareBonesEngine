/* Basic compute functions required by every copmute shader
 * Include this file in almost every compute shader
 * Author : Saurabh
 * Created :04_06_2019
*/

//#define uimage2D1x8_bindless uimage2D
//#define uimage2D1x32_bindless uimage2D
//#define iimage2D1x32_bindless iimage2D

/* <single_value_return> imageLoad(gimage image​, ivec2 cord);
 *
 * these functions take image uniform struct and 2D image cordinates as input respectively
 * these functions returs a single value RED component
 * which is used for grayscale image processing
 * <single_value_return> is float in case of image2D()
 * and uint in case of uimage2D
*/

//for Image Unit Format : R8UI , Format Qualifier : r8ui
uint getImagePixel(uimage2D1x8_bindless img,ivec2 cord){
    return imageLoad(img, cord).r;
}

//for Image Unit Format : R32UI , Format Qualifier : r32ui
uint getImagePixel(uimage2D1x32_bindless img,ivec2 cord){
    return imageLoad(img, cord).r;
}

//for Image Unit Format : R32I , Format Qualifier : r32ui
int getImagePixel(iimage2D1x32_bindless img,ivec2 cord){
    return imageLoad(img, cord).r;
}

//for Image Unit Format : R8 , Format Qualifier : r8
float getImagePixel(unimage2D1x8_bindless img,ivec2 cord){
    return imageLoad(img, cord).r;
}


/* <single_value_return> imageLoad(gimage image​, ivec2 cord);
 *
 * these functions take image uniform struct and 2D image cordinates as input respectively
 * these functions returs a single value RED component
 * which is used for grayscale image processing
 * <single_value_return> is float in case of image2D()
 * and uint in case of uimage2D
*/

//for Image Unit Format : R8UI , Format Qualifier : r8ui
void setImagePixel(uimage2D1x8_bindless img,ivec2 cord,uint value){
    imageStore(img, cord,uvec4(value,0,0,255));
}

//for Image Unit Format : R32UI , Format Qualifier : r32ui
void setImagePixel(uimage2D1x32_bindless img,ivec2 cord,uint value){
    imageStore(img, cord,uvec4(value,0,0,255));
}

//for Image Unit Format : R32I , Format Qualifier : r32i
void setImagePixel(iimage2D1x32_bindless img,ivec2 cord,int value){
    imageStore(img, cord,ivec4(value,0,0,255));
}

//for Image Unit Format : R8 , Format Qualifier : r8
void setImagePixel(unimage2D1x8_bindless img,ivec2 cord,float value){
    imageStore(img, cord,uvec4(value,0,0,255));
}


/* float compute_kernal_filter_3_3(gimage img,ivec2 image_cord,float kernal[3*3])
 * performs 2d spatial domain filtering on image (convolution filtering)
 * these functions take image uniform struct , 2D image cordinates ,kernal matrix float array(3*3 elements) as input respectively
 * these functions returs a single value component
 * which is used for grayscale image processing
*/
//for Image Unit Format : R8UI , Format Qualifier : r8ui
float compute_kernal_filter_3_3(uimage2D1x8_bindless img,ivec2 image_cord,float kernal[3*3])
{
    float val = 0;
    for(int i = 0;i<3;i++)
    {
        for(int j = 0;j<3;j++)
        {
            val += (kernal[i*3+j] * getImagePixel(img,image_cord+ivec2(i-1,j-1)));
        }
    }
   return val;
}

//for Image Unit Format : R32UI , Format Qualifier : r32ui
float compute_kernal_filter_3_3(uimage2D1x32_bindless img,ivec2 image_cord,float kernal[3*3])
{
    float val = 0;
    for(int i = 0;i<3;i++)
    {
        for(int j = 0;j<3;j++)
        {
            val += (kernal[i*3+j] * getImagePixel(img,image_cord+ivec2(i-1,j-1)));
        }
    }
   return val;
}

//for Image Unit Format : R8 , Format Qualifier : r8
float compute_kernal_filter_3_3(unimage2D1x8_bindless img,ivec2 image_cord,float kernal[3*3])
{
    float val = 0;
    for(int i = 0;i<3;i++)
    {
        for(int j = 0;j<3;j++)
        {
            val += (kernal[i*3+j] * getImagePixel(img,image_cord+ivec2(i-1,j-1)));
        }
    }
   return val;
}


/* float compute_kernal_filter_5_5(gimage img,ivec2 image_cord,float kernal[5*5])
 * performs 2d spatial domain filtering on image (convolution filtering)
 * these functions take image uniform struct , 2D image cordinates ,kernal matrix float array(3*3 elements) as input respectively
 * these functions returs a single value component
 * which is used for grayscale image processing
*/
//for Image Unit Format : R8UI , Format Qualifier : r8ui
float compute_kernal_filter_5_5(uimage2D1x8_bindless img,ivec2 image_cord,float kernal[5*5])
{
    float val = 0;
    for(int i = 0;i<5;i++)
    {
        for(int j = 0;j<5;j++)
        {
            val += (kernal[i*5+j] * getImagePixel(img,image_cord+ivec2(i-2,j-2)));
        }
    }
   return val;
}

//for Image Unit Format : R32UI , Format Qualifier : r32ui
float compute_kernal_filter_5_5(uimage2D1x32_bindless img,ivec2 image_cord,float kernal[5*5])
{
    float val = 0;
    for(int i = 0;i<5;i++)
    {
        for(int j = 0;j<5;j++)
        {
            val += (kernal[i*5+j] * getImagePixel(img,image_cord+ivec2(i-2,j-2)));
        }
    }
   return val;
}

//for Image Unit Format : R8 , Format Qualifier : r8
float compute_kernal_filter_5_5(unimage2D1x8_bindless img,ivec2 image_cord,float kernal[5*5])
{
    float val = 0;
    for(int i = 0;i<5;i++)
    {
        for(int j = 0;j<5;j++)
        {
            val += (kernal[i*5+j] * getImagePixel(img,image_cord+ivec2(i-2,j-2)));
        }
    }
   return val;
}
