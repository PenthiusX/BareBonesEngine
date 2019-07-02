#include "_sceneentity.h"
#include <QDebug>

/* Constructor: _SceneEntity()
 * Takes the values for diffrent aspects of a renderable model.
 * Author: Aditya
*/

_SceneEntity::_SceneEntity(){	//sets the rotation value at init and uses the from axis angle
    rotation = glm::vec3(0.0,0.0,0.0);
    postion = glm::vec3(0.0, 0.0, 0.0);
    color = QVector4D(0.5,0.5,0.5,1.0);
    vShaderPath = ":/shaders/dmvshader.glsl";
    fShaderPath = ":/shaders/dmfshader.glsl";
    scale = 1.0;
    orderInIndex = 0;
    isActive = true;
    isTransformationLocal = false;
    isPivotSet = false;
    isPhysicsObject = false;
    isHitByRay = false;
    isMeshEditable = false;
    isPhysicsHelper = false;
    tag = new char();
    isLineMode = false;
    isLineNoCullMode = false;
    isTransformationAllowed = true;

    modelInfo.setIsLoaded(false);
}
/*
 * Constructor: _SceneEntity(QVector3D pos, QQuaternion rot, float scale)
 * Takes the values for diffrent aspects of a renderable model.
 * Author: Aditya
 * Created:26_02_2019
*/
_SceneEntity::_SceneEntity(glm::vec3 pos, glm::vec3 rot, float scale){
    postion = pos;
    rotation = rot;
    this->scale = scale;
}
_SceneEntity::~_SceneEntity(){
}
/*
 * Function: setId(int id) & getId()
 * sets/gets the id for the current object.
 * ID is only a tag or name, used only to find and
 * manupulate the respective object with the relavant ID.
 * Created:26_02_2019
*/
void _SceneEntity::setId(uint id){
    this->id = id;
}
uint _SceneEntity::getId() const{
    return id;
}
/* Function: setTag(const char *tag)
 * sets/gets the tag for the scene object
 * Tag is a string name uniqe identifier for the
 * object,similar to iD.
*/
void _SceneEntity::setTag(QString tag){
    this->tag = tag;
}
QString _SceneEntity::getTag() const{
    return tag;
}
/*
 * Created: 10_06_2019
*/
void _SceneEntity::setOrderInIndex(uint i){
    orderInIndex = i;
}
uint _SceneEntity::getIndexPosInScene()const{
    return orderInIndex;
}

void _SceneEntity::setIsTransformationAllowed(bool isit){
    isTransformationAllowed = isit;
}

bool _SceneEntity::getIsTransformationAllowed(){
    return isTransformationAllowed;
}
/*
 * Function: setPosition(QVector3D pos) & getPostion()
 * sets/gets the position for the current object.
 * Created:26_02_2019
*/
void _SceneEntity::setPosition(glm::vec3 pos){
    postion = pos;
}
glm::vec3 _SceneEntity::getPostion() const{
    return postion;
}
/*
 * Function: setRotation(QQuaternion rot) & getRotation()
 * sets/gets the rotation for the current object.
 * Created:26_02_2019
*/
void _SceneEntity::setRotation(glm::vec3 rot){
    rotation = rot;
}
glm::vec3 _SceneEntity::getRotation() const{
    return rotation;
}
/*
 * Function: set/getPivot
 * sets gets the pivot position for the
 * scene entity.
 * Created:16_05_2019
*/
void _SceneEntity::setPivot(glm::vec3 pivot){
    this->pivot = pivot;
    isPivotSet = true;
}
glm::vec3 _SceneEntity::getPivot() const{
    return pivot;
}
//return if the pivot is set or not
bool _SceneEntity::getIsPivotSet(){
    return isPivotSet;
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
    return scale;
}
/*
 * Function: set/getColor
 * sts gets the color variable of the
 * sceneObject
 * Created: 21_05_2019
*/
void _SceneEntity::setColor(QVector4D col){
    color = col;
}
QVector4D _SceneEntity::getColor() const{
    return color;
}
/*
 * Function: set/get translationMatrix()
 * that stores the traslation part of the modelMatrix
 * locally
 * Created: 20_05_2019
*/
void _SceneEntity::setTranslationMatrix(glm::mat4x4 tmat){
    translationMatrix = tmat;
}
glm::mat4x4 _SceneEntity::getTranslationMatrix() const{
    return translationMatrix;
}
/*
 * Function: set/get Rotationmatrix()
 * that stores the rotation part of the modelMatrix
 * locally
 * Created: 20_05_2019
*/
void _SceneEntity::setRotationmatrix(glm::mat4x4 rmat){
    rotationMatrix = rmat;
}
glm::mat4x4 _SceneEntity::getRotationmatrix() const{
    return rotationMatrix;
}
/*
 * Function: set/get ScaleingMatrix()
 * that stores the scaling part of the modelMatrix
 * locally
 * Created: 20_05_2019
*/
void _SceneEntity::setScaleingMatrix(glm::mat4x4 smat){
    scaleMatirx = smat;
}
glm::mat4x4 _SceneEntity::getScaleingMatrix() const{
    return scaleMatirx;
}
void _SceneEntity::setModelMatrix(glm::mat4x4 mmat){
    modelMatrix = mmat;
}
glm::mat4x4 _SceneEntity::getModelMatrix() const{
    return modelMatrix;
}
/*
 * Created:3_06_2019
*/
void _SceneEntity::setProjectionMatrix(glm::mat4x4 proj){
    projectionMatrix = proj;
}
glm::mat4x4 _SceneEntity::getProjectionMatrix() const{
    return projectionMatrix;
}
/*
 * Created:3_06_2019
*/
void _SceneEntity::setViewMatrix(glm::mat4x4 view){
    viewMatrix = view;
}
glm::mat4x4 _SceneEntity::getViewMatrix() const{
    return viewMatrix;
}
/*
 * Created:13_06_2019
*/
void _SceneEntity::setModelInfo(_ModelInfo minfo){
    modelInfo = minfo;
}
_ModelInfo _SceneEntity::getModelInfo() const{
    return modelInfo;
}

