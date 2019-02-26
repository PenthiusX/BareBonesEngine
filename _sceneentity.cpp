#include "_sceneentity.h"

_SceneEntity::_SceneEntity()
{

}
_SceneEntity::~_SceneEntity()
{
}
//
void _SceneEntity::setId(int id)
{
	this->id = id;
}
//
int _SceneEntity::getId()
{
	return this->id;
}
//
void _SceneEntity::setPosition(QVector3D pos)
{
	this->postion = pos;
}
//
QVector3D _SceneEntity::getPostion()
{
	return this->postion;
}
//
void _SceneEntity::setRotation(QQuaternion rot)
{
	this->rotation = rot;
}
//
QQuaternion _SceneEntity::getRotation()
{
	return this->rotation;
}
//
void _SceneEntity::setScale(float scale)
{
	this->scale = scale;
}
//
float _SceneEntity::getScale()
{
	return this->scale;
}
//
void _SceneEntity::setVertexData(std::vector<float> vertices)
{
	this->vertexData = vertices;
}
std::vector<float> _SceneEntity::getvertexData()
{
	return this->vertexData;
}
//
void _SceneEntity::setIndexData(std::vector<unsigned int> indices)
{
	this->indexData = indices;
}
std::vector<unsigned int> _SceneEntity::getIndexData()
{
	return this->indexData;
}
//
void _SceneEntity::setuvData(std::vector<int> uvCoords)
{
	this->uvData = uvCoords;
}
std::vector<int> _SceneEntity::getUvData()
{
	return this->uvData;
}
//
void _SceneEntity::setnormalData(std::vector<float> normalData)
{
	this->normalData = normalData;
}
std::vector<float> _SceneEntity::getNormalData()
{
	return this->normalData;
}
//
void _SceneEntity::setModelData(std::vector<float> vertices, std::vector<unsigned int> indices)
{
	this->vertexData = vertices;
	this->indexData = indices;
}
//
void _SceneEntity::setShader(char* qrcVetexshaderLocation, char* qrcFragmentshaderLocation)
{
	this->vShaderPath = qrcVetexshaderLocation;
	this->fShaderPath = qrcFragmentshaderLocation;
}
