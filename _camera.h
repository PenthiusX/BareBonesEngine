#ifndef _CAMERA_H
#define _CAMERA_H
#include <QVector3D>
#include <glm/glm.hpp>

/*
 * Class: _Camera
 * class sets values needed by the Viewmatrix
 * for setting of values on Init or realtime
 * Author: Aditya
*/

class _Camera
{
public:
    //
    _Camera();
    ~_Camera();
    //
    void setEyePosition(glm::vec3 epos);
    glm::vec3 getEyePosition()const;
    void setFocalPoint(glm::vec3 focalPos);
    glm::vec3 getFocalPoint()const;
    void setUpVector(glm::vec3 upVec);
    glm::vec3 getUpVector()const;
    void setFOV(unsigned fov);
    uint getFOV()const;
    glm::vec3 getFrontVector();

    void setFarClipDistance(float fclp);
    void setNearClipDistance(float nclp);
    float getNearClipDistance()const;
    float getFarClipDistance()const;

private:
    glm::vec3 eyePosition;
    glm::vec3 focalPoint;
    glm::vec3 upVector;
    glm::vec3 frontVector;
    uint fov;
    float nearClipDistance;
    float farClipDistance;
};

#endif // _CAMERA_H
