#include "_camera.h"

/*
 * Class: _Camera
 * class sets values needed by the Viewmatrix
 * for setting of values on Init or realtime
 * Created: 01_03_2019
 * Author: Aditya
*/

/* 
* Constructor:_Camera()
* sets an the defalut values for the camera object 
* on init. 
* Destructor: ~_Camera();
*/
_Camera::_Camera(){
    this->upVector = QVector3D(0.0, 1.0, 0.0);
    this->eyePosition = QVector3D(0.0, 0.0, -10.0);
    this->focalPoint = QVector3D(0.0, 0.0, 0.0);
}
_Camera::~_Camera(){

}
/*
* Function: set/get Eye postition(QVector3D epos)
* sets and gets the postion for the camera object 
* in opengl space. these values get fed into the renderer 
* into the viewMatrix.
* Created: 01_03_2019
*/
void _Camera::setEyePosition(QVector3D epos)
{
    this->eyePosition = epos;
}
QVector3D _Camera::getEyePosition()
{
    return this->eyePosition;
}
/*
* Function: set/get FocalPoint()
* sets and returns the point the camera is looking at.
* Created: 01_03_2019
*/
void _Camera::setFocalPoint(QVector3D focalPos)
{
    this->focalPoint = focalPos;
}
QVector3D _Camera::getFocalPoint()
{
    return this->focalPoint;
}
/*
* Function: set/get UpVector()
* sets the up vector for the camera
* the direction in which the camera orents itself.
* Created: 01_03_2019
*/
void _Camera::setUpVector(QVector3D upVec)
{
    this->upVector = upVec;
}
QVector3D _Camera::getUpVector()
{
    return this->upVector;
}

/*
 * Function:get/setFOV
 * gets sets the variable used in
 * field of view setting
 * Created: 08_04_2019
*/
void _Camera::setFOV(unsigned int fove)
{
    this->fov = fove;
}

unsigned int _Camera::getFOV()
{
    return this->fov;
}
