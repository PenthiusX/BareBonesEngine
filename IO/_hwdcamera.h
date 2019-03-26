#ifndef __HWDCAMERA_H
#define __HWDCAMERA_H

#include <QString>

class _HWDCamera
{
public:
    _HWDCamera();
    virtual int init(int v); //initialize camera
    virtual char* get_frame(); //return frame
    virtual int stop_camera(); //
    virtual int list_cameras();
    virtual int grab_frame(QString filename);

private:
    virtual int select_camera(int v);
    virtual int camera_setup_defaults();
};

#endif // _HWDCAMERA_H
