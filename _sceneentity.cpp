#include "_sceneentity.h"
#include <QDebug>

/* Constructor: _SceneEntity()
 * Takes the values for diffrent aspects of a renderable model.
 * Author: Aditya
*/
_SceneEntity::_SceneEntity(){	//sets the rotation value at init and uses the from axis angle
    this->rotation = QVector3D(0.0,0.0,0.0);
    this->postion = QVector3D(0.0, 0.0, 0.0);
    this->scale = 1.0;
    this->orderInIndex = 0;
    this->isActive = true;
    this->isTransformationLocal = false;
    this->isPivotSet = false;
    this->isPhysicsObject = false;
    this->isHitByRay = false;
    this->isMeshEditable = false;
    this->tag = new char();
    this->isLineMode = false;
    this->isTransformationAllowed = true;

    this->modelInfo.setIsLoaded(false);
}
/*
 * Constructor: _SceneEntity(QVector3D pos, QQuaternion rot, float scale)
 * Takes the values for diffrent aspects of a renderable model.
 * Author: Aditya
 * Created:26_02_2019
*/
_SceneEntity::_SceneEntity(QVector3D pos, QVector3D rot, float scale){
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
void _SceneEntity::setId(unsigned int id){
    this->id = id;
}
unsigned int _SceneEntity::getId() const{
    return this->id;
}
/* Function: setTag(const char *tag)
 * sets/gets the tag for the scene object
 * Tag is a string name uniqe identifier for the
 * object,similar to iD.
*/
void _SceneEntity::setTag(const char *tag){
    this->tag = tag;
}
const char *_SceneEntity::getTag() const{
    return this->tag;
}
/*
 * Created: 10_06_2019
*/
void _SceneEntity::setOrderInIndex(unsigned int i){
    this->orderInIndex = i;
}
unsigned int _SceneEntity::getIndexPosInScene()const{
    return this->orderInIndex;
}

void _SceneEntity::setIsTransformationAllowed(bool isit){
    this->isTransformationAllowed = isit;
}

bool _SceneEntity::getIsTransformationAllowed(){
    return this->isTransformationAllowed;
}
/*
 * Function: setPosition(QVector3D pos) & getPostion()
 * sets/gets the position for the current object.
 * Created:26_02_2019
*/
void _SceneEntity::setPosition(QVector3D pos){
    this->postion = pos;
}
QVector3D _SceneEntity::getPostion() const{
    return this->postion;
}
/*
 * Function: setRotation(QQuaternion rot) & getRotation()
 * sets/gets the rotation for the current object.
 * Created:26_02_2019
*/
void _SceneEntity::setRotation(QVector3D rot){
    this->rotation = rot;
}
QVector3D _SceneEntity::getRotation() const{
    return this->rotation;
}
/*
 * Function: set/getPivot
 * sets gets the pivot position for the
 * scene entity.
 * Created:16_05_2019
*/
void _SceneEntity::setPivot(QVector3D pivot){
    this->pivot = pivot;
    isPivotSet = true;
}
QVector3D _SceneEntity::getPivot() const{
    return this->pivot;
}
//return if the pivot is set or not
bool _SceneEntity::getIsPivotSet(){
    return this->isPivotSet;
}
/*
 * Function: setScale(float scale) & getScale()
 * sets/gets the scale for the current object.
 * Created:26_02_2019
*/
void _SceneEntity::setScale(float scale){
    this->scale = scale;
}
float _SceneEntity::getScale() const{
    return this->scale;
}
/*
 * Function: set/getColor
 * sts gets the color variable of the
 * sceneObject
 * Created: 21_05_2019
*/
void _SceneEntity::setColor(QVector4D col){
    this->color = col;
}
QVector4D _SceneEntity::getColor() const{
    return this->color;
}
/*
 * Function: set/get translationMatrix()
 * that stores the traslation part of the modelMatrix
 * locally
 * Created: 20_05_2019
*/
void _SceneEntity::setTranslationMatrix(glm::mat4x4 tmat){
    this->TranslationMatrix = tmat;
}
glm::mat4x4 _SceneEntity::getTranslationMatrix() const{
    return this->TranslationMatrix;
}
/*
 * Function: set/get Rotationmatrix()
 * that stores the rotation part of the modelMatrix
 * locally
 * Created: 20_05_2019
*/
void _SceneEntity::setRotationmatrix(glm::mat4x4 rmat){
    this->RotationMatrix = rmat;
}
glm::mat4x4 _SceneEntity::getRotationmatrix() const{
    return this->RotationMatrix;
}
/*
 * Function: set/get ScaleingMatrix()
 * that stores the scaling part of the modelMatrix
 * locally
 * Created: 20_05_2019
*/
void _SceneEntity::setScaleingMatrix(glm::mat4x4 smat){
    this->ScaleMatirx = smat;
}
glm::mat4x4 _SceneEntity::getScaleingMatrix() const{
    return this->ScaleMatirx;
}
void _SceneEntity::setModelMatrix(glm::mat4x4 mmat){
    this->ModelMatrix = mmat;
}
glm::mat4x4 _SceneEntity::getModelMatrix() const{
    return this->ModelMatrix;
}
/*
 * Created:3_06_2019
*/
void _SceneEntity::setProjectionMatrix(glm::mat4x4 proj){
    this->ProjectionMatrix = proj;
}
glm::mat4x4 _SceneEntity::getProjectionMatrix() const{
    return this->ProjectionMatrix;
}
/*
 * Created:3_06_2019
*/
void _SceneEntity::setViewMatrix(glm::mat4x4 view){
    this->ViewMatrix = view;
}
glm::mat4x4 _SceneEntity::getViewMatrix() const{
    return this->ViewMatrix;
}
/*
 * Created:13_06_2019
*/
void _SceneEntity::setModelInfo(_AssetLoader::Model_Info minfo){
    this->modelInfo = minfo;
}
_AssetLoader::Model_Info _SceneEntity::getModelInfo() const{
    return this->modelInfo;
}
void _SceneEntity::setIsActive(bool isIt){
    this->isActive = isIt;
}
bool _SceneEntity::getIsActive(){
    return this->isActive;
}
/*
 * Created:15_05_2019
*/
void _SceneEntity::setIsMeshEditable(bool isit){
    this->isMeshEditable = isit;
}
bool _SceneEntity::getIsMeshEditable(){
    return this->isMeshEditable;
}
/*
 * Created:15_05_2019
*/
void _SceneEntity::setIsLineMode(bool isit){
    this->isLineMode = isit;
}
bool _SceneEntity::getIsLineMode(){
    return this->isLineMode;
}
/*
 *Function:isTransformationLocal()
 * sets gets the isTranformationlocal boolean object
 * that flags if this scene object need to tranform on local axis
 * or global axis
*/
void _SceneEntity::setIsTransformationLocal(bool isLoc){
    this->isTransformationLocal = isLoc;
}
bool _SceneEntity::getIsTransformationLocal(){
    return this->isTransformationLocal;
}
/*
 *Created:11_06_2017
*/
void _SceneEntity::setModelData(_AssetLoader::Model_Info minfo)
{
    if(minfo.getVertices().size() > 0 && minfo.getIndices().size() > 0){
        assetLoader.setModelInfo(minfo);
        assetLoader.calcMinMaxExtents();
        this->modelInfo = assetLoader.getModelInfo();
        this->modelInfo.setIsLoaded(true);
        this->isActive = true;
    }
    else{
        qInfo() << "model data not sufficent,please check input";
        this->isActive = false;
    }
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
    this->assetLoader.objLoader(path);
    if(assetLoader.getAssetVertices().size() > 0 && assetLoader.getAssetIndices().size() > 0){
        this->modelInfo = assetLoader.getModelInfo();
        this->modelInfo.setIsLoaded(true);
        this->isActive = true;
    }
    else{
        qInfo() << "no model data in file, please check the path to file";
        this->isActive = false;
    }
}
/*
 * Function: setShaderPath(QString vSh, QString fSh)
 * sets the path  for the shadert to be loaded ,for the current object.
 * Created:26_02_2019
*/
void _SceneEntity::setShader(QString vSh, QString fSh){
    this->vShaderPath = vSh;
    this->fShaderPath = fSh;
}
/* Function:gets/sets texture path getTexturePath().
 * returns the path of the texture that is applied to the sceneObject.
 * Date: 26_02_2019
*/
QString _SceneEntity::getTexturePath() const{
    return this->texturePath;
}
void _SceneEntity::setTexturePath(QString texPath){
    this->texturePath = texPath;
}
/*
* Function: getVertexShaderPath()
* returns the Vertex shader path set in the object via set shader Path
* Created:26_02_2019
*/
QString _SceneEntity::getVertexShaderPath() const{
    return this->vShaderPath;
}
/*
* Function: getFragmentShaderPath()
* returns the fragment shader path set in the object via set shader Path
* Created:26_02_2019
*/
QString _SceneEntity::getFragmentShaderPath() const{
    return this->fShaderPath;
}
/*
 * Function: setPhysicsObject(_Physics::PhysicsObjects penum)
 * sets the physics object type in the scenEntity. Defines what
 * kind of collision obbject is attached to the sceneEntity
 * Created: 22_05_2019
*/
void _SceneEntity::setPhysicsObject(_SceneEntity::scenePhysicsObjects penum){
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
void _SceneEntity::setIsHitByRay(bool isHitByRay){
    this->isHitByRay = isHitByRay;
}
/*
 * Created:3_06_2015
*/
bool _SceneEntity::getIsPhysicsObject() const {
    return this->isPhysicsObject;
}
_SceneEntity::scenePhysicsObjects _SceneEntity::getPhysicsObjectType(){
    return this->phyObjtype;
}
