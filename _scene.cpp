#include "_scene.h"


_Scene::_Scene()
{
	isCamera = false;
}
_Scene::~_Scene(){}
std::vector<_Renderer> _Scene::getSceneObjects()
{
	return this->sceneObjects;
}
/*
*
*/
void _Scene::addSceneObject(_SceneEntity s)
{
	if (isCamera == true)
	{
		r.setShader(s.getVertexShaderPath(), s.getFragmentShaderPath());
		r.setBuffers(s.getvertexData(), s.getIndexData());
		r.setModelMatrix(s.getPostion(), s.getScale(), s.getRotation());
		r.setProjectionMatrix(800,600, 60.0, 100, 0.0001);
		r.setCamViewMatrix(cam.getEyePosition(), cam.getFocalPoint(), cam.getUpVector());
		sceneObjects.push_back(r);
	}
}
void _Scene::addCamera(_Camera c)
{
	isCamera = true;
	cam = c;

}
void _Scene::onResize(int w,int h)
{
	//Harcoded values for resW,resH,fov,zFar,zNear
	r.setProjectionMatrix(w, h, 45.0, 50.0, 0.1);
}
/*
* need to implemt upadte on modelmatrix and camera matrix
*/
void _Scene::render()
{
	for (int i = 0; i < sceneObjects.size(); i++)
	{
		sceneObjects[i].draw();
	}
}
