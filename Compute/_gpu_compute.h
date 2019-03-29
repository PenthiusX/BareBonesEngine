#ifndef _GPU_COMPUTE_H
#define _GPU_COMPUTE_H

#include <QOpenGLExtraFunctions>
#include <_texture.h>

class _GPU_Compute : private QOpenGLExtraFunctions
{
public:
    _GPU_Compute();

    void compute_sobel_edge(_Texture& input_img, _Texture& output_img);
    void compute_invert(_Texture &input_img, _Texture &output_img);
    void compute_canny_edge(_Texture &input_img, _Texture &output_img);
    void compute_sobel_edge(_Texture &input_img, _Texture &output_mag, _Texture &output_theta);
    void compute_guassian_blur_3_3(_Texture &input_img, _Texture &output_img);
    void compute_guassian_blur_5_5(_Texture &input_img, _Texture &output_img);
    void compute_threshold(_Texture &input_img, _Texture &output_img);
};

#endif // _GPU_COMPUTE_H
