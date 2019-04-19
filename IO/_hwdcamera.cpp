#include "_hwdcamera.h"
#include <QDebug>
#include <QDir>

_HWDCamera::_HWDCamera()
{
    qDebug() <<"fallback camera contructor";
}

_HWDCamera::_HWDCamera(QJsonObject cnfg) : config(cnfg)
{
    qDebug() <<"fallback camera contructor";
}

_HWDCamera::~_HWDCamera()
{
    stop_camera();
    //delete byteframe;
}
int _HWDCamera::init(int v)
{
    byteframe = new char[Width*Height];
    image_header = new char[17];
    return 0;
}

char *_HWDCamera::get_frame()
{
    return byteframe;
}

int _HWDCamera::stop_camera()
{
    return 0;
}

int _HWDCamera::list_cameras()
{
    return 0;
}

int _HWDCamera::grab_frame()
{
    return 0;
}

int _HWDCamera::grab_frame(QString filename)
{
    //qDebug() <<"fallback camera grabframe";

    filename = images_dir+filename;

    imagefile=fopen(filename.toLocal8Bit(), "r");

    if( imagefile == NULL) {
        qDebug() << "Can't open:" << filename;
    }

    fread(image_header, 17, 1, imagefile);
    fread(byteframe, Width*Height, 1, imagefile);
    fclose(imagefile);

    qDebug() << "opened avt: " << filename;
    return 0;
}

void _HWDCamera::set_image_dir(QString dir)
{
    images_dir = dir+"/";
}

void _HWDCamera::setBrightness(int value)
{

}

void _HWDCamera::setGain(int value)
{

}

void _HWDCamera::setExposure(int value)
{

}

void _HWDCamera::setContrast(int value)
{

}

int _HWDCamera::getWidth() const
{
    return Width;
}

int _HWDCamera::getHeight() const
{
    return Height;
}

int _HWDCamera::select_camera(int v)
{
    return 0;
}

int _HWDCamera::camera_setup_defaults()
{
    return 0;
}
