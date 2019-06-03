#include "_sceneentity.h"
#include <QDebug>

/* Constructor: _SceneEntity()
 * Takes the values for diffrent aspects of a renderable model.
 * Author: Aditya
*/
_SceneEntity::_SceneEntity()
{	//sets the rotation value at init and uses the from axis angle
    this->rotation = QVector3D(0.0,0.0,0.0);
    this->postion = QVector3D(0.0, 0.0, 0.0);
    this->scale = 1.0;
    this->isActive = true;
    this->isTransfomationLocal = false;
    this->isPivotSet = false;
    this->isPhysicsObject = false;
    this->isHitByRay = false;
    this->isMeshEditable = false;
    this->tag = new char();
    isLineMode = false;
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
_SceneEntity::~_SceneEntity(){
    tag = nullptr;
    delete tag;
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
unsigned int _SceneEntity::getId() const
{
    return this->id;
}

/* Function: setTag(const char *tag)
 * sets/gets the tag for the scene object
 * Tag is a string name uniqe identifier for the
 * object,similar to iD.
*/
void _SceneEntity::setTag(const char *tag)
{
    this->tag = tag;
}
const char *_SceneEntity::getTag() const
{
    return this->tag;
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
QVector3D _SceneEntity::getPostion() const
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
QVector3D _SceneEntity::getRotation() const
{
    return this->rotation;
}

/*
 * Function: set/getPivot
 * sets gets the pivot position for the
 * scene entity.
 * Created:16_05_2019
*/
void _SceneEntity::setPivot(QVector3D pivot)
{
    this->pivot = pivot;
    isPivotSet = true;
}
QVector3D _SceneEntity::getPivot() const
{
    return this->pivot;
}
//return if the pivot is set or not
bool _SceneEntity::getIsPivotSet()
{
    return this->isPivotSet;
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
float _SceneEntity::getScale() const
{
    return this->scale;
}

/*
 * Function: set/getColor
 * sts gets the color variable of the
 * sceneObject
 * Created: 21_05_2019
*/
void _SceneEntity::setColor(QVector4D col)
{
    this->color = col;
}
QVector4D _SceneEntity::getColor() const
{
    return this->color;
}

/*
 * Function: set/get translationMatrix()
 * that stores the traslation part of the modelMatrix
 * locally
 * Created: 20_05_2019
*/
void _SceneEntity::setTranslationMatrix(glm::mat4x4 tmat)
{
    this->TranslationMatrix = tmat;
}
glm::mat4x4 _SceneEntity::getTranslationMatrix() const
{
    return this->TranslationMatrix;
}
/*
 * Function: set/get Rotationmatrix()
 * that stores the rotation part of the modelMatrix
 * locally
 * Created: 20_05_2019
*/
void _SceneEntity::setRotationmatrix(glm::mat4x4 rmat)
{
    this->RotationMatrix = rmat;
}
glm::mat4x4 _SceneEntity::getRotationmatrix() const
{
    return this->RotationMatrix;
}
/*
 * Function: set/get ScaleingMatrix()
 * that stores the scaling part of the modelMatrix
 * locally
 * Created: 20_05_2019
*/
void _SceneEntity::setScaleingMatrix(glm::mat4x4 smat)
{
    this->ScaleMatirx = smat;
}
glm::mat4x4 _SceneEntity::getScaleingMatrix() const
{
    return this->ScaleMatirx;
}
/*
 *
*/
void _SceneEntity::setProjectionMatrix(glm::mat4x4 proj)
{
    this->ProjectionMatrix = proj;
}
glm::mat4x4 _SceneEntity::getProjectionMatrix() const
{
    return this->ProjectionMatrix;
}
/*
 *
*/
void _SceneEntity::setViewMatrix(glm::mat4x4 view)
{
    this->ViewMatrix = view;
}
glm::mat4x4 _SceneEntity::getViewMatrix() const
{
    return this->ViewMatrix;
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
std::vector<float> _SceneEntity::getVertexData() const
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
std::vector<unsigned int> _SceneEntity::getIndexData() const
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
std::vector<int> _SceneEntity::getUvData()const
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
std::vector<float> _SceneEntity::getNormalData()const
{
    return this->normalData;
}
/*
 *
*/
void _SceneEntity::setIsActive(bool isIt)
{
    this->isActive = isIt;
}
bool _SceneEntity::getIsActive()
{
    return this->isActive;
}
/*
 *
*/
void _SceneEntity::setIsMeshEditable(bool isit)
{
    this->isMeshEditable = isit;
}
bool _SceneEntity::getIsMeshEditable()
{
    return this->isMeshEditable;
}
/*
 *
*/
void _SceneEntity::setIsLineMode(bool isit)
{
    this->isLineMode = isit;
}
bool _SceneEntity::getIsLineMode()
{
    return this->isLineMode;
}

/*
 *Function:isTransfomationLocal()
 * sets gets the isTranformationlocal boolean object
 * that flags if this scene object need to tranform on local axis
 * or global axis
*/
void _SceneEntity::setIsTransfomationLocal(bool isLoc)
{
    this->isTransfomationLocal = isLoc;
}
bool _SceneEntity::getIsTransfomationLocal()
{
    return this->isTransfomationLocal;
}

/*
 * Function: setModelData(std::vector<float> vertices, std::vector<unsigned int> indices)
 * sets the vertex and index data in one function, for the current object.
 * Created:26_02_2019
*/
void _SceneEntity::setModelData(std::vector<float> vertices,std::vector<unsigned int> indices)
{
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
    assetLoader.objLoader(path);
    this->vertexData = assetLoader.getAssetVertices();
    this->indexData = assetLoader.getAssetIndices();
}
/*
 *
*/
void _SceneEntity::setModelData(_AssetLoader::Model_Info m)
{
    this->modelInfo = m;
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
QString _SceneEntity::getTexturePath() const
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
QString _SceneEntity::getVertexShaderPath() const
{
    return this->vShaderPath;
}

/*
* Function: getFragmentShaderPath()
* returns the fragment shader path set in the object via set shader Path
* Created:26_02_2019
*/
QString _SceneEntity::getFragmentShaderPath() const
{
    return this->fShaderPath;
}
/*
 * Function: setPhysicsObject(_Physics::PhysicsObjects penum)
 * sets the physics object type in the scenEntity. Defines what
 * kind of collision obbject is attached to the sceneEntity
 * Created: 22_05_2019
*/
void _SceneEntity::setPhysicsObject(_Physics::PhysicsObjects penum)
{
    this->isPhysicsObject = true;
    this->phyObjtype = penum;
}
/*
 * Funtion: getisHitRay()
 * reutrn is this sceneEntity has been
 * hit by the mousPointRay or not
 * Created: 22_05_2019
*/
bool _SceneEntity::getisHitByRay(){
    return this->isHitByRay;
}

void _SceneEntity::setIsHitByRay(bool isHitByRay)
{
    this->isHitByRay = isHitByRay;
}

bool _SceneEntity::getIsPhysicsObject() const
{
    return this->isPhysicsObject;
}

_Physics::PhysicsObjects _SceneEntity::getPhysicsObjectType()
{
    return this->phyObjtype;
}
