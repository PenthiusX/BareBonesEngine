#ifndef _SCENEENTITY_H
#define _SCENEENTITY_H
#include <qquaternion.h>


class _SceneEntity
{
public:
    _SceneEntity();
    ~_SceneEntity();


private:
	int id;
	float postion;
	QQuaternion rotation;
	int scale;
};

#endif // _SCENEENTITY_H
