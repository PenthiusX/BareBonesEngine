#include "_gpu_compute.h"
#include <_shader.h>
#include <QDebug>
#include <_tools.h>
#include <vector>
#include <cmath>

#define PI 3.1415926535897932384626433832795

/* _GPU_Compute Class
 * for handling all compute operations
 * should be created when a gl context is active
 * and processing should happen in that same context
 * created: 20_03_2019
 * Author : Saurabh
*/
_GPU_Compute::_GPU_Compute() :
    QOpenGLExtraFunctions(QOpenGLContext::currentContext())
{

}

void _GPU_Compute::compute_copy_8_to_32(_Texture &input_img, _Texture &output_img)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(input_img.getWidth(), input_img.getHeight());

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_copy_8_to_32.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img.bindForCompute(0,GL_R8UI,GL_READ_ONLY);
    output_img.bindForCompute(1,GL_R32I,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void _GPU_Compute::compute_copy_red_to_rgba(_Texture &input_img, _Texture &output_img)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(input_img.getWidth(), input_img.getHeight());

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_copy_red_to_rgba.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img.bindForCompute(0,GL_R8UI,GL_READ_ONLY);
    output_img.bindForCompute(1,GL_RGBA8UI,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);


    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void _GPU_Compute::compute_copy_32_to_8(_Texture &input_img, _Texture &output_img)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(input_img.getWidth(), input_img.getHeight());

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_copy_32_to_8.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img.bindForCompute(0,GL_R32I,GL_READ_ONLY);
    output_img.bindForCompute(1,GL_R8UI,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}
void _GPU_Compute::computeFrom32iTo8uiDevide(_Texture &input_img, _Texture &output_img,unsigned int devide_value)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(input_img.getWidth(), input_img.getHeight());

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_devide_32_to_8.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img.bindForCompute(0,GL_R32I,GL_READ_ONLY);
    output_img.bindForCompute(1,GL_R8UI,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glUniform1ui(0,devide_value);

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

/* Function: getWorkGroupSize(int w,int h, COMPUTE_OPERTION operation)
 * This function calculates number of workgroup and workgroup sizes
 * to invoke gpu compute operation
 * barrier in compute shaders work within that workgroup only
 * ie. barrier waits for all threads in workgroup to reach the same point
 * so if a row wise operations is to be performed it will require whole row to contain in one workgroup
 * the third parameter operation specifies the operation type to be performed, default is _DEFAULT_2D_GROUP
 * created : 15_04_2019
*/
_GPU_Compute::GroupSize _GPU_Compute::getWorkGroupSize(int w,int h, COMPUTE_OPERTION operation)
{
    GroupSize groupsize;
    switch (operation) {
    case _DEFAULT_2D_GROUP:
    {
        int level = 16;
        groupsize.WorkGroupSize.x=level;
        groupsize.NumWorkGroups.x=glm::ceil(float(w)/level);

        groupsize.WorkGroupSize.y=level;
        groupsize.NumWorkGroups.y=glm::ceil(float(h)/level);

        groupsize.WorkGroupSize.z = 1;
        groupsize.NumWorkGroups.z = 1;
        break;
    }

    case _ROW_WISE_LOCAL_GROUP:
    {
        groupsize.NumWorkGroups=_Tools::GetGroupSize(h);
        groupsize.WorkGroupSize=_Tools::GetGroupSize(w);//local has row size

        break;
    }
    case _COLUMN_WISE_LOCAL_GROUP:
    {
        groupsize.NumWorkGroups=_Tools::GetGroupSize(w);
        groupsize.WorkGroupSize=_Tools::GetGroupSize(h);//local has column size

        break;
    }
    }

    return groupsize;
}

void _GPU_Compute::compute_row_wise_max(_Texture& input_img,_Texture& output_img)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(input_img.getWidth()/2, input_img.getHeight(),_ROW_WISE_LOCAL_GROUP);

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_row_wise_max.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img.bindForCompute(0,GL_R8UI,GL_READ_ONLY);
    output_img.bindForCompute(1,GL_R8UI,GL_WRITE_ONLY);

    shader.useShaderProgram();

    int levels = glm::ceil(glm::log2(float(input_img.getWidth())));
    glUniform1ui(0,levels);

    //calculate size of workgroups based on image resolution here

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void _GPU_Compute::compute_mark_column_index(_Texture& input_img,_Texture& output_img)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(1, input_img.getHeight(),_COLUMN_WISE_LOCAL_GROUP);

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_mark_column_index.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    compute_clear_8_ui_texture(output_img,0);

    input_img.bindForCompute(0,GL_R32I,GL_READ_ONLY);
    output_img.bindForCompute(1,GL_RGBA8UI,GL_WRITE_ONLY);

    shader.useShaderProgram();

    //calculate size of workgroups based on image resolution here

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void _GPU_Compute::compute_copy_column_from_to(_Texture& input_img,_Texture& output_img,int from_,int to_)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(1, input_img.getHeight(),_COLUMN_WISE_LOCAL_GROUP);

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_copy_column_from_to.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img.bindForCompute(0,GL_R32I,GL_READ_ONLY);
    output_img.bindForCompute(1,GL_R32I,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glUniform1i(0,from_);
    glUniform1i(1,to_);

    //calculate size of workgroups based on image resolution here

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void _GPU_Compute::compute_divide_mark_column_index(_Texture& input_img,_Texture& input_img2,_Texture& output_img)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(1, input_img.getHeight(),_COLUMN_WISE_LOCAL_GROUP);

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_divide_mark_column_index.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    compute_clear_8_ui_texture(output_img,0);

    input_img.bindForCompute(0,GL_R32I,GL_READ_ONLY);
    input_img2.bindForCompute(1,GL_R32I,GL_READ_ONLY);
    output_img.bindForCompute(2,GL_R8UI,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glUniform1ui(0,input_img.getWidth());
    glUniform1ui(1,input_img.getHeight());

    //calculate size of workgroups based on image resolution here

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void _GPU_Compute::compute_clear_8_ui_texture(_Texture& input_img,unsigned int value)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(input_img.getWidth(), input_img.getHeight());

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_clear_8_ui_texture.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img.bindForCompute(0,GL_R8UI,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glUniform1ui(0,value);

    //calculate size of workgroups based on image resolution here

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void _GPU_Compute::compute_clear_32_i_texture(_Texture& input_img,unsigned int value)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(input_img.getWidth(), input_img.getHeight());

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_clear_32_ui_texture.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img.bindForCompute(0,GL_R32I,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glUniform1ui(0,value);

    //calculate size of workgroups based on image resolution here

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void _GPU_Compute::compute_row_wise_arg_max(_Texture& input_img,_Texture& output_img)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(input_img.getWidth()/2, input_img.getHeight(),_ROW_WISE_LOCAL_GROUP);
//    static unsigned int ssbo = 0;

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_row_wise_arg_max.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";

//        glGenBuffers(1, &ssbo);
//        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
//        glBufferData(GL_SHADER_STORAGE_BUFFER, input_img.getHeight() * 200 * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);

    }

    input_img.bindForCompute(0,GL_R8UI,GL_READ_ONLY);
    output_img.bindForCompute(1,GL_R32I,GL_READ_WRITE);

    shader.useShaderProgram();

    int levels = glm::ceil(glm::log2(float(input_img.getWidth())));
    glUniform1ui(0,levels);

    //calculate size of workgroups based on image resolution here

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
//    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);

}

void _GPU_Compute::compute_row_wise_sum(_Texture& input_img,_Texture& output_img)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(input_img.getWidth()/2, input_img.getHeight(),_ROW_WISE_LOCAL_GROUP);

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_row_wise_arg_max.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img.bindForCompute(0,GL_R8UI,GL_READ_ONLY);
    output_img.bindForCompute(1,GL_R32I,GL_READ_WRITE);

    shader.useShaderProgram();

    int levels = glm::ceil(glm::log2(float(input_img.getWidth())));

    glUniform1ui(0,levels);
    glUniform1ui(1,input_img.getWidth());
    glUniform1ui(2,input_img.getHeight());

    //calculate size of workgroups based on image resolution here

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void _GPU_Compute::compute_row_wise_left_edge(_Texture& input_img,_Texture& output_img)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(input_img.getWidth()/2, input_img.getHeight(),_ROW_WISE_LOCAL_GROUP);

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_row_wise_left_edge.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img.bindForCompute(0,GL_R8UI,GL_READ_ONLY);
    output_img.bindForCompute(1,GL_R32I,GL_READ_WRITE);

    shader.useShaderProgram();

    int levels = glm::ceil(glm::log2(float(input_img.getWidth())));

    glUniform1ui(0,levels);
    glUniform2i(1,output_img.getWidth(),output_img.getHeight());

    //calculate size of workgroups based on image resolution here

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void _GPU_Compute::compute_row_wise_right_edge(_Texture& input_img,_Texture& output_img)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(input_img.getWidth()/2, input_img.getHeight(),_ROW_WISE_LOCAL_GROUP);

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_row_wise_right_edge.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img.bindForCompute(0,GL_R8UI,GL_READ_ONLY);
    output_img.bindForCompute(1,GL_R32I,GL_READ_WRITE);

    shader.useShaderProgram();

    int levels = glm::ceil(glm::log2(float(input_img.getWidth())));

    glUniform1ui(0,levels);
    glUniform2i(1,output_img.getWidth(),output_img.getHeight());

    //calculate size of workgroups based on image resolution here

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void _GPU_Compute::compute_row_wise_mean(_Texture& input_img,_Texture& output_img)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(input_img.getWidth()/2, input_img.getHeight(),_ROW_WISE_LOCAL_GROUP);

    _Texture sigma_x_dot_y = _Texture(nullptr,input_img.getWidth(),input_img.getHeight());
    _Texture sigma_y = _Texture(nullptr,input_img.getWidth(),input_img.getHeight());

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_row_wise_mean.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
        sigma_x_dot_y.load(GL_R32I,GL_RED_INTEGER, GL_INT);
        sigma_y.load(GL_R32I,GL_RED_INTEGER, GL_INT);
    }
    input_img.bindForCompute(0,GL_R8UI,GL_READ_ONLY);
    sigma_x_dot_y.bindForCompute(1,GL_R32I,GL_READ_WRITE);
    sigma_y.bindForCompute(2,GL_R32I,GL_READ_WRITE);

    shader.useShaderProgram();

    int levels = glm::ceil(glm::log2(float(input_img.getWidth())));

    glUniform1ui(0,levels);
    glUniform1ui(1,input_img.getWidth());
    glUniform1ui(2,input_img.getHeight());

    //calculate size of workgroups based on image resolution here

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    compute_divide_mark_column_index(sigma_x_dot_y,sigma_y,output_img);

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
    static GroupSize groupsize = getWorkGroupSize(input_img.getWidth(), input_img.getHeight());

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_sobel_edge_mag.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img.bindForCompute(0,GL_R8UI,GL_READ_ONLY);
    output_img.bindForCompute(1,GL_R8UI,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

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
    static GroupSize groupsize = getWorkGroupSize(input_img.getWidth(), input_img.getHeight());

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_sobel_edge_mag_theta.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img.bindForCompute(0,GL_R8UI,GL_READ_ONLY);
    output_mag.bindForCompute(1,GL_R8UI,GL_WRITE_ONLY);
    output_theta.bindForCompute(2,GL_R8UI,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);


    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void _GPU_Compute::compute_guassian_blur_3_3(_Texture& input_img,_Texture& output_img)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(input_img.getWidth(), input_img.getHeight());

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_guassian_blur_3_3.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img.bindForCompute(0,GL_R8UI,GL_READ_ONLY);
    output_img.bindForCompute(1,GL_R8UI,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void _GPU_Compute::compute_guassian_blur_5_5(_Texture& input_img,_Texture& output_img)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(input_img.getWidth(), input_img.getHeight());

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_guassian_blur_5_5.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img.bindForCompute(0,GL_R8UI,GL_READ_ONLY);
    output_img.bindForCompute(1,GL_R8UI,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void _GPU_Compute::compute_invert(_Texture& input_img,_Texture& output_img)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(input_img.getWidth(), input_img.getHeight());

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_uint_invert.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img.bindForCompute(0,GL_R8,GL_READ_ONLY);
    output_img.bindForCompute(1,GL_R8,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void _GPU_Compute::compute_mask_image_rgba_r(_Texture& input_img,_Texture& mask_img,_Texture& output_img)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(input_img.getWidth(), input_img.getHeight());

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_mask_image_rgba_r.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img.bindForCompute(0,GL_RGBA8UI,GL_READ_ONLY);
    mask_img.bindForCompute(1,GL_R8UI,GL_READ_ONLY);
    output_img.bindForCompute(2,GL_RGBA8UI,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void _GPU_Compute::compute_mix_with_mask_rgba_r_rgba(_Texture& input_img_1,_Texture& mask_img,_Texture& input_img_2,_Texture& output_img)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(input_img_1.getWidth(), input_img_1.getHeight());

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_mix_with_mask_rgba_r_rgba.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img_1.bindForCompute(0,GL_RGBA8UI,GL_READ_ONLY);
    input_img_2.bindForCompute(1,GL_RGBA8UI,GL_READ_ONLY);
    mask_img.bindForCompute(2,GL_R8UI,GL_READ_ONLY);
    output_img.bindForCompute(3,GL_RGBA8UI,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void _GPU_Compute::computeMaskImageRR(_Texture& input_img,_Texture& mask_img,_Texture& output_img)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(input_img.getWidth(), input_img.getHeight());

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_mask_image_r_r.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img.bindForCompute(0,GL_R8UI,GL_READ_ONLY);
    mask_img.bindForCompute(1,GL_R8UI,GL_READ_ONLY);
    output_img.bindForCompute(2,GL_R8UI,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void _GPU_Compute::computeMaskImageR32IR(_Texture& input_img,_Texture& mask_img,_Texture& output_img)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(input_img.getWidth(), input_img.getHeight());

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_mask_image_r32i_r.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img.bindForCompute(0,GL_R32I,GL_READ_ONLY);
    mask_img.bindForCompute(1,GL_R8UI,GL_READ_ONLY);
    output_img.bindForCompute(2,GL_R32I,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}


void _GPU_Compute::create_region_image_mask(_Texture &output_img, glm::ivec4 region)
{
    //8 bit image as 255-0 mask
    //region = ivec4(width_min,height_min,width_max,height_max)
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(output_img.getWidth(), output_img.getHeight());

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_create_region_image_mask.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    output_img.bindForCompute(0,GL_R8UI,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glUniform4i(0,region.x,region.y,region.z,region.w);

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void _GPU_Compute::compute_threshold(_Texture& input_img,_Texture& output_img,unsigned int threshold_value)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(input_img.getWidth(), input_img.getHeight());

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_threshold.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img.bindForCompute(0,GL_R8UI,GL_READ_ONLY);
    output_img.bindForCompute(1,GL_R8UI,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glUniform1ui(0,threshold_value);

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void _GPU_Compute::compute_threshold(_Texture* input_img,_Texture* output_img,unsigned int threshold_value)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(input_img->getWidth(), input_img->getHeight());

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_threshold.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img->bindForCompute(0,GL_R8UI,GL_READ_ONLY);
    output_img->bindForCompute(1,GL_R8UI,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glUniform1ui(0,threshold_value);

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void _GPU_Compute::compute_threshold_inv(_Texture& input_img,_Texture& output_img,unsigned int threshold_value)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(input_img.getWidth(), input_img.getHeight());

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_threshold_inv.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img.bindForCompute(0,GL_R8UI,GL_READ_ONLY);
    output_img.bindForCompute(1,GL_R8UI,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glUniform1ui(0,threshold_value);

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void _GPU_Compute::compute_saturate_color(_Texture& input_img,_Texture& output_img)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(input_img.getWidth(), input_img.getHeight());

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_saturate_color.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img.bindForCompute(0,GL_RGBA8UI,GL_READ_ONLY);
    output_img.bindForCompute(1,GL_RGBA8UI,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void _GPU_Compute::compute_gradient_to_descrete_color(_Texture& texture_sobel_mag_,_Texture& texture_sobel_theta_,_Texture& output_img)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(texture_sobel_mag_.getWidth(), texture_sobel_mag_.getHeight());

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_gradient_to_descrete_rgba_color.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    texture_sobel_mag_.bindForCompute(0,GL_R8UI,GL_READ_ONLY);
    texture_sobel_theta_.bindForCompute(1,GL_R8UI,GL_READ_ONLY);
    output_img.bindForCompute(2,GL_RGBA8UI,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void _GPU_Compute::computeGradientToDescreteColorAndValue(_Texture& texture_sobel_mag_,_Texture& texture_sobel_theta_,_Texture& output_img,_Texture& output_img_value)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(texture_sobel_mag_.getWidth(), texture_sobel_mag_.getHeight());

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_gradient_to_descrete_rgba_color_value.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    texture_sobel_mag_.bindForCompute(0,GL_R8UI,GL_READ_ONLY);
    texture_sobel_theta_.bindForCompute(1,GL_R8UI,GL_READ_ONLY);
    output_img.bindForCompute(2,GL_RGBA8UI,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

std::vector<_GPU_Compute::LineEquation> _GPU_Compute::computeHoughLines(_Texture& texture_edge,_Texture& texture_hough_space)
{
    std::vector<LineEquation> equations;

    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(texture_edge.getWidth(), texture_edge.getHeight());

    _Texture texture_hough_space_8_bit(nullptr,720,glm::sqrt(pow(texture_edge.getWidth(),2)+pow(texture_edge.getHeight(),2)));

    texture_hough_space_8_bit.load(GL_RED,GL_UNSIGNED_BYTE);

    //_Texture texture_hough_space(nullptr,720,glm::sqrt((texture_edge.getWidth()^2)+(texture_edge.getHeight()^2)));

    //texture_hough_space.load(GL_R32I,GL_RED_INTEGER, GL_INT);

//    //if shader not initialized
//    if(shader.getShaderProgram() == 0)
//    {
//        shader.setChildShader(":/shaders/compute_gradient_to_descrete_rgba_color_value.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
//        shader.attachShaders();
//        qDebug() << "shader initialized";
//    }

    char *colorFrame = getTextureImageFramebuffer(texture_edge);

    //texture_hough_space.load(GL_R32I,GL_RED_INTEGER, GL_INT);

    compute_clear_32_i_texture(texture_hough_space,0);

//    colorFrame = getTextureImageFramebuffer(texture_hough_space);

//    _Tools::SaveImageToPgm(colorFrame,texture_hough_space.getWidth(),texture_hough_space.getHeight(),"texture_hough_space_before.pgm");

    //QImage::fromData(colorFrame);

    glUniform3i(6,315,946,761);

    unsigned int index=0;

    for (unsigned int w = 0; w < texture_edge.getWidth(); w++) {
        for (unsigned int h = 0; h < texture_edge.getHeight(); h++) {
            index = texture_edge.getWidth()*h+w;
            if(colorFrame[index]!=0)
            {
                computeMarkHoughWave(texture_hough_space,glm::ivec2(w,h));
            }
        }
    }

//    texture_sobel_mag_.bindForCompute(0,GL_R8UI,GL_READ_ONLY);
//    texture_sobel_theta_.bindForCompute(1,GL_R8UI,GL_READ_ONLY);
//    output_img.bindForCompute(2,GL_RGBA8UI,GL_WRITE_ONLY);

//    shader.useShaderProgram();

    computeFrom32iTo8uiDevide(texture_hough_space,texture_hough_space_8_bit,8);

    colorFrame = getTextureImageFramebuffer(texture_hough_space_8_bit);

    LineEquation left_vert,right_vert,middle_hor;
    int left_vert_max=-129,right_vert_max=-129,middle_hor_max=-129;

    //highly jugaad implementation needs to be corrected afterwards
    for (unsigned int w = 0; w < texture_hough_space_8_bit.getWidth(); w++) {
        for (unsigned int h = 0; h < texture_hough_space_8_bit.getHeight(); h++) {
            index = texture_hough_space_8_bit.getWidth()*h+w;
            uchar value = colorFrame[index];
                if(w<(texture_hough_space.getWidth()/2))
                {
                    if(middle_hor_max<value)
                    {
                        middle_hor_max=value;
                        middle_hor.r=(h*2)-texture_hough_space.getHeight();
                        middle_hor.theta=float(w-(texture_hough_space.getWidth()/4))*PI/texture_hough_space.getWidth();
                    }
                }
                else {
                    if(h<(texture_hough_space.getHeight()/4))
                    {
                        if(left_vert_max<value)
                        {
                            left_vert_max=value;
                            left_vert.r=(h*2)-texture_hough_space.getHeight();
                            left_vert.theta=float(w-(texture_hough_space.getWidth()/4))*PI/texture_hough_space.getWidth();
                        }
                    }
                    else {
                        if(right_vert_max<value)
                        {
                            right_vert_max=value;
                            right_vert.r=(h*2)-texture_hough_space.getHeight();
                            right_vert.theta=float(w-(texture_hough_space.getWidth()/4))*PI/texture_hough_space.getWidth();
                        }
                    }
            }
        }
    }

    //colorFrame[texture_hough_space.getWidth()*right_vert.r+right_vert.theta]=255;
    //colorFrame[texture_hough_space.getWidth()*left_vert.r+left_vert.theta]=255;
    //colorFrame[texture_hough_space.getWidth()*middle_hor.r+middle_hor.theta]=255;

    equations.push_back(middle_hor);
    equations.push_back(left_vert);
    equations.push_back(right_vert);

    //colorFrame = getTextureImageFramebuffer(texture_hough_space_8_bit);

    //_Tools::SaveImageToPgm(colorFrame,texture_hough_space_8_bit.getWidth(),texture_hough_space_8_bit.getHeight(),"texture_hough_space.pgm");

    //compute_clear_32_i_texture(texture_hough_space,0);

    //colorFrame = getTextureImageFramebuffer(texture_hough_space);

    //colorFrame[texture_hough_space.getWidth()*right_vert.r+right_vert.theta]=255;
    //colorFrame[texture_hough_space.getWidth()*left_vert.r+left_vert.theta]=255;
    //colorFrame[texture_hough_space.getWidth()*texture_hough_space.getHeight()*3/4+texture_hough_space.getWidth()*3/4]=255;

    //_Tools::SaveImageToPgm(colorFrame,texture_hough_space.getWidth(),texture_hough_space.getHeight(),"texture_hough_space_max.pgm");

    return equations;
}

void _GPU_Compute::computeMarkHoughWave(_Texture& output_img,glm::ivec2 cordinate)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(output_img.getWidth(),1,_ROW_WISE_LOCAL_GROUP);

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_mark_hough_wave.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    output_img.bindForCompute(0,GL_R32I,GL_READ_WRITE);

    shader.useShaderProgram();

    glUniform2i(0,cordinate.x,cordinate.y);
    glUniform2i(1,output_img.getWidth(),output_img.getHeight());

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

glm::vec3 _GPU_Compute::compute_stage_angle(_Texture& input_img,_Texture& output_img)
{
    _Texture texture_sobel_mag_(nullptr,input_img.getWidth(),input_img.getHeight());
    _Texture texture_sobel_theta_(nullptr,input_img.getWidth(),input_img.getHeight());
    _Texture texture_mask(nullptr,input_img.getWidth(),input_img.getHeight());
    _Texture texture_mask_inv(nullptr,input_img.getWidth(),input_img.getHeight());
    _Texture texture_edge(nullptr,input_img.getWidth(),input_img.getHeight());
    _Texture texture_rgba(nullptr,input_img.getWidth(),input_img.getHeight());
    _Texture texture_out_rgba(nullptr,input_img.getWidth(),input_img.getHeight());
    _Texture texture_descrete_gradient_value(nullptr,input_img.getWidth(),input_img.getHeight());
    _Texture texture_hough_space(nullptr,720,glm::sqrt(pow(texture_edge.getWidth(),2)+pow(texture_edge.getHeight(),2)));


    texture_sobel_mag_.load(GL_RED,GL_UNSIGNED_BYTE);

    texture_sobel_theta_.load(GL_RED,GL_UNSIGNED_BYTE);
    texture_edge.load(GL_RED,GL_UNSIGNED_BYTE);
    texture_mask.load(GL_RED,GL_UNSIGNED_BYTE);
    texture_mask_inv.load(GL_RED,GL_UNSIGNED_BYTE);
    texture_descrete_gradient_value.load(GL_RED,GL_UNSIGNED_BYTE);
    texture_rgba.load(GL_RGBA,GL_UNSIGNED_BYTE);
    texture_out_rgba.load(GL_RGBA,GL_UNSIGNED_BYTE);
    texture_hough_space.load(GL_R32I,GL_RED_INTEGER, GL_INT);

    //compute_guassian_blur_5_5(input_img,texture_blur);
    //compute_canny_edge(input_img,texture_edge);
    //gpu_compute->compute_gradient_to_descrete_color(texture_in,texture_out);
    //gpu_compute->compute_copy_8_to_32(texture,texture_outt);
    //gpu_compute->compute_copy_32_to_8(texture_outt,texture_out);
    //gpu_compute->compute_sobel_edge(texture_in,texture_edge);
    //gpu_compute->compute_row_wise_arg_max(texture,texture_outt);
    //gpu_compute->compute_threshold(texture_edge,texture_thres,60);
    //gpu_compute->compute_stage_angle(texture_in,texture_out);
    //compute_copy_red_to_rgba(texture_edge,output_img);

    //compute_gradient_to_descrete_color(input_img,output_img);

    //sobel edge
    compute_sobel_edge(input_img,texture_sobel_mag_,texture_sobel_theta_);

    compute_canny_edge_from_sobel(texture_sobel_mag_,texture_sobel_theta_,texture_edge);

    //compute_copy_red_to_rgba(input_img,texture_out_rgba);

    //compute_gradient_to_descrete_color(texture_sobel_mag_,texture_sobel_theta_,output_img);

    computeGradientToDescreteColorAndValue(texture_sobel_mag_,texture_sobel_theta_,output_img,texture_descrete_gradient_value);

    create_region_image_mask(texture_mask,glm::ivec4(16,16,input_img.getWidth()-16,input_img.getHeight()-16));

    compute_mask_image_rgba_r(output_img,texture_mask,texture_out_rgba);

    compute_threshold(texture_edge,texture_mask,60);

    compute_mask_image_rgba_r(texture_out_rgba,texture_mask,texture_rgba);

    compute_saturate_color(texture_rgba,texture_rgba);

    compute_copy_red_to_rgba(input_img,output_img);

    compute_mix_with_mask_rgba_r_rgba(texture_rgba,texture_mask,output_img,output_img);

    create_region_image_mask(texture_edge,glm::ivec4(16,16,input_img.getWidth()-16,input_img.getHeight()-16));

    computeMaskImageRR(texture_mask,texture_edge,texture_edge);

    compute_copy_red_to_rgba(texture_edge,output_img);

    std::vector<LineEquation> equations = computeHoughLines(texture_edge,texture_hough_space);

    std::vector<LineEquationMC> eqns;

    for (auto eq:equations) {
        eqns.push_back(convertLineEquationPolarToMc(eq));
    }

    float center_x = (((eqns[0].c-eqns[1].c)/(eqns[1].m-eqns[0].m))+((eqns[2].c-eqns[1].c)/(eqns[1].m-eqns[2].m)))/2.0;

    float center_y = (eqns[0].m*center_x)+eqns[0].c;

    return glm::vec3(center_x,center_y,equations[0].theta);

    //computeGetVerticalGradientAngle()

//    static _Shader shader;
//    static GroupSize groupsize = getWorkGroupSize(input_img.getWidth(), input_img.getHeight());

//    //if shader not initialized
//    if(shader.getShaderProgram() == 0)
//    {
//        shader.setChildShader(":/shaders/compute_gradient_to_descrete_rgba_color.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
//        shader.attachShaders();
//        qDebug() << "shader initialized";
//    }

//    texture_sobel_mag_.bindForCompute(0,GL_R8UI,GL_READ_ONLY);
//    texture_sobel_theta_.bindForCompute(1,GL_R8UI,GL_READ_ONLY);
//    output_img.bindForCompute(2,GL_RGBA8UI,GL_WRITE_ONLY);

//    shader.useShaderProgram();

//    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

//    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void _GPU_Compute::compute_register_wrap_mesh(_Texture& texture_edge_bounds,_Texture& output_img,int rotation_step,glm::vec2 stage_center)
{
        static _Shader shader;
        static GroupSize groupsize = getWorkGroupSize(output_img.getWidth(),output_img.getHeight(),_ROW_WISE_LOCAL_GROUP);

        //if shader not initialized
        if(shader.getShaderProgram() == 0)
        {
            shader.setChildShader(":/shaders/compute_register_wrap_mesh.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
            shader.attachShaders();
            qDebug() << "shader initialized";
        }

        texture_edge_bounds.bindForCompute(0,GL_R32I,GL_READ_ONLY);
        output_img.bindForCompute(1,GL_R32I,GL_READ_WRITE);

        shader.useShaderProgram();

        glUniform2i(0,stage_center.x,stage_center.y);
        glUniform2i(1,output_img.getWidth(),output_img.getHeight());
        glUniform1i(2,rotation_step);
        glUniform2i(3,768,576);

        glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void _GPU_Compute::computeEdgeModel(_Texture& input_img,_Texture& output_img,_Texture& texture_model_wrap,_Texture& texture_out_8_bit,int rotation_step,glm::vec2 stage_center)
{
    _Texture texture_edge_bounds(nullptr,2,input_img.getHeight());
    _Texture texture_out_mask(nullptr,texture_model_wrap.getWidth(),texture_model_wrap.getHeight());
    //_Texture texture_out_8_bit(nullptr,texture_model_wrap.getWidth(),texture_model_wrap.getHeight());
    _Texture texture_mask(nullptr,input_img.getWidth(),input_img.getHeight());
    _Texture texture_mask_inv(nullptr,input_img.getWidth(),input_img.getHeight());
    _Texture texture_thres(nullptr,input_img.getWidth(),input_img.getHeight());
    _Texture texture_edge(nullptr,input_img.getWidth(),input_img.getHeight());
    _Texture texture_rgba(nullptr,input_img.getWidth(),input_img.getHeight());
    _Texture texture_out_rgba(nullptr,input_img.getWidth(),input_img.getHeight());
    _Texture texture_descrete_gradient_value(nullptr,input_img.getWidth(),input_img.getHeight());
    _Texture texture_max_extent(nullptr,input_img.getWidth(),input_img.getHeight());

    static char* colorFrame = nullptr;
    texture_edge_bounds.load(GL_R32I,GL_RED_INTEGER, GL_INT);
    texture_edge.load(GL_RED,GL_UNSIGNED_BYTE);
    texture_thres.load(GL_RED,GL_UNSIGNED_BYTE);
    texture_out_mask.load(GL_RED,GL_UNSIGNED_BYTE);
    texture_mask.load(GL_RED,GL_UNSIGNED_BYTE);
    texture_mask_inv.load(GL_RED,GL_UNSIGNED_BYTE);
    texture_descrete_gradient_value.load(GL_RED,GL_UNSIGNED_BYTE);
    texture_rgba.load(GL_RGBA,GL_UNSIGNED_BYTE);
    texture_out_rgba.load(GL_RGBA,GL_UNSIGNED_BYTE);
    texture_max_extent.load(GL_R32I,GL_RED_INTEGER, GL_INT);

    //compute_guassian_blur_5_5(input_img,texture_blur);
    //compute_canny_edge(input_img,texture_edge);
    //gpu_compute->compute_gradient_to_descrete_color(texture_in,texture_out);
    //gpu_compute->compute_copy_8_to_32(texture,texture_outt);
    //gpu_compute->compute_copy_32_to_8(texture_outt,texture_out);
    //gpu_compute->compute_sobel_edge(texture_in,texture_edge);
    //gpu_compute->compute_row_wise_arg_max(texture,texture_outt);
    compute_threshold_inv(input_img,texture_thres,100);

    create_region_image_mask(texture_mask,glm::ivec4(16,16,input_img.getWidth()-16,stage_center.y));

    computeMaskImageRR(texture_thres,texture_mask,texture_thres);

    compute_copy_red_to_rgba(input_img,output_img);

    compute_row_wise_left_edge(texture_thres,texture_max_extent);

    compute_mark_column_index(texture_max_extent,output_img);

    compute_copy_column_from_to(texture_max_extent,texture_edge_bounds,0,0);

    compute_row_wise_right_edge(texture_thres,texture_max_extent);

    compute_mark_column_index(texture_max_extent,output_img);

    compute_copy_column_from_to(texture_max_extent,texture_edge_bounds,0,1);

    //compute_mark_column_index(texture_edge_bounds,output_img);

    compute_register_wrap_mesh(texture_edge_bounds,texture_model_wrap,rotation_step,stage_center);

    computeFrom32iTo8uiDevide(texture_model_wrap,texture_out_8_bit,2);

    //compute_clear_8_ui_texture(texture_out_8_bit,100);

    create_region_image_mask(texture_out_mask,glm::ivec4(-1,68,texture_out_mask.getWidth(),460));

    //computeMaskImageRR(texture_out_8_bit,texture_out_mask,texture_out_8_bit);

    //compute_guassian_blur_5_5(texture_out_8_bit,texture_out_8_bit);


    if(rotation_step == 199)
    {
        computeMaskImageR32IR(texture_model_wrap,texture_out_mask,texture_model_wrap);

        int* wrap_frame = getTextureModelFramebuffer32I(texture_model_wrap,0);

        std::vector<double> vertsG;
        std::vector<unsigned int> indiceG;
        unsigned int index[4] = {0,0,0,0};

        glm::ivec2 resolution = glm::ivec2(texture_model_wrap.getWidth(),texture_model_wrap.getHeight());//wrap texture size

        for (unsigned int h = 0; h < resolution.y; h++) {
            for (unsigned int w = 0; w < resolution.x; w++) {

                glm::vec2 pixel_cord = glm::vec2(w,h);

                glm::vec2 texture_cord = glm::vec2((pixel_cord.x +0.5)/resolution.x,(pixel_cord.y +0.5)/resolution.y);

                int index = _Tools::indexFromPixelCordinates(pixel_cord,resolution);
                //texture_positions
                float r = wrap_frame[index];
                float theta = 2 * PI * w;


                vertsG.push_back(r*cos(theta));//x = s
                vertsG.push_back(r*sin(theta));//y = t
                vertsG.push_back(h);//z = 0.0
            }
        }

        glm::ivec2 step_size = glm::ivec2(1,1);

        glm::vec3 origin = glm::vec3(0,0,resolution.y/2);

        float volume = 0.0;

        for (unsigned int h = 0; h < resolution.y; h+=step_size.y) {
            for (unsigned int w = 0; w < resolution.x; w+=step_size.x) {

                glm::vec2 pixel_cord = glm::vec2(w,h);

                //indexes of neibhouring vertexes
                index[0] = _Tools::indexFromPixelCordinates(pixel_cord,resolution);
                index[1] = _Tools::indexFromPixelCordinates(pixel_cord+glm::vec2(step_size.x,0),resolution);
                index[2] = _Tools::indexFromPixelCordinates(pixel_cord+glm::vec2(step_size.x,step_size.y),resolution);
                index[3] = _Tools::indexFromPixelCordinates(pixel_cord+glm::vec2(0,step_size.y),resolution);

                if((pixel_cord.y < resolution.y))
                {
                    glm::ivec3 tri_verts_indexes[4] = {
                        _Tools::vertIndexesFromElementIndex(index[0]),
                        _Tools::vertIndexesFromElementIndex(index[1]),
                        _Tools::vertIndexesFromElementIndex(index[2]),
                        _Tools::vertIndexesFromElementIndex(index[3])
                    };
                    std::vector<glm::vec3> tri_verts = {
                        glm::vec3(vertsG[tri_verts_indexes[0].x],vertsG[tri_verts_indexes[0].y],vertsG[tri_verts_indexes[0].z]),
                        glm::vec3(vertsG[tri_verts_indexes[1].x],vertsG[tri_verts_indexes[1].y],vertsG[tri_verts_indexes[1].z]),
                        glm::vec3(vertsG[tri_verts_indexes[2].x],vertsG[tri_verts_indexes[2].y],vertsG[tri_verts_indexes[2].z]),
                        glm::vec3(vertsG[tri_verts_indexes[3].x],vertsG[tri_verts_indexes[3].y],vertsG[tri_verts_indexes[3].z])
                    };

                    volume += (glm::determinant(glm::mat3x3(tri_verts[0],tri_verts[1],tri_verts[2]))+glm::determinant(glm::mat3x3(tri_verts[0],tri_verts[2],tri_verts[3])));

                    //indexs of second triangle in quad
                }
            }
        }

        //volume = volume /6.0;
        glUniform2f(5,volume,volume);
        qDebug() << "volume : " << volume;
    }

    _Tools::SaveImageToPgm(getTextureImageFramebuffer(texture_model_wrap,GL_RED),texture_out_8_bit.getWidth(),texture_out_8_bit.getHeight(),"texture_wrap.pgm");

}

void _GPU_Compute::computeRegisterVoxelMesh(_Texture &input_img, _Texture &output_img, float cosine, int rotation_step, glm::vec2 stage_center)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(output_img.getWidth(),output_img.getHeight(),_DEFAULT_2D_GROUP);

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_register_voxel_mesh.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img.bindForCompute(0,GL_R8UI,GL_READ_ONLY);
    output_img.bindForCompute(1,GL_R8UI,GL_READ_WRITE);

    shader.useShaderProgram();

    glUniform2i(0,stage_center.x,stage_center.y);
    glUniform2i(1,output_img.getWidth(),output_img.getHeight());
    glUniform1i(2,rotation_step);
    glUniform2i(3,768,576);
    glUniform1f(4,cosine);

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void _GPU_Compute::computeVoxelsModel(_Texture& input_img,_Texture& output_img,_Texture& texture_model_wrap,_Texture& texture_out_8_bit,int rotation_step,glm::vec2 stage_center)
{
    _Texture texture_edge_bounds(nullptr,2,input_img.getHeight());
    _Texture texture_out_mask(nullptr,texture_model_wrap.getWidth(),texture_model_wrap.getHeight());
    //_Texture texture_out_8_bit(nullptr,texture_model_wrap.getWidth(),texture_model_wrap.getHeight());
    _Texture texture_mask(nullptr,input_img.getWidth(),input_img.getHeight());
    _Texture texture_mask_inv(nullptr,input_img.getWidth(),input_img.getHeight());
    _Texture texture_thres(nullptr,input_img.getWidth(),input_img.getHeight());
    _Texture texture_edge(nullptr,input_img.getWidth(),input_img.getHeight());
    _Texture texture_rgba(nullptr,input_img.getWidth(),input_img.getHeight());
    _Texture texture_out_rgba(nullptr,input_img.getWidth(),input_img.getHeight());
    _Texture texture_descrete_gradient_value(nullptr,input_img.getWidth(),input_img.getHeight());
    _Texture texture_max_extent(nullptr,input_img.getWidth(),input_img.getHeight());

    static std::vector<_Texture> texture_cyl_voxels(100, _Texture(nullptr,input_img.getWidth(),input_img.getHeight(),GL_RED,GL_UNSIGNED_BYTE));
    static bool init = true;

    if(init)
    {
        for (int i = 0; i < 100; ++i) {
            texture_cyl_voxels[i].load();
            compute_clear_8_ui_texture(texture_cyl_voxels[i],255);
        }
        init = false;
    }

    static char* colorFrame = nullptr;
    texture_edge_bounds.load(GL_R32I,GL_RED_INTEGER, GL_INT);
    texture_edge.load(GL_RED,GL_UNSIGNED_BYTE);
    texture_thres.load(GL_RED,GL_UNSIGNED_BYTE);
    texture_out_mask.load(GL_RED,GL_UNSIGNED_BYTE);
    texture_mask.load(GL_RED,GL_UNSIGNED_BYTE);
    texture_mask_inv.load(GL_RED,GL_UNSIGNED_BYTE);
    texture_descrete_gradient_value.load(GL_RED,GL_UNSIGNED_BYTE);
    texture_rgba.load(GL_RGBA,GL_UNSIGNED_BYTE);
    texture_out_rgba.load(GL_RGBA,GL_UNSIGNED_BYTE);
    texture_max_extent.load(GL_R32I,GL_RED_INTEGER, GL_INT);

    //compute_guassian_blur_5_5(input_img,texture_blur);
    //compute_canny_edge(input_img,texture_edge);
    //gpu_compute->compute_gradient_to_descrete_color(texture_in,texture_out);
    //gpu_compute->compute_copy_8_to_32(texture,texture_outt);
    //gpu_compute->compute_copy_32_to_8(texture_outt,texture_out);
    //gpu_compute->compute_sobel_edge(texture_in,texture_edge);
    //gpu_compute->compute_row_wise_arg_max(texture,texture_outt);

    compute_threshold_inv(input_img,texture_thres,100);

    create_region_image_mask(texture_mask,glm::ivec4(16,16,input_img.getWidth()-16,stage_center.y));

    computeMaskImageRR(texture_thres,texture_mask,texture_thres);

    for (int i = 0; i < 100; ++i) {
        float cosine = glm::cos(PI*i/100);
        int id = (i+rotation_step)%100;
        computeRegisterVoxelMesh(texture_thres,texture_cyl_voxels[id],cosine,rotation_step,stage_center);
    }

    for (int i = 0; i < 100; ++i) {

        float cosine = glm::cos(PI*i/100);

        compute_row_wise_left_edge(texture_cyl_voxels[i],texture_max_extent);

        //compute_mark_column_index(texture_max_extent,output_img);

        compute_subtract_column_from_value(texture_max_extent,texture_max_extent,stage_center.x);

        compute_copy_column_from_to(texture_max_extent,texture_model_wrap,0,i);

        compute_row_wise_right_edge(texture_cyl_voxels[i],texture_max_extent);

        //compute_mark_column_index(texture_max_extent,output_img);

        compute_subtract_value_from_column(texture_max_extent,texture_max_extent,stage_center.x);

        compute_copy_column_from_to(texture_max_extent,texture_model_wrap,0,(100+i)%200);
    }

    compute_copy_red_to_rgba(input_img,output_img);

    //computeFilterModelMesh(texture_model_wrap,texture_model_wrap,350);

//    create_region_image_mask(texture_mask,glm::ivec4(16,16,input_img.getWidth()-16,stage_center.y));

//    computeMaskImageRR(texture_thres,texture_mask,texture_thres);

//    compute_copy_red_to_rgba(input_img,output_img);

//    compute_row_wise_left_edge(texture_thres,texture_max_extent);

//    compute_mark_column_index(texture_max_extent,output_img);

//    compute_copy_column_from_to(texture_max_extent,texture_edge_bounds,0,0);

//    compute_row_wise_right_edge(texture_thres,texture_max_extent);

//    compute_mark_column_index(texture_max_extent,output_img);

//    compute_copy_column_from_to(texture_max_extent,texture_edge_bounds,0,1);

//    //compute_mark_column_index(texture_edge_bounds,output_img);

//    compute_register_wrap_mesh(texture_edge_bounds,texture_model_wrap,rotation_step,stage_center);

//    //compute_clear_8_ui_texture(texture_out_8_bit,100);

//    create_region_image_mask(texture_out_mask,glm::ivec4(-1,68,texture_out_mask.getWidth(),460));

//    computeMaskImageRR(texture_out_8_bit,texture_out_mask,texture_out_8_bit);

//    compute_guassian_blur_5_5(texture_out_8_bit,texture_out_8_bit);

//    computeMaskImageR32IR(texture_model_wrap,texture_out_mask,texture_model_wrap);

//    int* wrap_frame = getTextureModelFramebuffer32I(texture_model_wrap,0);

//    glm::ivec2 resolution = glm::ivec2(texture_model_wrap.getWidth(),texture_model_wrap.getHeight());//wrap texture size

//    glm::vec2 pixel_cordt = glm::vec2(77,275);

//    int indext = _Tools::indexFromPixelCordinates(pixel_cordt,resolution);
//    //texture_positions
//    int rt = wrap_frame[indext];

//    qDebug() << "out 32 int" << rt ;

}


void _GPU_Compute::computeFilterModelMesh(_Texture& input_img,_Texture& output_img,int value)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(input_img.getWidth(), input_img.getHeight(),_DEFAULT_2D_GROUP);

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_filter_model_mesh.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img.bindForCompute(0,GL_R32I,GL_READ_ONLY);
    output_img.bindForCompute(1,GL_R32I,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glUniform1i(0,value);

    //calculate size of workgroups based on image resolution here

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}


_GPU_Compute::LineEquationMC _GPU_Compute::convertLineEquationPolarToMc(_GPU_Compute::LineEquation eqn)
{
    LineEquationMC mceqn;
    mceqn.m = -1.0/glm::tan(eqn.theta);
    mceqn.c = eqn.r/glm::sin(eqn.theta);
    return mceqn;
}

void _GPU_Compute::compute_gradient_to_descrete_color(_Texture& input_img,_Texture& output_img)
{
    _Texture texture_sobel_mag_(nullptr,input_img.getWidth(),input_img.getHeight());
    _Texture texture_sobel_theta_(nullptr,input_img.getWidth(),input_img.getHeight());
    _Texture texture_blur(nullptr,input_img.getWidth(),input_img.getHeight());

    texture_sobel_mag_.load(GL_RED,GL_UNSIGNED_BYTE);
    texture_blur.load(GL_RED,GL_UNSIGNED_BYTE);
    texture_sobel_theta_.load(GL_RED,GL_UNSIGNED_BYTE);

    //compute_guassian_blur_5_5(input_img,texture_blur);
    compute_sobel_edge(input_img,texture_sobel_mag_,texture_sobel_theta_);

    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(input_img.getWidth(), input_img.getHeight());

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_gradient_to_descrete_rgba_color.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    texture_sobel_mag_.bindForCompute(0,GL_R8UI,GL_READ_ONLY);
    texture_sobel_theta_.bindForCompute(1,GL_R8UI,GL_READ_ONLY);
    output_img.bindForCompute(2,GL_RGBA8UI,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void _GPU_Compute::compute_canny_edge(_Texture& input_img,_Texture& output_img)
{
    _Texture texture_sobel_mag_(nullptr,input_img.getWidth(),input_img.getHeight());
    _Texture texture_sobel_theta_(nullptr,input_img.getWidth(),input_img.getHeight());
    _Texture texture_blur(nullptr,input_img.getWidth(),input_img.getHeight());

    texture_sobel_mag_.load(GL_RED,GL_UNSIGNED_BYTE);
    texture_blur.load(GL_RED,GL_UNSIGNED_BYTE);
    texture_sobel_theta_.load(GL_RED,GL_UNSIGNED_BYTE);

    //compute_guassian_blur_5_5(input_img,texture_blur);
    compute_sobel_edge(input_img,texture_sobel_mag_,texture_sobel_theta_);

    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(input_img.getWidth(), input_img.getHeight());

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_canny.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    texture_sobel_mag_.bindForCompute(0,GL_R8UI,GL_READ_ONLY);
    texture_sobel_theta_.bindForCompute(1,GL_R8UI,GL_READ_ONLY);
    output_img.bindForCompute(2,GL_R8UI,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void _GPU_Compute::compute_canny_edge_from_sobel(_Texture& texture_sobel_mag_,_Texture& texture_sobel_theta_,_Texture& output_img)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(texture_sobel_mag_.getWidth(), texture_sobel_mag_.getHeight());

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_canny.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    texture_sobel_mag_.bindForCompute(0,GL_R8UI,GL_READ_ONLY);
    texture_sobel_theta_.bindForCompute(1,GL_R8UI,GL_READ_ONLY);
    output_img.bindForCompute(2,GL_R8UI,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

char* _GPU_Compute::get_texture_image_framebuffer(_Texture& input_img,unsigned int format)
{
    static unsigned int framebuffer=0,renderbuffer=0;
    static char* colorFrame=nullptr;

    if(!framebuffer)
    {
        glGenFramebuffers(1,&framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);

        glGenRenderbuffers(1,&renderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT16,MAX_FRAME_WIDTH, MAX_FRAME_HEIGHT);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER, renderbuffer);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) qDebug() << "fbo complete";
        else qDebug() << "incomplete";

        colorFrame = new char[MAX_FRAME_WIDTH*MAX_FRAME_HEIGHT*4];
    }

    glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);

    glViewport(0, 0, input_img.getWidth(), input_img.getHeight());

    input_img.bindForFramebuffer();

    if(format==0) format = input_img.getColorformat();

    //glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_BYTE, colorFrame);

    glReadPixels(0, 0, input_img.getWidth(), input_img.getHeight(),format, GL_UNSIGNED_BYTE,colorFrame);

    return colorFrame;
}

char* _GPU_Compute::getTextureImageFramebuffer(_Texture& input_img,unsigned int format)
{
    static unsigned int framebuffer=0,renderbuffer=0;
    static char* colorFrame=nullptr;

    if(!framebuffer)
    {
        glGenFramebuffers(1,&framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);

        glGenRenderbuffers(1,&renderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT16,MAX_FRAME_WIDTH, MAX_FRAME_HEIGHT);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER, renderbuffer);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) qDebug() << "fbo complete";
        else qDebug() << "incomplete";

        colorFrame = new char[MAX_FRAME_WIDTH*MAX_FRAME_HEIGHT*4];
    }

    glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);

    glViewport(0, 0, input_img.getWidth(), input_img.getHeight());

    input_img.bindForFramebuffer();

    if(format==0) format = input_img.getColorformat();

    glReadPixels(0, 0, input_img.getWidth(), input_img.getHeight(),format, GL_UNSIGNED_BYTE,colorFrame);

    return colorFrame;
}

char* _GPU_Compute::getTextureModelFramebuffer(_Texture& input_img,unsigned int format)
{
    static unsigned int framebuffer=0,renderbuffer=0;
    static char* colorFrame=nullptr;

    if(!framebuffer)
    {
        glGenFramebuffers(1,&framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);

        glGenRenderbuffers(1,&renderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT16,MAX_FRAME_WIDTH, MAX_FRAME_HEIGHT);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER, renderbuffer);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) qDebug() << "fbo complete";
        else qDebug() << "incomplete";

        colorFrame = new char[MAX_FRAME_WIDTH*MAX_FRAME_HEIGHT*4];
    }

    glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);

    glViewport(0, 0, input_img.getWidth(), input_img.getHeight());

    input_img.bindForFramebuffer();

    if(format==0) format = input_img.getColorformat();

    glReadPixels(0, 0, input_img.getWidth(), input_img.getHeight(),format, input_img.getDataType(),colorFrame);

    return colorFrame;
}

void _GPU_Compute::compute_subtract_value_from_column(_Texture& input_img,_Texture& output_img,int value)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(1, input_img.getHeight(),_COLUMN_WISE_LOCAL_GROUP);

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_subtract_value_from_column.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img.bindForCompute(0,GL_R32I,GL_READ_ONLY);
    output_img.bindForCompute(1,GL_R32I,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glUniform1i(0,value);

    //calculate size of workgroups based on image resolution here

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void _GPU_Compute::compute_subtract_column_from_value(_Texture& input_img,_Texture& output_img,int value)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(1, input_img.getHeight(),_COLUMN_WISE_LOCAL_GROUP);

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_subtract_column_from_value.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img.bindForCompute(0,GL_R32I,GL_READ_ONLY);
    output_img.bindForCompute(1,GL_R32I,GL_WRITE_ONLY);

    shader.useShaderProgram();

    glUniform1i(0,value);

    //calculate size of workgroups based on image resolution here

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void _GPU_Compute::compute_retrive_lower_2_bytes(_Texture& input_img,_Texture& output_img)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(1, input_img.getHeight(),_COLUMN_WISE_LOCAL_GROUP);

    //if shader not initialized
    if(shader.getShaderProgram() == 0)
    {
        shader.setChildShader(":/shaders/compute_retrive_lower_2_bytes.glsl",GL_COMPUTE_SHADER,groupsize.WorkGroupSize);
        shader.attachShaders();
        qDebug() << "shader initialized";
    }

    input_img.bindForCompute(0,GL_R32I,GL_READ_ONLY);
    output_img.bindForCompute(1,GL_R32I,GL_WRITE_ONLY);

    shader.useShaderProgram();

    //calculate size of workgroups based on image resolution here

    glDispatchCompute(groupsize.NumWorkGroups.x,groupsize.NumWorkGroups.y,groupsize.NumWorkGroups.z);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

int* _GPU_Compute::getTextureModelFramebuffer32I(_Texture& input_img,unsigned int format)
{
    static unsigned int framebuffer=0,renderbuffer=0;
    static int* colorFrame=nullptr;

    if(!framebuffer)
    {
        glGenFramebuffers(1,&framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);

        glGenRenderbuffers(1,&renderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT16,MAX_FRAME_WIDTH, MAX_FRAME_HEIGHT);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER, renderbuffer);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) qDebug() << "fbo complete";
        else qDebug() << "incomplete";

        colorFrame = new int[input_img.getWidth()*input_img.getHeight()];
    }

    glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);

    glViewport(0, 0, input_img.getWidth(), input_img.getHeight());

    input_img.bindForFramebuffer();

    if(format==0) format = input_img.getColorformat();

    glReadPixels(0, 0, input_img.getWidth(), input_img.getHeight(),GL_RED_INTEGER, GL_INT,colorFrame);

    return colorFrame;
}
