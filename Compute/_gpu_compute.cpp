#include "_gpu_compute.h"
#include <_shader.h>
#include <QDebug>

_GPU_Compute::_GPU_Compute() : QOpenGLExtraFunctions(QOpenGLContext::currentContext())
{

}

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

    input_img.bindForCompute(0,GL_R8,GL_READ_ONLY);
    output_img.bindForCompute(1,GL_R8,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glDispatchCompute(input_img.getWidth() / 16, input_img.getHeight() / 16, 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}
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

    input_img.bindForCompute(0,GL_R8,GL_READ_ONLY);
    output_mag.bindForCompute(1,GL_R8,GL_WRITE_ONLY);
    output_theta.bindForCompute(2,GL_R8,GL_WRITE_ONLY);

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

    input_img.bindForCompute(0,GL_R8,GL_READ_ONLY);
    output_img.bindForCompute(1,GL_R8,GL_WRITE_ONLY);

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

    input_img.bindForCompute(0,GL_R8,GL_READ_ONLY);
    output_img.bindForCompute(1,GL_R8,GL_WRITE_ONLY);

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
        shader.setChildShader(":/shaders/compute_invert.glsl",GL_COMPUTE_SHADER);
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

    input_img.bindForCompute(0,GL_R8,GL_READ_ONLY);
    output_img.bindForCompute(1,GL_R8,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glDispatchCompute(input_img.getWidth() / 16, input_img.getHeight() / 16, 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void _GPU_Compute::compute_canny_edge(_Texture& input_img,_Texture& output_img)
{
    _Texture texture_sobel_mag(0,input_img.getWidth(),input_img.getHeight());
    _Texture texture_sobel_theta(0,input_img.getWidth(),input_img.getHeight());
    _Texture texture_blur(0,input_img.getWidth(),input_img.getHeight());

    texture_sobel_mag.load(GL_RED,GL_UNSIGNED_BYTE);
    texture_blur.load(GL_RED,GL_UNSIGNED_BYTE);
    texture_sobel_theta.load(GL_RED,GL_UNSIGNED_BYTE);

    compute_guassian_blur_5_5(input_img,texture_blur);
    compute_sobel_edge(texture_blur,texture_sobel_mag,texture_sobel_theta);

    static _Shader shader;

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_canny.glsl",GL_COMPUTE_SHADER);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    texture_sobel_mag.bindForCompute(0,GL_R8,GL_READ_ONLY);
    texture_sobel_theta.bindForCompute(1,GL_R8,GL_READ_ONLY);
    output_img.bindForCompute(2,GL_R8,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glDispatchCompute(input_img.getWidth() / 16, input_img.getHeight() / 16, 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}
