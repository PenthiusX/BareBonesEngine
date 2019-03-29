#ifndef __HWDCAMERA_H
#define __HWDCAMERA_H

#include <QString>
#include <QJsonObject>

class _HWDCamera
{
public:
    _HWDCamera();
    _HWDCamera(QJsonObject cnfg);
    virtual int init(int v); //initialize camera
    virtual char* get_frame(); //return frame
    virtual int stop_camera(); //
    virtual int list_cameras();
    virtual int grab_frame(QString filename);
    virtual void set_image_dir(QString);


protected:
    char *byteframe = nullptr;
    char *image_header = nullptr;
    FILE* imagefile;
    QString images_dir;

private:
    virtual int select_camera(int v);
    virtual int camera_setup_defaults();
    QJsonObject config;

};

#endif // _HWDCAMERA_H
