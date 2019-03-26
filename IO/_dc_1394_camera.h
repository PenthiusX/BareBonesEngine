#ifndef _DC_1394_CAMERA_H
#define _DC_1394_CAMERA_H

#include <IO/_hwdcamera.h>
#include <QString>

#   ifdef PLATFORM_LINUX

#include <dc1394/dc1394.h>
class _DC_1394_Camera : public _HWDCamera
{
public:
    _DC_1394_Camera();
    ~_DC_1394_Camera();
    int init(int v) override;
    char* get_frame() override;
    int stop_camera() override;
    int list_cameras() override;
    int grab_frame(QString filename) override;
    int grab_frame() override;

private:
    dc1394camera_t *camera;
    unsigned int width, height;
    dc1394video_frame_t *frame=NULL;
    dc1394featureset_t features;
    dc1394video_modes_t video_modes;
    dc1394video_mode_t video_mode;
    dc1394color_coding_t coding;
    dc1394_t * d;
    dc1394camera_list_t * list;
    dc1394error_t err;


    void cleanup_and_exit(dc1394camera_t *camera);

    int select_camera(int v) override;
    int camera_setup_defaults() override;
    int success = 1;
};
#   endif //PLATFORM_LINUX

#endif // _DC_1394_CAMERA_H
