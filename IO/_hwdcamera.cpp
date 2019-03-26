#include "_camera.h"
#include <QDebug>
_Camera::_Camera()
{
    qDebug() <<"fallback camera contructor";

}

int _Camera::init(int v)
{
    return 0;
}

char *_Camera::get_frame()
{
    return 0;
}

int _Camera::stop_camera()
{
    return 0;
}

int _Camera::list_cameras()
{
    return 0;
}

int _Camera::grab_frame(QString filename)
{
    qDebug() <<"fallback camera grabframe";
    return 0;
}

int _Camera::select_camera(int v)
{
    return 0;
}

int _Camera::camera_setup_defaults()
{
    return 0;
}
