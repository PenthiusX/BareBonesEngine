#ifndef _CAMERA_H
#define _CAMERA_H
#include <QVector3D>

/*
 * Class: _Camera
 * class sets values needed by the Viewmatrix
 * for setting of values on Init or realtime
 * Created: 01_03_2019
 * Author: Aditya
*/

class _Camera
{
public:
    //
    _Camera();
    ~_Camera();
    //
    void setEyePosition(QVector3D epos);
    QVector3D getEyePosition()const;
    void setFocalPoint(QVector3D focalPos);
    QVector3D getFocalPoint()const;
    void setUpVector(QVector3D upVec);
    QVector3D getUpVector()const;
    void setFOV(unsigned fov);
    unsigned int getFOV()const;

    void setFarClipDistance(float fclp);
    void setNearClipDistance(float nclp);
    float getNearClipDistance()const;
    float getFarClipDistance()const;

private:
    QVector3D eyePosition;
    QVector3D focalPoint;
    QVector3D upVector;
    unsigned int fov;
    float nearClipDistance;
    float farClipDistance;
};

#endif // _CAMERA_H
