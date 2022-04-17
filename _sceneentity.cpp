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
    lighttype = light::Empty;
    tag = new char();
    isLineMode = false;
    isLineNoCullMode = false;
    isTransformationAllowed = true;
    isShadowCaster = false;
    modelInfo.setIsLoaded(false);

    glEnablements.cullMode = GlEnablements::BackFace;
    glEnablements.fillMode = GlEnablements::FrontAndBackFill;
    glEnablements.frameBufferMode = GlEnablements::BlendAndDepth;
}
/*
 * Constructor: _SceneEntity(QVector3D pos, QQuaternion rot, float scale)
 * Takes the values for diffrent aspects of a renderable model.
 * Author: Aditya

*/
_SceneEntity::_SceneEntity(glm::vec3 pos, glm::vec3 rot, float scale){
    postion = pos;
    rotation = rot;
    this->scale = scale;
}
_SceneEntity::~_SceneEntity(){}
/*
 * setId(int id) & getId()
 * sets/gets the id for the current object.
 * ID is only a tag or name, used only to find and
 * manupulate the respective object with the relavant ID.

*/
void _SceneEntity::setId(uint id){
    this->id = id;
}
uint _SceneEntity::getId() const{
    return id;
}
/* setTag(const char *tag)
 * sets/gets the tag for the scene object
 * Tag is a string name uniqe identifier for the
 * object,similar to iD.
*/
void _SceneEntity::setTag(std::string tag){
    this->tag = tag;
}
std::string _SceneEntity::getTag() const{
    return tag;
}
/*
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
 * setPosition(QVector3D pos) & getPostion()
 * sets/gets the position for the current object.

*/
void _SceneEntity::setPosition(glm::vec3 pos){
    postion = pos;
}
glm::vec3 _SceneEntity::getPostion() const{
    return postion;
}
/*
 * setRotation(QQuaternion rot) & getRotation()
 * sets/gets the rotation for the current object.

*/
void _SceneEntity::setRotation(glm::vec3 rot){
    rotation = rot;
}
glm::vec3 _SceneEntity::getRotation() const{
    return rotation;
}
/*
 * set/getPivot
 * sets gets the pivot position for the
 * scene entity.
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
 * setScale(float scale) & getScale()
 * sets/gets the scale for the current object.

*/
void _SceneEntity::setScale(float scale){
    this->scale = scale;
}
float _SceneEntity::getScale() const{
    return scale;
}
/*
 * set/getColor
 * sts gets the color variable of the
 * sceneObject
*/
void _SceneEntity::setColor(QVector4D col){
    color = col;
}
QVector4D _SceneEntity::getColor() const{
    return color;
}
/*
 * set/get translationMatrix()
 * that stores the traslation part of the modelMatrix
 * locally
*/
void _SceneEntity::setTranslationMatrix(glm::mat4x4 tmat){
    translationMatrix = tmat;
}
glm::mat4x4 _SceneEntity::getTranslationMatrix() const{
    return translationMatrix;
}
/*
 * set/get Rotationmatrix()
 * that stores the rotation part of the modelMatrix
 * locally
*/
void _SceneEntity::setRotationmatrix(glm::mat4x4 rmat){
    rotationMatrix = rmat;
}
glm::mat4x4 _SceneEntity::getRotationmatrix() const{
    return rotationMatrix;
}
/*
 * set/get ScaleingMatrix()
 * that stores the scaling part of the modelMatrix
 * locally
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
*/
void _SceneEntity::setProjectionMatrix(glm::mat4x4 proj){
    projectionMatrix = proj;
}
glm::mat4x4 _SceneEntity::getProjectionMatrix() const{
    return projectionMatrix;
}
/*
*/
void _SceneEntity::setViewMatrix(glm::mat4x4 view){
    viewMatrix = view;
}
glm::mat4x4 _SceneEntity::getViewMatrix() const{
    return viewMatrix;
}
/*
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
*/
void _SceneEntity::setIsMeshEditable(bool isit){
    isMeshEditable = isit;
}
bool _SceneEntity::getIsMeshEditable(){
    return isMeshEditable;
}

void _SceneEntity::setIsShadowCaster(bool isit){
    this->isShadowCaster = isit;
}

bool _SceneEntity::getIsShadowCaster(){
    return isShadowCaster;
}

void _SceneEntity::setGLModes(_SceneEntity::GlEnablements g)
{
    this->glEnablements = g;
}

