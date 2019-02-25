#include "_sceneentity.h"

_SceneEntity::_SceneEntity() : QOpenGLExtraFunctions(QOpenGLContext::currentContext())
{

}

void _SceneEntity::setId(int id)
{
	this->id = id;
}
int _SceneEntity::getId()
{
	return this->id;
}
//
void _SceneEntity::setPosition(QVector3D pos)
{
	this->postion = pos;
}
QVector3D _SceneEntity::getPostion()
{
	return this->postion;
}
//
void _SceneEntity::setRotation(QQuaternion rot)
{
	this->rotation = rot;
}
QQuaternion _SceneEntity::getRotation()
{
	return this->rotation;
}
//
void _SceneEntity::setScale(float scale)
{
	this->scale = scale;
}
float _SceneEntity::getScale()
{
	return this->scale;
}
//
void _SceneEntity::setModelData(std::vector<float> vertices, std::vector<float> indices)
{

}
//
void _SceneEntity::setShader(char* qrcVetexshaderLocation, char* qrcFragmentshaderLocation)
{

}