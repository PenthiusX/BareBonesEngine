#ifndef _GPU_COMPUTE_H
#define _GPU_COMPUTE_H

#include <QOpenGLExtraFunctions>
#include <_texture.h>
#include <glm/glm.hpp>

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
    float compute_stage_angle(_Texture &input_img, _Texture &output_img);
    char *get_texture_image_framebuffer(_Texture &input_img);
    //void compute_gradient_to_descrete_color(_Texture &input_mag, _Texture &input_theta, _Texture &output_img);
    //void compute_gradient_to_descrete_color(_Texture &input_img, _Texture &output_img);
    void compute_mask_image_rgba_r(_Texture &input_img, _Texture &mask_img, _Texture &output_img);
    void create_region_image_mask(_Texture &output_img,glm::ivec4 region);
    void compute_gradient_to_descrete_color(_Texture &texture_sobel_mag_, _Texture &texture_sobel_theta_, _Texture &output_img);
    void compute_canny_edge_from_sobel(_Texture &texture_sobel_mag_, _Texture &texture_sobel_theta_, _Texture &output_img);

    void compute_gradient_to_descrete_color(_Texture &input_img, _Texture &output_img);
    void compute_mix_with_mask_rgba_r_rgba(_Texture &input_img_1, _Texture &mask_img, _Texture &input_img_2, _Texture &output_img);
};

#endif // _GPU_COMPUTE_H