_SceneEntity::GlEnablements _SceneEntity::getGLModes()
{
    return this->glEnablements;
}
/*
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
 * Sets the modelData in the SceneEntity itself that then gets
 * transfered to the entityRenderes in the _scene
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
    if(minfo.getVertexArray().size() > 0 /*&& minfo.getIndexArray().size() > 0*/){
        modelInfo = minfo;
        modelInfo.setIsLoaded(true);
    }
    else{
        qInfo() << "_Error::Model data not sufficent,please check input";
        isActive = false;
    }
}
/*
 * setModelData(Qstring path)
 * sets the vertex and index data in one function, for the current object.
 * takes data as a path and uses the assetloader class to load in the obj data from
 * a path to the obj File.
*/
void _SceneEntity::setModelData(QString path)
{
    //!!Need to create an exception to handel External and qrc paths
    if(path.at(0) == ":" || path.at(0) == "q" ){
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
    else{
//        assetLoader.extrenalObjLoader(path.toStdString());//use this for obj files only if assimp not presesnt
         assetLoader.assimpLoader(path.toStdString());//Assimp for mutiformat support
        if(assetLoader.getModelInfo().getVertexInfoArray().size() > 0 && assetLoader.getModelInfo().getIndexArray().size() > 0){
            modelInfo = assetLoader.getModelInfo();
            isActive = true;

             //test for only one file type , Debug test
            if(path == "D:/WorkSpace/BareBonesEngine/models/Animated/Free_Warriors_Turbosquid/models/FBX(animated)/blue/egyptian_B.fbx"){
                _Animation danceAnimation("D:/WorkSpace/BareBonesEngine/models/Animated/Free_Warriors_Turbosquid/models/FBX(animated)/blue/egyptian_B.fbx",&modelInfo);
                Animator animator(danceAnimation);
               this->animator = animator;//pass this to renderer see:renderer updateloop
            }
        }
    }
}
/*
 * setShaderPath(QString vSh, QString fSh)
 * sets the path  for the shadert to be loaded ,for the current object.
*/
void _SceneEntity::setShader(QString vSh, QString fSh) {
    vShaderPath = vSh;
    fShaderPath = fSh;
}

void _SceneEntity::setShader(QString vshader, QString fshader, QString geometry)
{
    vShaderPath = vshader;
    fShaderPath = fshader;
    gShaderPath = geometry;
}
/* Function:gets/sets texture path getTexturePath().
 * returns the path of the texture that is applied to the sceneObject.
 * Date: 26_02_2019
*/
QString _SceneEntity::getTexturePath() const{
    return texturePath;
}
/*
* getVertexShaderPath()
* returns the Vertex shader path set in the object via set shader Path
*/
QString _SceneEntity::getVertexShaderPath() const{
    return vShaderPath;
}
/*
* getFragmentShaderPath()
* returns the fragment shader path set in the object via set shader Path
*/
QString _SceneEntity::getFragmentShaderPath() const{
    return fShaderPath;
}
QString _SceneEntity::getGeometryShaderPath() const
{
    return gShaderPath;
}
/*
 * setPhysicsObject(_Physics::PhysicsObjects penum)
 * sets the physics object type in the scenEntity. Defines what
 * kind of collision obbject is attached to the sceneEntity
*/
void _SceneEntity::setPhysicsObject(_SceneEntity::PhysicsBody penum){
    isPhysicsObject = true;
    isPhysicsHelper = false;
    phyObjtype = penum;
}
void _SceneEntity::setPhysicsObject(_SceneEntity::PhysicsBody penum, PhysicsBody helper){
    if((helper = _SceneEntity::Helper)){
        isPhysicsHelper = true;}
    else if((helper = _SceneEntity::NoHelper)){
        isPhysicsHelper = false;}

    isPhysicsObject = true;
    phyObjtype = penum;
}
/*
*/
void _SceneEntity::setMaterial(_Material m){
    this->material = m;
}
_Material _SceneEntity::getMaterial(){
    return this->material;
}
void _SceneEntity::setAsLight(light l){
    this->lighttype = l;
}
_SceneEntity::light _SceneEntity::getLightSource(){
    return lighttype;
}
/*
 * Funtion: getisHitRay()
 * reutrn is this sceneEntity has been
 * hit by the mousPointRay or not
*/
bool _SceneEntity::getisHitByRay(){
    return isHitByRay;
}
void _SceneEntity::setIsHitByRay(bool isHitByRay){
    this->isHitByRay = isHitByRay;
}
/*
 * getIsHitByTri()
 * return on even when the object
 * collided with anotehr MeshOBject
 * in a Tri on Tri collision scenario
*/
bool _SceneEntity::getIsHitByTri(){
    return isHitByTri;
}
void _SceneEntity::setIsHitByTri(bool isHitByTri){
    this->isHitByTri = isHitByTri;
}
/*
*/
bool _SceneEntity::getIsSelected() const {
    return isSelected;
}
void _SceneEntity::setIsSelected(bool isit) {
    isSelected = isit;
}
/*
*/
bool _SceneEntity::getIsPhysicsObject() const {
    return isPhysicsObject;
}
/*
*/
bool _SceneEntity::getIsPhysicsHelper() const {
    return isPhysicsHelper;
}
_SceneEntity::PhysicsBody _SceneEntity::getPhysicsObjectType(){
    return phyObjtype;
}
