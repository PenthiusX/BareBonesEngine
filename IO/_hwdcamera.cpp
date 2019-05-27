#include "_hwdcamera.h"
#include <QDebug>
#include <QDir>
#include <fstream>
#include <sstream>
#include <string>

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

/* Function : grab_frame(QString filename)\
 * this function read image from .pgm file when no camera is connected
 * fisrt lie contains "P5"
 * the second line of .pgm file contains "<width> <height> <intensity_max>"
 * eg:
 *      P5
 *      1360 1024 255
 *      <binary_image_data>
 *
*/
int _HWDCamera::grab_frame(QString filename)
{
    //qDebug() <<"fallback camera grabframe";

    filename = images_dir+filename;

    imagefile=fopen(filename.toLocal8Bit(), "r");

    if( imagefile == NULL) {
        qDebug() << "Can't open:" << filename;
        //fclose(imagefile);
        return 0;
    }
    char line[30];
    fgets(line,30,imagefile);
    fgets(line,30,imagefile);
    QString resolution_line = QString::fromUtf8(line);
    QStringList resolution = resolution_line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    Width = resolution[0].toInt();
    Height = resolution[1].toInt();

    fread(byteframe, Width*Height, 1, imagefile);
    fclose(imagefile);

    return 1;
}

void _HWDCamera::setImageDir(QString dir)
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

void _HWDCamera::setOffset(int value)
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
