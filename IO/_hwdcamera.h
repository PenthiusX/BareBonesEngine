#ifndef __HWDCAMERA_H
#define __HWDCAMERA_H

#include <QString>
#include <QJsonObject>


class _HWDCamera
{
public:
    _HWDCamera();
    _HWDCamera(QJsonObject cnfg);
    virtual ~_HWDCamera();
    virtual int init(int v); //initialize camera
    virtual char* get_frame(); //return frame
    virtual int stop_camera(); //
    virtual int list_cameras();
    virtual int grab_frame(QString filename);
    virtual int grab_frame();
    virtual void set_image_dir(QString);

    virtual void setBrightness(int value);
    virtual void setGain(int value);
    virtual void setExposure(int value);
    virtual void setContrast(int value);
    virtual void setOffset(int value);

    int getWidth() const;
    int getHeight() const;

protected:
    char *byteframe = nullptr;
    char *image_header = nullptr;
    int Width = MAX_FRAME_WIDTH;
    int Height = MAX_FRAME_HEIGHT;
    FILE* imagefile;
    QString images_dir;

private:
    virtual int select_camera(int v);
    virtual int camera_setup_defaults();
    QJsonObject config;

};

#endif // _HWDCAMERA_H
