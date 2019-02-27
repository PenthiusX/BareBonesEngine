#ifndef _SCENE_H
#define _SCENE_H
#include <qopenglextrafunctions.h>
#include "_sceneentity.h"
#include "_renderer.h"
#include <vector>

class _Scene : protected QOpenGLExtraFunctions
{
public:
    _Scene();
    ~_Scene();
        void add(_SceneEntity s);
        void add(/*camera c*/);//need to implement camera class.Pending
        void render();
private:
	std::vector<_SceneEntity> sceneEntityVector;
	std::vector<_Renderer> rendererVector;
	_Renderer renderer;
};

#endif // _SCENE_H
