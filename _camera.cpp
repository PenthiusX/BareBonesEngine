#include "_camera.h"

_Camera::_Camera(){
	this->upVector = QVector3D(0.0, 1.0, 0.0);
	this->eyePosition = QVector3D(0.0, 0.0, -10.0);
	this->focalPoint = QVector3D(0.0, 0.0, 0.0);
}
_Camera::~_Camera(){
}
/**/
void _Camera::setEyePosition(QVector3D epos)
{
	this->eyePosition = epos;
}
QVector3D _Camera::getEyePosition()
{
	return this->eyePosition;
}
/**/
void _Camera::setFocalPoint(QVector3D focalPos)
{
	this->focalPoint = focalPos;
}
QVector3D _Camera::getFocalPoint()
{
	return this->focalPoint;
}
/**/
void _Camera::setUpVector(QVector3D upVec)
{
	this->upVector = upVec;
}
QVector3D _Camera::getUpVector()
{
	return this->upVector;
}
