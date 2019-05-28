#ifndef _GPU_COMPUTE_H
#define _GPU_COMPUTE_H

#include <QOpenGLExtraFunctions>
#include <_texture.h>
#include <glm/glm.hpp>
#include <_texturepool.h>

class _GPU_Compute : private QOpenGLExtraFunctions
{
    enum COMPUTE_OPERTION
    {
        _DEFAULT_2D_GROUP = 0,//workgroup is a 2d rectangle with certain resolution,use for image mapping operations
        _ROW_WISE_LOCAL_GROUP ,//whole row is in same workgroup, to use row wise barriers
        _COLUMN_WISE_LOCAL_GROUP//whole column is in same workgroup,to use column wise barriers
    };
    struct GroupSize
    {
        glm::vec3 NumWorkGroups;
        glm::vec3 WorkGroupSize;
    };

    struct LineEquation
    {
        int r;
        float theta;
    };

    struct LineEquationMC
    {
        float m;
        float c;
    };

public:
    _GPU_Compute();

    GroupSize getWorkGroupSize(int w,int h,COMPUTE_OPERTION operation = _DEFAULT_2D_GROUP);

    void compute_sobel_edge(_Texture& input_img, _Texture& output_img);
    void compute_invert(_Texture &input_img, _Texture &output_img);
    void compute_canny_edge(_Texture &input_img, _Texture &output_img);
    void compute_sobel_edge(_Texture &input_img, _Texture &output_mag, _Texture &output_theta);
    void compute_guassian_blur_3_3(_Texture &input_img, _Texture &output_img);
    void compute_guassian_blur_5_5(_Texture &input_img, _Texture &output_img);
    void compute_threshold(_Texture &input_img, _Texture &output_img,unsigned int threshold_value=127);
    void compute_copy_8_to_32(_Texture &input_img, _Texture &output_img);
    void compute_copy_32_to_8(_Texture &input_img, _Texture &output_img);
    void compute_row_wise_max(_Texture &input_img, _Texture &output_img);
    void compute_row_wise_arg_max(_Texture &input_img, _Texture &output_img);
    void compute_mark_column_index(_Texture &input_img, _Texture &output_img);
    void compute_clear_8_ui_texture(_Texture &input_img, unsigned int value=0);
    void compute_row_wise_sum(_Texture &input_img, _Texture &output_img);
    void compute_row_wise_mean(_Texture &input_img, _Texture &output_img);
    void compute_divide_mark_column_index(_Texture &input_img, _Texture &input_img2, _Texture &output_img);
    void compute_copy_red_to_rgba(_Texture &input_img, _Texture &output_img);
    glm::vec3 compute_stage_angle(_Texture &input_img, _Texture &output_img);
    char *get_texture_image_framebuffer(_Texture &input_img,unsigned int format=0);
    //void compute_gradient_to_descrete_color(_Texture &input_mag, _Texture &input_theta, _Texture &output_img);
    //void compute_gradient_to_descrete_color(_Texture &input_img, _Texture &output_img);
    void compute_mask_image_rgba_r(_Texture &input_img, _Texture &mask_img, _Texture &output_img);
    void create_region_image_mask(_Texture &output_img,glm::ivec4 region);
    void compute_gradient_to_descrete_color(_Texture &texture_sobel_mag_, _Texture &texture_sobel_theta_, _Texture &output_img);
    void compute_canny_edge_from_sobel(_Texture &texture_sobel_mag_, _Texture &texture_sobel_theta_, _Texture &output_img);

    void compute_gradient_to_descrete_color(_Texture &input_img, _Texture &output_img);
    void compute_mix_with_mask_rgba_r_rgba(_Texture &input_img_1, _Texture &mask_img, _Texture &input_img_2, _Texture &output_img);
    void compute_saturate_color(_Texture &input_img, _Texture &output_img);
    void computeGradientToDescreteColorAndValue(_Texture &texture_sobel_mag_, _Texture &texture_sobel_theta_, _Texture &output_img, _Texture &output_img_value);
    void computeMaskImageRR(_Texture &input_img, _Texture &mask_img, _Texture &output_img);
    //std::vector<LineEquation> computeHoughLines(_Texture &texture_sobel_mag_);
    void computeMarkHoughWave(_Texture &output_img, glm::ivec2 cordinate);
    std::vector<_GPU_Compute::LineEquation> computeHoughLines(_Texture &texture_edge, _Texture &texture_hough_space);
    char *getTextureImageFramebuffer(_Texture &input_img, unsigned int format=0);
    void compute_clear_32_i_texture(_Texture &input_img, unsigned int value);
    void computeFrom32iTo8uiDevide(_Texture &input_img, _Texture &output_img, unsigned int devide_value=1);

    LineEquationMC convertLineEquationPolarToMc(_GPU_Compute::LineEquation eqn);
    //void computeEdgeModel(_Texture &input_img, _Texture &output_img, _Texture &texture_model_wrap,int rotation_step,glm::vec2 stage_center=glm::vec2(400,480));
    void compute_threshold_inv(_Texture &input_img, _Texture &output_img, unsigned int threshold_value=127);
    void compute_row_wise_left_edge(_Texture &input_img, _Texture &output_img);
    void compute_row_wise_right_edge(_Texture &input_img, _Texture &output_img);
    void compute_copy_column_from_to(_Texture &input_img, _Texture &output_img, int from_ = 0, int to_ = 0);
    void compute_register_wrap_mesh(_Texture &texture_edge_bounds, _Texture &output_img, int rotation_step,glm::vec2 stage_center);
    char *getTextureModelFramebuffer(_Texture &input_img, unsigned int format);
    void computeEdgeModel(_Texture &input_img, _Texture &output_img, _Texture &texture_model_wrap, _Texture &texture_out_8_bit, int rotation_step, glm::vec2 stage_center);
    void compute_subtract_value_from_column(_Texture &input_img, _Texture &output_img,int value = 400);
    void compute_retrive_lower_2_bytes(_Texture &input_img, _Texture &output_img);
    int *getTextureModelFramebuffer32I(_Texture &input_img, unsigned int format);
    void computeMaskImageR32IR(_Texture &input_img, _Texture &mask_img, _Texture &output_img);

    void compute_threshold(_Texture *input_img, _Texture *output_img, unsigned int threshold_value=127);
protected:
    _TexturePool texture_pool_8_bit;

};

#endif // _GPU_COMPUTE_H
