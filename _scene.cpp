#include "_scene.h"
#include <iostream>
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
_Scene::_Scene()
{
	isCamera = false;
}
_Scene::~_Scene()
{
	renderObjects.clear();
}
/*
 * Function: getSceneObjects()
 * returns the vector array of sceneObjects.
 * this is being called by the _GlWidget class.
 * Created:26_02_2019
*/
std::vector<_Renderer*> _Scene::getSceneObjectsArray()
{
	return this->renderObjects;
}
/*
* Function: addSceneObject(_SceneEntity s)
* binds the propertes set by the scene objectes into the 
* renderer instace for rendering in the scene
* this is being called by the _GlWidget class.
* Created:26_02_2019
*/
void _Scene::addSceneObject(_SceneEntity s)
{
// Only sets the scene object if the camera has been set already and scene object is active
    if (s.getIsActive() == true)
    {
        if (isCamera == true)
        {
            r = new _Renderer();
            r->setCamViewMatrix(cam.getEyePosition(), cam.getFocalPoint(), cam.getUpVector());
            r->setSceneEntityInRenderer(s);
            renderObjects.push_back(r);
        }
        else //use default values for camera if no camera set.
        {
            r = new _Renderer();
            r->setCamViewMatrix(QVector3D(0.0, 0.0, -10.0), QVector3D(0.0, 0.0, 0.0), QVector3D(0.0, 0.0, 0.0));//set a default camera value
            r->setSceneEntityInRenderer(s);
            renderObjects.push_back(r);
        }
    }
    else
    {
        std::cout << "scene object has not been set Properly" << std::endl;
    }
}
/*
 * Function: addCamera(_Camera c)
 * function checks if the cmaera is attached and sets the local
 * camera object with the camera object passed via parameter for use in rendering
 * and setting the view matrix in the renderer
 * Created:26_02_2019
*/
void _Scene::addCamera(_Camera c)
{
	isCamera = true;
    cam = c;
}
/*
 * Function: updateCamera(_Camera c)
 * !!Needs implemntation!!
*/
void _Scene::updateCamera(_Camera c)
{

}
/*
 * Function: onResize(int w,int h)
 * This function gets called on resize and all operations will run when the windows is resized
 * this is being called by the _GlWidget class.
 * Created:26_02_2019
*/
void _Scene::onResize(int w,int h)
{
    for (unsigned int i = 0; i < renderObjects.size(); i++)
	{
		renderObjects[i]->setProjectionMatrix(w,h,45.0f,100.0f,1.0f);
	}
}
/*
 * Function: render()
 * This function is render function that will call the glDraw fuinction in 
 * the render final draw of all sceneEntity objects attached to scene.
 * this is being called by the _GlWidget class.
 * Created:26_02_2019
*/
void _Scene::render()
{
    for (unsigned int i = 0; i < renderObjects.size(); i++)
    {
        renderObjects[i]->draw();
    }
}
