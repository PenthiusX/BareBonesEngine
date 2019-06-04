#ifndef _SCENEENTITY_H
#define _SCENEENTITY_H

#include <qquaternion.h>
#include <vector>
#include <glm/glm.hpp>
#include "_assetloader.h"
//#include "_physics.h"

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
    _SceneEntity(QVector3D pos,QVector3D rot , float scale);
    _SceneEntity();
    ~_SceneEntity();

    void setIsActive(bool isIt);
    bool getIsActive();
    //
    void setId(unsigned int id);//sets the iD either externaly or internally generated
    unsigned int getId() const;
    void setTag(const char* tag);//sets a name based identifier for the object
    const char* getTag() const;
    //
    void setPosition(QVector3D pos);//sets the position for the object in the Model matrix via the _renderer class instance.
    QVector3D getPostion() const;//get the current position of the relvant object instace
    void setRotation(QVector3D rotation);//sets the rotation for the object in the Model matrix via the _renderer class instance.
    QVector3D getRotation() const;//get the Quaternian value of the rottion of the relavant object instance.
    void setPivot(QVector3D pivot);//sets the pivot point to rotate around
    bool getIsPivotSet();
    QVector3D getPivot() const;
    void setScale(float scale);//sets the scale for the object in the Model matrix via the _renderer class instance.
    float getScale() const;
    void setColor(QVector4D col);
    QVector4D getColor() const;
    //
    void setTranslationMatrix(glm::mat4x4 tmat);
    glm::mat4x4 getTranslationMatrix()const;
    void setRotationmatrix(glm::mat4x4 rmat);
    glm::mat4x4 getRotationmatrix()const;
    void setScaleingMatrix(glm::mat4x4 smat);
    glm::mat4x4 getScaleingMatrix()const;
    //
    void setProjectionMatrix(glm::mat4x4 proj);
    glm::mat4x4 getProjectionMatrix()const;
    void setViewMatrix(glm::mat4x4 view);
    glm::mat4x4 getViewMatrix()const;
    //
    void setModelData(std::vector<float> vertices,std::vector<unsigned int> indices);//set the model data explicityl with defined vertices and indices
    void setModelData(_AssetLoader::Model_Info m);
    void setModelData(QString path);//takes the relative path via a qrc file path
    void setShader(QString vshader, QString fshader);//sets the relative qrc file path to the shader files for use in the
    void setTexturePath(QString texPath);
    QString getTexturePath() const;
    //
    QString getVertexShaderPath() const;//returns the vertexshader path
    QString getFragmentShaderPath() const;//returns the fragment shader path
    //
    std::vector<float> getVertexData() const;// get  the array of verterticess for refrence
    std::vector<unsigned int> getIndexData() const;// get the array of indices for refrence
    std::vector<int> getUvData() const;// get the array of UVs for refrence
    std::vector<float> getNormalData() const;// get the Array of normals for refrence
    //
    void setIsMeshEditable(bool isit);
    bool getIsMeshEditable();
    void setIsLineMode(bool isit);
    bool getIsLineMode();
    //type of transformations
    void setIsTransfomationLocal(bool isLoc);
    bool getIsTransfomationLocal();
    //Physics
    bool getisHitByRay();
    void setIsHitByRay(bool isHitByRay);
    bool getIsPhysicsObject()const;
    enum scenePhysicsObjects{
        Sphere = 0,
        Box = 1,
        Mesh = 2,
    };
    _SceneEntity::scenePhysicsObjects getPhysicsObjectType();
    void setPhysicsObject( _SceneEntity::scenePhysicsObjects penum);

private:
    unsigned int id;
    const char* tag;
    QVector3D postion;
    QVector3D rotation;
    QVector3D pivot;
    float scale;
    QVector4D color;
    //
    std::vector<float> vertexData;
    std::vector<unsigned int> indexData;
    std::vector<int> uvData;
    std::vector<float> normalData;
    //
    QString vShaderPath;
    QString fShaderPath;
    QString texturePath;
    //
    void setVertexData(std::vector<float> vertices);//sets the Vertex data.
    void setIndexData(std::vector<unsigned int> indices);//sets the Index data.
    void setuvData(std::vector<int> uvCoords);//sets the UV data.
    void setnormalData(std::vector<float> normalData);//sets the normal data.
    //
    bool isTransfomationLocal;
    bool isPivotSet;
    bool isActive;
    bool isMeshEditable;
    bool isHitByRay;
    bool isPhysicsObject;
    bool isLineMode;
    //
    glm::mat4x4 TranslationMatrix;
    glm::mat4x4 RotationMatrix;
    glm::mat4x4 ScaleMatirx;
    glm::mat4x4 ProjectionMatrix;
    glm::mat4x4 ViewMatrix;
    //
    _AssetLoader assetLoader;//Asset loading
    _AssetLoader::Model_Info modelInfo;
    _SceneEntity::scenePhysicsObjects phyObjtype;//Physics Type identifier


};

#endif // _SCENEENTITY_H
