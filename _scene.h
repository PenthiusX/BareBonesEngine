#ifndef _SCENE_H
#define _SCENE_H
#include "_sceneentity.h"
#include "_renderer.h"
#include "_camera.h"
#include <vector>
#include <qopenglextrafunctions.h>

class _Scene : protected QOpenGLExtraFunctions
{
public:
    _Scene();
    ~_Scene();
		
	std::vector<_Renderer> getSceneObjects();

        void addSceneObject(_SceneEntity s);
        void addCamera(_Camera c);
		void onResize(int w,int h);
        void render();
private:
	std::vector<_SceneEntity> sceneEntityVector;
	std::vector<_Renderer> sceneObjects;
	_Renderer r;
	_Camera cam;
	bool isCamera;
};

#endif // _SCENE_H
