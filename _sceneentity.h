#ifndef _SCENEENTITY_H
#define _SCENEENTITY_H

#include <qquaternion.h>
#include <vector>
#include <glm/glm.hpp>
#include "_assetloader.h"

/* Class: _SceneEntity()
 * The scene entity class holds the values for all the paramets a scene object needs
 * to derive from while rendering.
 * The objects of this class set and get properties at initialisation as well as runtime.
 * accesing the scene entity object and its member variables from the sceneObjets will give
 * you the current runtime updated values to all the parameters it holds, setting the same values
 * on runtime will also translate to changes of the respective sceneoObject on screen.
 * Author: Aditya
*/
class _SceneEntity
{
public:
    _SceneEntity(glm::vec3 pos, glm::vec3 rot , float scale);
    _SceneEntity();
    ~_SceneEntity();
    //
    void setIsActive(bool isIt);
    bool getIsActive();
    //
    void setId(uint id);//set a unique iD either externaly or internally generated
    uint getId() const;
    void setTag(QString tag);//set unique string based identifier for the object
    QString getTag() const;
    void setOrderInIndex(uint i);
    uint getIndexPosInScene() const;
    //
    void setIsTransformationAllowed(bool isit);
    bool getIsTransformationAllowed();
    void setPosition(glm::vec3 pos);//sets the position for the object in the Model matrix via the _renderer class instance.
    glm::vec3 getPostion() const;//get the current position of the relvant object instace
    void setRotation(glm::vec3 rotation);//sets the rotation for the object in the Model matrix via the _renderer class instance.
    glm::vec3 getRotation() const;//get the Quaternian value of the rottion of the relavant object instance.
    void setPivot(glm::vec3 pivot);//sets the pivot point to rotate around
    bool getIsPivotSet();
    glm::vec3 getPivot() const;
    void setScale(float scale);//sets the scale for the object in the Model matrix via the _renderer class instance.
    float getScale() const;//get the flot value for scale for unifor scaling, can be converted to a vec3 for more explicit scaling on axis
    void setColor(QVector4D col);//sets the collor value that passes into shder of the objects program instance.
    QVector4D getColor() const;
    //
    void setTranslationMatrix(glm::mat4x4 tmat);
    glm::mat4x4 getTranslationMatrix()const;
    void setRotationmatrix(glm::mat4x4 rmat);
    glm::mat4x4 getRotationmatrix()const;
    void setScaleingMatrix(glm::mat4x4 smat);
    glm::mat4x4 getScaleingMatrix()const;
    void setModelMatrix(glm::mat4x4 mmat);
    glm::mat4x4 getModelMatrix()const;
    //
    void setProjectionMatrix(glm::mat4x4 proj);
    glm::mat4x4 getProjectionMatrix()const;
    void setViewMatrix(glm::mat4x4 view);
    glm::mat4x4 getViewMatrix()const;
    //
    void setModelInfo(_ModelInfo minfo);
    _ModelInfo getModelInfo()const;
    //
    void setModelData(_AssetLoader aloader);//set the modelInfo object in sceneEntity
    void setModelData(_ModelInfo minfo);//set the modelInfo object in sceneEntity
    void setModelData(QString path);//takes the relative path via a qrc file path
    void setShader(QString vshader, QString fshader);//sets the relative qrc file path to the shader files for use in the
    void setTexturePath(QString texPath);
    QString getTexturePath() const;
    //
    QString getVertexShaderPath() const;//returns the vertexshader path
    QString getFragmentShaderPath() const;//returns the fragment shader path

    //flag for enabling mesh editing
    void setIsMeshEditable(bool isit);
    bool getIsMeshEditable();
    //flag for setting if glLineMode
    void setIsLineMode(bool isit);
    bool getIsLineMode();
    void setIsLineNoCullMode(bool isit);
    bool getIsLineNoCullMode();
    //flag for type of transformations
    void setIsTransformationLocal(bool isLoc);
    bool getIsTransformationLocal();
    //Physics
    enum scenePhysicsObjects{
        Sphere = 0,
        Box = 1,
        Mesh = 2,
        Helper = 3,
        NoHelper = 4
    };
    bool getisHitByRay();
    void setIsHitByRay(bool isHitByRay);
    bool getIsHitByTri();
    void setIsHitByTri(bool isHitByTri);
    void setIsSelected(bool isit);
    bool getIsSelected() const;
    bool getIsPhysicsObject()const;
    bool getIsPhysicsHelper()const;
    _SceneEntity::scenePhysicsObjects getPhysicsObjectType();
    void setPhysicsObject( _SceneEntity::scenePhysicsObjects penum);
    void setPhysicsObject( _SceneEntity::scenePhysicsObjects penum,_SceneEntity::scenePhysicsObjects helper);

private:
    uint id;
    QString tag;
    glm::vec3 postion;
    glm::vec3 rotation;
    glm::vec3 pivot;
    float scale;
    QVector4D color;
    uint orderInIndex;
    //
    std::vector<float> vertexData;
    std::vector<uint> indexData;
    std::vector<int> uvData;
    std::vector<float> normalData;
    //
    QString vShaderPath;
    QString fShaderPath;
    QString texturePath;
    //
    void setVertexData(std::vector<float> vertices);//sets the Vertex data.
    void setIndexData(std::vector<uint> indices);//sets the Index data.
    void setuvData(std::vector<int> uvCoords);//sets the UV data.
    void setnormalData(std::vector<float> normalData);//sets the normal data.
    //
    bool isTransformationLocal;
    bool isPivotSet;
    bool isActive;
    bool isMeshEditable;
    bool isHitByRay;
    bool isHitByTri;
    bool isSelected;
    bool isPhysicsObject;
    bool isPhysicsHelper;
    bool isLineMode;
    bool isLineNoCullMode;
    bool isTransformationAllowed;
    //
    glm::mat4x4 translationMatrix;
    glm::mat4x4 rotationMatrix;
    glm::mat4x4 scaleMatirx;
    glm::mat4x4 modelMatrix;
    glm::mat4x4 projectionMatrix;
    glm::mat4x4 viewMatrix;
    //
    _AssetLoader assetLoader;//Asset loading
    _ModelInfo modelInfo;
    //
    _SceneEntity::scenePhysicsObjects phyObjtype;//Physics Type identifier
};

#endif // _SCENEENTITY_H
