#ifndef _SCENE_H
#define _SCENE_H
#include "_sceneentity.h"
#include "_renderer.h"
#include "_camera.h"
#include <vector>

/*
 * Class: _Scene
 * This class define the scene manager , manages what needs to be rendered and what propertes need to be
 * set inside via a sceneentity object. essentially sets values in the scen entity object into the Renderer for drawing
 * Autor: Aditya
 * Created:26_02_2019
*/
class _Scene
{
public:
    _Scene();
    ~_Scene();

		//
	std::vector<_Renderer*> getSceneObjectsArray();
        void addSceneObject(_SceneEntity s);
        void addCamera(_Camera c);
		void onResize(int w,int h);
        void render();
private:
	std::vector<_SceneEntity> sceneEntityVector;
	std::vector<_Renderer*> renderObjects;
	_Renderer* r;
	_Camera cam;
	bool isCamera;
};

#endif // _SCENE_H
