#include "_scene.h"


_Scene::_Scene()
{
	isCamera = false;
}
_Scene::~_Scene(){}
/*
*
*/
void _Scene::add(_SceneEntity s)
{
	if (isCamera == true)
	{
		r.setShader(s.getVertexShaderPath(), s.getFragmentShaderPath());
		r.setBuffers(s.getvertexData(), s.getIndexData());
		r.setModelMatrix(s.getPostion(), s.getScale(), s.getRotation());
		r.setCamViewMatrix(cam.getEyePosition(), cam.getFocalPoint(), cam.getUpVector());
		r.generateMVP();
		rendererVector.push_back(r);
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
	r.setProjectionMatrix(w, h, 45.0, 10.0, 2.0);
}
/*
*
*/
void _Scene::render()
{
	for (int i = 0; i < rendererVector.size(); i++)
	{
		rendererVector[i].draw();
	}
}
