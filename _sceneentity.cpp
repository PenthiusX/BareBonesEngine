#include "_sceneentity.h"

/* Constructor: _SceneEntity()
 * Takes the values for diffrent aspects of a renderable model.
 * Author: Aditya
*/
_SceneEntity::_SceneEntity()
{	//sets the rotation value at init and uses the from axis angle 
	this->rotation = QQuaternion::fromAxisAndAngle(QVector3D(0.0,0.0,0.0),0.0);
	this->postion = QVector3D(0.0, 0.0, 0.0);
	this->scale = 1.0;
}
/*
 * Constructor: _SceneEntity(QVector3D pos, QQuaternion rot, float scale)
 * Takes the values for diffrent aspects of a renderable model.
 * Author: Aditya
 * Created:26_02_2019
*/
_SceneEntity::_SceneEntity(QVector3D pos, QQuaternion rot, float scale)
{
	this->postion = pos;
	this->rotation = rot;
	this->scale = scale;
}
/*
*/
_SceneEntity::~_SceneEntity(){

}
/*
 * Function: setId(int id) & getId()
 * sets/gets the id for the current object.
 * Created:26_02_2019
*/
void _SceneEntity::setId(int id)
{
	this->id = id;
}
int _SceneEntity::getId()
{
	return this->id;
}
/*
 * Function: setPosition(QVector3D pos) & getPostion()
 * sets/gets the position for the current object.
 * Created:26_02_2019
*/
void _SceneEntity::setPosition(QVector3D pos)
{
	this->postion = pos;
}
QVector3D _SceneEntity::getPostion()
{
	return this->postion;
}
/*
 * Function: setRotation(QQuaternion rot) & getRotation()
 * sets/gets the rotation for the current object.
 * Created:26_02_2019
*/
void _SceneEntity::setRotation(QQuaternion rot)
{
	this->rotation = rot;
}
QQuaternion _SceneEntity::getRotation()
{
	return this->rotation;
}
/*
 * Function: setScale(float scale) & getScale()
 * sets/gets the scale for the current object.
 * Created:26_02_2019
*/
void _SceneEntity::setScale(float scale)
{
	this->scale = scale;
}
float _SceneEntity::getScale()
{
	return this->scale;
}
/*
 * Function: setVertexData(std::vector<float> vertices) & getvertexData()
 * sets/gets the vertexData for the current object.
 * Created:26_02_2019
*/
void _SceneEntity::setVertexData(std::vector<float> vertices)
{
	this->vertexData = vertices;
}
std::vector<float> _SceneEntity::getvertexData()
{
	return this->vertexData;
}
/*
 * Function: setIndexData(std::vector<unsigned int> indices) & getIndexData()
 * sets/gets the indexData for the current object.
 * Created:26_02_2019
*/
void _SceneEntity::setIndexData(std::vector<unsigned int> indices)
{
	this->indexData = indices;
}
std::vector<unsigned int> _SceneEntity::getIndexData()
{
	return this->indexData;
}
/*
 * Function: setuvData(std::vector<unsigned int> uvCoords) & getUvData()
 * sets/gets the uvcooords for the current object.
 * Created:26_02_2019
*/
void _SceneEntity::setuvData(std::vector<int> uvCoords)
{
	this->uvData = uvCoords;
}
std::vector<int> _SceneEntity::getUvData()
{
	return this->uvData;
}
/*
 * Function: setnormalData(std::vector<float> normalData) & getNormalData()
 * sets/gets the normaldata for the current object.
 * Created:26_02_2019
*/
void _SceneEntity::setnormalData(std::vector<float> normalData)
{
	this->normalData = normalData;
}
std::vector<float> _SceneEntity::getNormalData()
{
	return this->normalData;
}
/*
 * Function: setModelData(std::vector<float> vertices, std::vector<unsigned int> indices)
 * sets the vertex and index data in one function, for the current object.
 * Created:26_02_2019
*/
void _SceneEntity::setModelData(std::vector<float> vertices, std::vector<unsigned int> indices)
{
	this->vertexData = vertices;
	this->indexData = indices;
}
/*
 * Function: setShaderPath(QString vSh, QString fSh)
 * sets the path  for the shadert to be loaded ,for the current object.
 * Created:26_02_2019
*/
void _SceneEntity::setShaderPath(QString vSh, QString fSh)
{
	this->vShaderPath = vSh;
	this->fShaderPath = fSh;
}
/*
* Function: getVertexShaderPath()
* returns the Vertex shader path set in the object via set shader Path
 * Created:26_02_2019
*/
QString _SceneEntity::getVertexShaderPath()
{
	return this->vShaderPath;
}
/*
* Function: getFragmentShaderPath()
* returns the fragment shader path set in the object via set shader Path
 * Created:26_02_2019
*/
QString _SceneEntity::getFragmentShaderPath()
{
	return this->fShaderPath;
}
