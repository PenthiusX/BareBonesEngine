#ifndef _CAMERA_H
#define _CAMERA_H
#include <QVector3D>
#include <glm/glm.hpp>

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
    void setFocalPoint(glm::vec3 focalPos);
    glm::vec3 getFocalPoint()const;
    void setUpVector(QVector3D upVec);
    QVector3D getUpVector()const;
    void setFOV(unsigned fov);
    uint getFOV()const;

    void setFarClipDistance(float fclp);
    void setNearClipDistance(float nclp);
    float getNearClipDistance()const;
    float getFarClipDistance()const;

private:
    QVector3D eyePosition;
    glm::vec3 focalPoint;
    QVector3D upVector;
    uint fov;
    float nearClipDistance;
    float farClipDistance;
};

#endif // _CAMERA_H
