#ifndef _CAMERA_H
#define _CAMERA_H
#include <QVector3D>

class _Camera
{
public:
    _Camera();
    ~_Camera();

    void setEyePosition(QVector3D epos);
    QVector3D getEyePosition();
    void setFocalPoint(QVector3D focalPos);
    QVector3D getFocalPoint();
    void setUpVector(QVector3D upVec);
    QVector3D getUpVector();

private:
 QVector3D eyePosition;
 QVector3D focalPoint;
 QVector3D upVector;
};

#endif // _CAMERA_H