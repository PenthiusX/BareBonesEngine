#include "_gpu_compute.h"
#include <_shader.h>
#include <QDebug>
#include <_tools.h>


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


void _GPU_Compute::compute_row_wise_mean(_Texture& input_img,_Texture& output_img)
{
    static _Shader shader;
    static GroupSize groupsize = getWorkGroupSize(input_img.getWidth()/2, input_img.getHeight(),_ROW_WISE_LOCAL_GROUP);

    static _Texture sigma_x_dot_y = _Texture(nullptr,input_img.getWidth(),input_img.getHeight());
    static _Texture sigma_y = _Texture(nullptr,input_img.getWidth(),input_img.getHeight());

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

float _GPU_Compute::compute_stage_angle(_Texture& input_img,_Texture& output_img)
{
    static _Texture texture_sobel_mag_(nullptr,input_img.getWidth(),input_img.getHeight());
    static _Texture texture_sobel_theta_(nullptr,input_img.getWidth(),input_img.getHeight());
    static _Texture texture_mask(nullptr,input_img.getWidth(),input_img.getHeight());
    static _Texture texture_mask_inv(nullptr,input_img.getWidth(),input_img.getHeight());
    static _Texture texture_edge(nullptr,input_img.getWidth(),input_img.getHeight());
    static _Texture texture_rgba(nullptr,input_img.getWidth(),input_img.getHeight());
    static _Texture texture_out_rgba(nullptr,input_img.getWidth(),input_img.getHeight());

    texture_sobel_mag_.load(GL_RED,GL_UNSIGNED_BYTE);
    texture_edge.load(GL_RED,GL_UNSIGNED_BYTE);
    texture_sobel_theta_.load(GL_RED,GL_UNSIGNED_BYTE);
    texture_mask.load(GL_RED,GL_UNSIGNED_BYTE);
    texture_mask_inv.load(GL_RED,GL_UNSIGNED_BYTE);
    texture_rgba.load(GL_RGBA,GL_UNSIGNED_BYTE);
    texture_out_rgba.load(GL_RGBA,GL_UNSIGNED_BYTE);

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

    compute_gradient_to_descrete_color(texture_sobel_mag_,texture_sobel_theta_,output_img);

    create_region_image_mask(texture_mask,glm::ivec4(16,16,input_img.getWidth()-16,input_img.getHeight()-16));

    compute_mask_image_rgba_r(output_img,texture_mask,texture_out_rgba);

    compute_threshold(texture_edge,texture_mask,60);

    compute_mask_image_rgba_r(texture_out_rgba,texture_mask,texture_rgba);

    compute_copy_red_to_rgba(input_img,output_img);

    compute_mix_with_mask_rgba_r_rgba(texture_rgba,texture_mask,output_img,output_img);


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
    return 0.0;
}

void _GPU_Compute::compute_gradient_to_descrete_color(_Texture& input_img,_Texture& output_img)
{
    static _Texture texture_sobel_mag_(nullptr,input_img.getWidth(),input_img.getHeight());
    static _Texture texture_sobel_theta_(nullptr,input_img.getWidth(),input_img.getHeight());
    static _Texture texture_blur(nullptr,input_img.getWidth(),input_img.getHeight());

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
    static _Texture texture_sobel_mag_(nullptr,input_img.getWidth(),input_img.getHeight());
    static _Texture texture_sobel_theta_(nullptr,input_img.getWidth(),input_img.getHeight());
    static _Texture texture_blur(nullptr,input_img.getWidth(),input_img.getHeight());

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
char* _GPU_Compute::get_texture_image_framebuffer(_Texture& input_img)
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
    unsigned int format = input_img.getColorformat();

    glReadPixels(0, 0, input_img.getWidth(), input_img.getHeight(),format, GL_UNSIGNED_BYTE,colorFrame);

    format = GL_RED;

    return colorFrame;
}