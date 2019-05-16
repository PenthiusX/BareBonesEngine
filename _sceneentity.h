#ifndef _SCENEENTITY_H
#define _SCENEENTITY_H

#include <qquaternion.h>
#include <vector>
#include <_assetloader.h>


class _SceneEntity
{
public:
    _SceneEntity(QVector3D pos,QVector3D rot , float scale);
    _SceneEntity();
    ~_SceneEntity();
    void setId(unsigned int id);//sets the iD either externaly or internally generated
    unsigned int getId() const;// call to retrieve unique id for the object instance.
    void setTag(const char* tag);
    const char* getTag() const;
    //
    void setPosition(QVector3D pos);//sets the position for the object in the Model matrix via the _renderer class instance.
    QVector3D getPostion() const;//get the current position of the relvant object instace
    void setRotation(QVector3D rotation);//sets the rotation for the object in the Model matrix via the _renderer class instance.
    QVector3D getRotation() const;//get the Quaternian value of the rottion of the relavant object instance.
    void setPivot();//sets the pivot point to rotate around
    QVector3D getPivot();//gets the pivot point to rotate around
    void setScale(float scale);//sets the scale for the object in the Model matrix via the _renderer class instance.
    float getScale() const;//get the float  value of the rotation of the relavant object instance.
    //
    void setModelData(std::vector<float> vertices,std::vector<unsigned int> indices);//set the model data explicityl with defined vertices and indices
    void _SceneEntity::setModelData(QString path);//takes the relative path via a qrc file path
    void setShader(QString vshader, QString fshader);//sets the relative qrc file path to the shader files for use in the
    void setTexturePath(QString texPath);
    QString getTexturePath() const;
    //
    QString getVertexShaderPath() const;//returns the vertexshader path
    QString getFragmentShaderPath() const;//returns the fragment shader path
    //
    std::vector<float> getvertexData() const;//can get  the array of verterticess for refrence
    std::vector<unsigned int> getIndexData() const;//can get the array of indices for refrence
    std::vector<int> getUvData() const;// can get the array of UVs for refrence
    std::vector<float> getNormalData() const;// can get the Array of normals for refrence
    //
    bool getIsActive();
    //
    void setIsTransfomationLocal(bool isLoc);
    bool getIsTransfomationLocal();

private:
    unsigned int id;
    const char* tag;
    QVector3D postion;
    QVector3D rotation;
    QVector3D pivot;
    float scale;
    //
    std::vector<float> vertexData;
    std::vector<unsigned int> indexData;
    std::vector<int> uvData;
    std::vector<float> normalData;
    bool isActive;
    //
    QString vShaderPath;
    QString fShaderPath;
    //
    QString texturePath;
    //
    void setVertexData(std::vector<float> vertices);//sets the Vertex data.
    void setIndexData(std::vector<unsigned int> indices);//sets the Index data.
    void setuvData(std::vector<int> uvCoords);//sets the UV data.
    void setnormalData(std::vector<float> normalData);//sets the normal data.
    //
    bool isTransfomationLocal;
};

#endif // _SCENEENTITY_H
