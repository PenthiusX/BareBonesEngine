#include "_hwdcamera.h"
#include <QDebug>
_HWDCamera::_HWDCamera()
{
    qDebug() <<"fallback camera contructor";

}

int _HWDCamera::init(int v)
{
    return 0;
}

char *_HWDCamera::get_frame()
{
    return 0;
}

int _HWDCamera::stop_camera()
{
    return 0;
}

int _HWDCamera::list_cameras()
{
    return 0;
}

int _HWDCamera::grab_frame(QString filename)
{
    qDebug() <<"fallback camera grabframe";
    return 0;
}

int _HWDCamera::select_camera(int v)
{
    return 0;
}

int _HWDCamera::camera_setup_defaults()
{
    return 0;
}
