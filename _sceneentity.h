#ifndef _SCENEENTITY_H
#define _SCENEENTITY_H

#include <qquaternion.h>
#include <vector>

class _SceneEntity
{
public:
    _SceneEntity(QVector3D pos,QQuaternion rot , float scale);
    _SceneEntity();
    ~_SceneEntity();
	void setId(int id);//sets the iD either externaly or internally generated
	int getId();// call to retrieve unique id for the object instance.
	//
	void setPosition(QVector3D pos);//sets the position for the object in the Model matrix via the _renderer class instance.
	QVector3D getPostion();//get the current position of the relvant object instace
	void setRotation(QQuaternion rotation);//sets the rotation for the object in the Model matrix via the _renderer class instance.
	QQuaternion getRotation();//get the Quaternian value of the rottion of the relavant object instance.
	void setScale(float scale);//sets the scale for the object in the Model matrix via the _renderer class instance.
	float getScale();//get the float  value of the rotation of the relavant object instance.
	//
	void setVertexData(std::vector<float> vertices);
	std::vector<float> getvertexData();
	void setIndexData(std::vector<unsigned int> indices);
	std::vector<unsigned int> getIndexData();
	void setuvData(std::vector<int> uvCoords);
	std::vector<int> getUvData();
	void setnormalData(std::vector<float> normalData);
	std::vector<float> getNormalData();
	//
	void setModelData(std::vector<float> vertices,std::vector<unsigned int> indices);//Implemntation pending
	void setShaderPath(QString vshader, QString fshader);//Implementation pending
	QString getVertexShaderPath();
	QString getFragmentShaderPath();

private:
	int id;
	QVector3D postion;//
	QQuaternion rotation;//
	float scale;//
	std::vector<float>vertexData;//
	std::vector<unsigned int> indexData;//
	std::vector<int> uvData;//
	std::vector<float> normalData;//

	QString vShaderPath;
	QString fShaderPath;
	void calculateNormals();//Runs after Vertices have been fed , Implementation pending.
};

#endif // _SCENEENTITY_H
