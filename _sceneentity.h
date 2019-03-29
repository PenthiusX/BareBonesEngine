#ifndef _SCENEENTITY_H
#define _SCENEENTITY_H

#include <qquaternion.h>
#include <vector>
#include <_assetloader.h>


class _SceneEntity
{
public:
    _SceneEntity(QVector3D pos,QQuaternion rot , float scale);
    _SceneEntity();
    ~_SceneEntity();
    void setId(unsigned int id);//sets the iD either externaly or internally generated
    unsigned int getId();// call to retrieve unique id for the object instance.
	//
	void setPosition(QVector3D pos);//sets the position for the object in the Model matrix via the _renderer class instance.
	QVector3D getPostion();//get the current position of the relvant object instace
	void setRotation(QQuaternion rotation);//sets the rotation for the object in the Model matrix via the _renderer class instance.
	QQuaternion getRotation();//get the Quaternian value of the rottion of the relavant object instance.
	void setScale(float scale);//sets the scale for the object in the Model matrix via the _renderer class instance.
	float getScale();//get the float  value of the rotation of the relavant object instance.
	//
	void setModelData(std::vector<float> vertices,std::vector<unsigned int> indices);//set the model data explicityl with defined vertices and indices
    void setModelData(QString path);//takes the relative path via a qrc file path
    void setShader(QString vshader, QString fshader);//sets the relative qrc file path to the shader files for use in the
    void setTexturePath(QString texPath);
    QString getTexturePath();

	QString getVertexShaderPath();//returns the vertexshader path
	QString getFragmentShaderPath();//returns the fragment shader path

	std::vector<float> getvertexData();//can get  the array of verterticess for refrence
	std::vector<unsigned int> getIndexData();//can get the array of indices for refrence
	std::vector<int> getUvData();// can get the array of UVs for refrence
	std::vector<float> getNormalData();// can get the Array of normals for refrence

	bool getIsActive();
	

private:
    unsigned int id;
	QVector3D postion;
	QQuaternion rotation;
	float scale;
	//
	std::vector<float>vertexData;
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
};

#endif // _SCENEENTITY_H
