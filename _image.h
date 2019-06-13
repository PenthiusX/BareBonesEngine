#ifndef _IMAGE_H
#define _IMAGE_H

#include <_texture.h>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

class _Image : public cv::Mat
{
    enum LastOperation
    {
        _CV = 0,
        _GPU = 1
    };

public:
    _Image();
    void sync();
    void setLastOperation(LastOperation op){sync_dir_rev=op;}

    void *dataFromGPUTexture();
protected:
    LastOperation sync_dir_rev=_CV;

};

#endif // _IMAGE_H
