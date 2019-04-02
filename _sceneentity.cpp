#include "_sceneentity.h"

/* Constructor: _SceneEntity()
 * Takes the values for diffrent aspects of a renderable model.
 * Author: Aditya
*/
_SceneEntity::_SceneEntity()
{	//sets the rotation value at init and uses the from axis angle 
    this->rotation = QVector3D(0.0,0.0,0.0);
	this->postion = QVector3D(0.0, 0.0, 0.0);
	this->scale = 1.0;
	this->isActive = false;
}
/*
 * Constructor: _SceneEntity(QVector3D pos, QQuaternion rot, float scale)
 * Takes the values for diffrent aspects of a renderable model.
 * Author: Aditya
 * Created:26_02_2019
*/
_SceneEntity::_SceneEntity(QVector3D pos, QVector3D rot, float scale)
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
 * ID is only a tag or name, used only to find and
 * manupulate the respective object with the relavant ID.
 * Created:26_02_2019
*/
void _SceneEntity::setId(unsigned int id)
{
	this->id = id;
}
unsigned int _SceneEntity::getId()
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
void _SceneEntity::setRotation(QVector3D rot)
{
	this->rotation = rot;
}
QVector3D _SceneEntity::getRotation()
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
bool _SceneEntity::getIsActive()
{
	return this->isActive;
}
/*
 * Function: setModelData(std::vector<float> vertices, std::vector<unsigned int> indices)
 * sets the vertex and index data in one function, for the current object.
 * Created:26_02_2019
*/
void _SceneEntity::setModelData(std::vector<float> vertices, std::vector<unsigned int> indices)
{
    this->isActive = true;
	this->vertexData = vertices;
	this->indexData = indices;
}
/*
 * Function: setModelData(Qstring path)
 * sets the vertex and index data in one function, for the current object.
 * takes data as a path and uses the assetloader class to load in the obj data from
 * a path to the obj File.
 * Created:26_02_2019
 */
void _SceneEntity::setModelData(QString path)
{
	this->isActive = true;
	_AssetLoader a;
	a.objLoader(path);
	this->vertexData = a.getAssetVertices();
	this->indexData = a.getAssetIndices();
}
/*
 * Function: setShaderPath(QString vSh, QString fSh)
 * sets the path  for the shadert to be loaded ,for the current object.
 * Created:26_02_2019
*/
void _SceneEntity::setShader(QString vSh, QString fSh)
{
	this->vShaderPath = vSh;
    this->fShaderPath = fSh;
}
/* Function:gets/sets texture path getTexturePath().
 * returns the path of the texture that is applied to the sceneObject.
 * Date: 26_02_2019
*/
QString _SceneEntity::getTexturePath()
{
    return this->texturePath;
}
void _SceneEntity::setTexturePath(QString texPath)
{
    this->texturePath = texPath;
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