void _SceneEntity::setIsActive(bool isIt){
    isActive = isIt;
}
bool _SceneEntity::getIsActive(){
    return isActive;
}
/*
 * Created:15_05_2019
*/
void _SceneEntity::setIsMeshEditable(bool isit){
    isMeshEditable = isit;
}
bool _SceneEntity::getIsMeshEditable(){
    return isMeshEditable;
}
/*
 * Created:15_05_2019
*/
void _SceneEntity::setIsLineMode(bool isit){
    isLineMode = isit;
}
bool _SceneEntity::getIsLineMode(){
    return isLineMode;
}

void _SceneEntity::setIsLineNoCullMode(bool isit){
    isLineNoCullMode = isit;
}
bool _SceneEntity::getIsLineNoCullMode()
{
    return isLineNoCullMode;
}
/*
 *Function:isTransformationLocal()
 * sets gets the isTranformationlocal boolean object
 * that flags if this scene object need to tranform on local axis
 * or global axis
*/
void _SceneEntity::setIsTransformationLocal(bool isLoc){
    isTransformationLocal = isLoc;
}
bool _SceneEntity::getIsTransformationLocal(){
    return isTransformationLocal;
}
/*
 *Created:11_06_2017
*/
void _SceneEntity::setModelData(_AssetLoader aloader)
{
    if(aloader.getModelInfo().getVertexArray().size() > 0 && aloader.getModelInfo().getIndexArray().size() > 0){
        assetLoader = aloader;
        modelInfo = assetLoader.getModelInfo();
        modelInfo.setIsLoaded(true);
    }
    else{
        qInfo() << "_Error::model data not sufficent,please check input";
        isActive = false;
    }
}
void _SceneEntity::setModelData(_ModelInfo minfo)
{
    if(minfo.getVertexArray().size() > 0 && minfo.getIndexArray().size() > 0){
        assetLoader.setModelInfo(minfo);
        modelInfo = assetLoader.getModelInfo();
        modelInfo.setIsLoaded(true);
    }
    else{
        qInfo() << "_Error::Model data not sufficent,please check input";
        isActive = false;
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
    assetLoader.objLoader(path);
    if(assetLoader.getModelInfo().getVertexArray().size() > 0 && assetLoader.getModelInfo().getIndexArray().size() > 0){
        modelInfo = assetLoader.getModelInfo();
        modelInfo.setIsLoaded(true);
        isActive = true;
    }
    else{
        qInfo() << "_Error::No model data in file, please check the path to file!!!!!!";
        isActive = false;
    }
}
/*
 * Function: setShaderPath(QString vSh, QString fSh)
 * sets the path  for the shadert to be loaded ,for the current object.
 * Created:26_02_2019
*/
void _SceneEntity::setShader(QString vSh, QString fSh){
    vShaderPath = vSh;
    fShaderPath = fSh;
}
/* Function:gets/sets texture path getTexturePath().
 * returns the path of the texture that is applied to the sceneObject.
 * Date: 26_02_2019
*/
QString _SceneEntity::getTexturePath() const{
    return texturePath;
}
void _SceneEntity::setTexturePath(QString texPath){
    texturePath = texPath;
}
/*
* Function: getVertexShaderPath()
* returns the Vertex shader path set in the object via set shader Path
* Created:26_02_2019
*/
QString _SceneEntity::getVertexShaderPath() const{
    return vShaderPath;
}
/*
* Function: getFragmentShaderPath()
* returns the fragment shader path set in the object via set shader Path
* Created:26_02_2019
*/
QString _SceneEntity::getFragmentShaderPath() const{
    return fShaderPath;
}
/*
 * Function: setPhysicsObject(_Physics::PhysicsObjects penum)
 * sets the physics object type in the scenEntity. Defines what
 * kind of collision obbject is attached to the sceneEntity
 * Created: 22_05_2019
*/
void _SceneEntity::setPhysicsObject(_SceneEntity::scenePhysicsObjects penum){
    isPhysicsObject = true;
    isPhysicsHelper = false;
    phyObjtype = penum;
}
void _SceneEntity::setPhysicsObject(_SceneEntity::scenePhysicsObjects penum, scenePhysicsObjects helper)
{
    if(helper = _SceneEntity::Helper){
        isPhysicsHelper = true;}
    else if(helper = _SceneEntity::NoHelper){
        isPhysicsHelper = false;}

    isPhysicsObject = true;
    phyObjtype = penum;
}
/*
 * Funtion: getisHitRay()
 * reutrn is this sceneEntity has been
 * hit by the mousPointRay or not
 * Created: 22_05_2019
*/
bool _SceneEntity::getisHitByRay(){
    return isHitByRay;
}
void _SceneEntity::setIsHitByRay(bool isHitByRay){
    this->isHitByRay = isHitByRay;
}
/*
 * Created: 22_06_2019
*/
bool _SceneEntity::getIsSelected() const{
    return isSelected;
}
void _SceneEntity::setIsSelected(bool isit){
    isSelected = isit;
}
/*
 * Created:3_06_2015
*/
bool _SceneEntity::getIsPhysicsObject() const {
    return isPhysicsObject;
}
/*
 * Created: 15_06_2019
*/
bool _SceneEntity::getIsPhysicsHelper() const
{
    return isPhysicsHelper;
}
_SceneEntity::scenePhysicsObjects _SceneEntity::getPhysicsObjectType(){
    return phyObjtype;
}
