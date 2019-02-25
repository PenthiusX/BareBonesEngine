#ifndef _SCENEENTITY_H
#define _SCENEENTITY_H
#include <qquaternion.h>
#include <qopenglextrafunctions.h>
#include <vector>

class _SceneEntity : protected QOpenGLExtraFunctions
{
public:
    _SceneEntity();
    ~_SceneEntity();
	//
	void setId(int id);//sets the iD either externaly or internally generated
	int getId();// call to retrieve unique id for the object instance.
	//
	void setPosition(QVector3D pos);//sets the position for the object in the Model matrix via the _renderer class instance.
	QVector3D getPostion();//get the current position of the relvant object instace
	//
	void setRotation(QQuaternion rotation);//sets the rotation for the object in the Model matrix via the _renderer class instance.
	QQuaternion getRotation();//get the Quaternian value of the rottion of the relavant object instance.
	//
	void setScale(float scale);//sets the scale for the object in the Model matrix via the _renderer class instance.
	float getScale();//get the float  value of the rotation of the relavant object instance.
	//
	void setModelData(std::vector<float> vertices,std::vector<float> indices);//Implemntation pending
	void setShader(char* qrcVetexshaderLocation, char* qrcFragmentshaderLocation);

private:
	int id;
	QVector3D postion;
	QQuaternion rotation;
	int scale;

	void setModelMatrix();
};

#endif // _SCENEENTITY_H
