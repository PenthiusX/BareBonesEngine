#include "_gpu_compute.h"
#include <_shader.h>
#include <QDebug>


/* _GPU_Compute Class
 * for handling all compute operations
 * should be created when a gl context is active
 * and processing should happen in that same context
 * created: 20_03_2019
 * Author : Saurabh
*/
_GPU_Compute::_GPU_Compute() : QOpenGLExtraFunctions(QOpenGLContext::currentContext())
{

}
void _GPU_Compute::compute_copy_8_to_32(_Texture &input_img, _Texture &output_img)
{
    static _Shader shader;

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_copy_8_to_32.glsl",GL_COMPUTE_SHADER);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img.bindForCompute(0,GL_R8UI,GL_READ_ONLY);
    output_img.bindForCompute(1,GL_R32I,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glDispatchCompute(input_img.getWidth() / 16, input_img.getHeight() / 16, 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void _GPU_Compute::compute_copy_32_to_8(_Texture &input_img, _Texture &output_img)
{
    static _Shader shader;

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_copy_32_to_8.glsl",GL_COMPUTE_SHADER);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img.bindForCompute(0,GL_R32I,GL_READ_ONLY);
    output_img.bindForCompute(1,GL_R8UI,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glDispatchCompute(input_img.getWidth() / 16, input_img.getHeight() / 16, 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

/* Function : compute_sobel_edge
 *
 * computes sobel edge by using 3 x 3 kernal
 * returns sobel gradient magnitude
 * requires imput image texture and output image texture
*/
void _GPU_Compute::compute_sobel_edge(_Texture& input_img,_Texture& output_img)
{
    static _Shader shader;

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_sobel_edge_mag.glsl",GL_COMPUTE_SHADER);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img.bindForCompute(0,GL_R8UI,GL_READ_ONLY);
    output_img.bindForCompute(1,GL_R8UI,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glDispatchCompute(input_img.getWidth() / 16, input_img.getHeight() / 16, 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

/* Function : compute_sobel_edge
 *
 * computes sobel edge by using 3 x 3 kernal
 * returns sobel gradient magnitude and gradint angle(range 0:180 degress as 0:255 values)
 * requires imput image texture , output magnitude image texture ,output angle image texture
*/
void _GPU_Compute::compute_sobel_edge(_Texture& input_img,_Texture& output_mag,_Texture& output_theta)
{

    static _Shader shader;

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_sobel_edge_mag_theta.glsl",GL_COMPUTE_SHADER);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img.bindForCompute(0,GL_R8UI,GL_READ_ONLY);
    output_mag.bindForCompute(1,GL_R8UI,GL_WRITE_ONLY);
    output_theta.bindForCompute(2,GL_R8UI,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glDispatchCompute(input_img.getWidth() / 16, input_img.getHeight() / 16, 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}


void _GPU_Compute::compute_guassian_blur_3_3(_Texture& input_img,_Texture& output_img)
{
    static _Shader shader;

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_guassian_blur_3_3.glsl",GL_COMPUTE_SHADER);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img.bindForCompute(0,GL_R8UI,GL_READ_ONLY);
    output_img.bindForCompute(1,GL_R8UI,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glDispatchCompute(input_img.getWidth() / 16, input_img.getHeight() / 16, 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void _GPU_Compute::compute_guassian_blur_5_5(_Texture& input_img,_Texture& output_img)
{
    static _Shader shader;

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_guassian_blur_5_5.glsl",GL_COMPUTE_SHADER);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img.bindForCompute(0,GL_R8UI,GL_READ_ONLY);
    output_img.bindForCompute(1,GL_R8UI,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glDispatchCompute(input_img.getWidth() / 16, input_img.getHeight() / 16, 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void _GPU_Compute::compute_invert(_Texture& input_img,_Texture& output_img)
{
    static _Shader shader;

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_uint_invert.glsl",GL_COMPUTE_SHADER);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img.bindForCompute(0,GL_R8,GL_READ_ONLY);
    output_img.bindForCompute(1,GL_R8,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glDispatchCompute(input_img.getWidth() / 16, input_img.getHeight() / 16, 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void _GPU_Compute::compute_threshold(_Texture& input_img,_Texture& output_img)
{
    static _Shader shader;

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_threshold.glsl",GL_COMPUTE_SHADER);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img.bindForCompute(0,GL_R8UI,GL_READ_ONLY);
    output_img.bindForCompute(1,GL_R8UI,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glDispatchCompute(input_img.getWidth() / 16, input_img.getHeight() / 16, 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void _GPU_Compute::compute_canny_edge(_Texture& input_img,_Texture& output_img)
{
    static _Texture texture_sobel_mag_(nullptr,input_img.getWidth(),input_img.getHeight());
    static _Texture texture_sobel_theta_(nullptr,input_img.getWidth(),input_img.getHeight());
    static _Texture texture_blur(nullptr,input_img.getWidth(),input_img.getHeight());

    texture_sobel_mag_.load(GL_RED,GL_UNSIGNED_BYTE);
    texture_blur.load(GL_RED,GL_UNSIGNED_BYTE);
    texture_sobel_theta_.load(GL_RED,GL_UNSIGNED_BYTE);

    compute_guassian_blur_3_3(input_img,texture_blur);
    compute_sobel_edge(texture_blur,texture_sobel_mag_,texture_sobel_theta_);

    static _Shader shader;

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_canny.glsl",GL_COMPUTE_SHADER);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    texture_sobel_mag_.bindForCompute(0,GL_R8UI,GL_READ_ONLY);
    texture_sobel_theta_.bindForCompute(1,GL_R8UI,GL_READ_ONLY);
    output_img.bindForCompute(2,GL_R8UI,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glDispatchCompute(input_img.getWidth() / 16, input_img.getHeight() / 16, 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}
