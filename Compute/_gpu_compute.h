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

    void compute_sobel_edge(_Texture& input_img, _Texture& output_img);
    void compute_invert(_Texture &input_img, _Texture &output_img);
    void compute_canny_edge(_Texture &input_img, _Texture &output_img);
    void compute_sobel_edge(_Texture &input_img, _Texture &output_mag, _Texture &output_theta);
    void compute_guassian_blur_3_3(_Texture &input_img, _Texture &output_img);
    void compute_guassian_blur_5_5(_Texture &input_img, _Texture &output_img);
    void compute_threshold(_Texture &input_img, _Texture &output_img);
    void compute_copy_8_to_32(_Texture &input_img, _Texture &output_img);
    void compute_copy_32_to_8(_Texture &input_img, _Texture &output_img);
    GroupSize getWorkGroupSize(int w,int h,COMPUTE_OPERTION operation = _DEFAULT_2D_GROUP);
};

#endif // _GPU_COMPUTE_H
