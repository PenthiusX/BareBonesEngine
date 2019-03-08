#include "_scene.h"
/*
 * Class: _Scene()
 * This class define the scene manager , manages what needs to be rendered and what propertes need to be
 * set inside via a sceneentity object. essentially sets values in the scen entity object into the Renderer for drawing
 * Autor: Aditya
 * Created:26_02_2019
*/

/*
 * Constructor: _Scene class
*/
_Scene::_Scene()/* : QOpenGLExtraFunctions(QOpenGLContext::currentContext())*/
{
	isCamera = false;
}
_Scene::~_Scene(){}
/*
 *
*/
std::vector<_Renderer*> _Scene::getSceneObjects()
{
	return this->sceneObjects;
}
/*
*
*/
void _Scene::addSceneObject(_SceneEntity s)
{
// Only sets the scene object if the camera has been set already
	if (isCamera == true)
	{
		r = new _Renderer();
		r->setShader(s.getVertexShaderPath(), s.getFragmentShaderPath());
		r->setBuffers(s.getvertexData(), s.getIndexData());
		r->setModelMatrix(s.getPostion(), s.getScale(), s.getRotation());
		r->setProjectionMatrix(800,600, 45.0f, 100.0f, 1.0f);
		r->setCamViewMatrix(cam.getEyePosition(), cam.getFocalPoint(), cam.getUpVector());
		sceneObjects.push_back(r);
	}
	else //used default values
	{
		r->setShader(s.getVertexShaderPath(), s.getFragmentShaderPath());
		r->setBuffers(s.getvertexData(), s.getIndexData());
		r->setModelMatrix(s.getPostion(), s.getScale(), s.getRotation());
		r->setProjectionMatrix(800, 600, 45.0f, 100.0f, 1.0f);
		r->setCamViewMatrix(QVector3D(0.0,0.0,-5.0), QVector3D(0.0,0.0,0.0), QVector3D(0.0,1.0,0.0));
		sceneObjects.push_back(r);
	}
}
/*
 *
*/
void _Scene::addCamera(_Camera c)
{
	isCamera = true;
	cam = c;
}
/*
 *
*/
void _Scene::onResize(int w,int h)
{
	/*sceneObject[0].setMatrices(w,h);
	sceneObject[1].setMatrices(w, h);*/
}
/*
* need to implemt upadte on modelmatrix and camera matrix
*/

void _Scene::render()
{
	
	for (int i = 0; i < sceneObjects.size(); i++)
	{
		sceneObjects[i]->draw();
	}

}
