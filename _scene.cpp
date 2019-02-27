#include "_scene.h"


_Scene::_Scene()
{

}
_Scene::~_Scene(){}
/*
*
*/
void _Scene::add(_SceneEntity s)
{
	renderer.setShader(s.getVertexShaderPath(),s.getFragmentShaderPath());
	renderer.setBuffers(s.getvertexData(),s.getIndexData());
	renderer.setModelMatrix(s.getPostion(), s.getScale(), s.getRotation());
	rendererVector.push_back(renderer);
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
