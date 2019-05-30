#include "_scene.h"
/*
 * Class: _Scene()
 * This class define the scene manager , manages what needs to be rendered and what propertes need to be
 * set inside via a sceneentity object. essentially sets values in the scene entity object into the Renderer for drawing
 * Sets up Delegation to the class _Framebuffer,_Render and _Physics to work in one scene instance in cohision.
 * Autor: Aditya
 * Created:26_02_2019
*/

/*
 * Constructor: _Scene class
*/
_Scene::_Scene()
{
    isCamera = false;
    fboObject = new _FrameBuffer();
}
_Scene::~_Scene()
{
    renderObjects.clear();
    delete r;
}
/*
 * Function: getSceneObjects()
 * returns the vector array of sceneObjects.
 * this is being called by the _GlWidget class.
 * Created:26_02_2019
*/
std::vector<_Renderer*> _Scene::getSceneObjects()
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
            r->setProjectionMatrix(this->resW,this->resH,cam.getFOV(),cam.getNearClipDistance(),cam.getFarClipDistance());
            r->initSceneEntityInRenderer(s);
            renderObjects.push_back(r);
        }
        else //use default values for camera if no camera set.
        {
            r = new _Renderer();
            r->setCamViewMatrix(QVector3D(0.0, 0.0, -10.0), QVector3D(0.0, 0.0, 0.0), QVector3D(0.0, 0.0, 0.0));//set a default camera value
            r->setProjectionMatrix(this->resW,this->resH,cam.getFOV(),cam.getNearClipDistance(),cam.getFarClipDistance());
            r->initSceneEntityInRenderer(s);
            renderObjects.push_back(r);
        }
    }
    else
    {
        qDebug() << "scene object has not been set Properly";
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
 * sets the camera updated values to every render entity matrix
*/
void _Scene::updateCamera(_Camera c)
{
    cam = c;
    if(isCamera == true)
    {
        for (unsigned int i = 0; i < renderObjects.size(); i++)
        {
            renderObjects[i]->setCamViewMatrix(c.getEyePosition(),c.getFocalPoint(),c.getUpVector());
        }
    }
}
/*
 * Function: onResize(int w,int h)
 * This function gets called on resize and all operations will run when the windows is resized
 * this is being called by the _GlWidget class.
 * Created:26_02_2019
*/
void _Scene::onResize(int w,int h)
{
    this->resW = w;
    this->resH = h;
    for (unsigned int i = 0; i < renderObjects.size(); i++)
    {
        renderObjects[i]->setProjectionMatrix(w,h,cam.getFOV(),cam.getNearClipDistance(),cam.getFarClipDistance());
    }
    fboObject->initialise();
    fboObject->setupFramebuffer(w,h);
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
    //sets the Frame for the framebufferObject , the frames are being bound underneath in the draw() function below
    fboObject->setFrameInUpdate();
    //Frame to render is below
    for (unsigned int i = 0; i < renderObjects.size(); i++)
    {
        //Physics update
        if(renderObjects[i]->getSceneEntity().getIsPhysicsObject())//if the sceneEntity has physics body attached
        {   //Passing some essentials into the updateLoop for physics
            updatePhysics(renderObjects[i]->getSceneEntity().getPhysicsObjectType(),
                          glm::vec2(this->mousePositionL.x(),//Mouse position
                                    this->mousePositionL.y()),
                          glm::vec3(cam.getEyePosition().x(),//Camera Position
                                    cam.getEyePosition().y(),
                                    cam.getEyePosition().z()),
                          glm::vec2(this->resW,this->resH),//Current Resolution
                          renderObjects[i]->getSceneEntity(),//Selected sceneEntity
                          i);//Selected Index
        }
        //Raster update
        renderObjects[i]->draw();//calls the draw function unique to each renderObject
    }
    //Frame is Loader and rendered on Quad below
    fboObject->setMousePos(this->mousePositionR); //sets the mouse pointervalues to the fbo object
    fboObject->renderFrameOnQuad(); // sets the frame on the Quad that has been hardcoded into the function
}

void _Scene::setMousePositionInScene(QVector2D mousePos,Qt::MouseButton m)
{
    if(m == Qt::RightButton)
        this->mousePositionR = mousePos;
    else if(m == Qt::LeftButton)
        this->mousePositionL = mousePos;
}

/*
 *Function: updatePhysics(glm::vec2 mousePos,glm::vec3 camPos)
 * update the physcs variables realtime and is callsed in the scene class
 * in the drawFunction
 * Created: 22_05_2019
 */
void _Scene::updatePhysics(_Physics::PhysicsObjects type, glm::vec2 mousePos,glm::vec3 camPos,glm::vec2 screenRes,_SceneEntity s,unsigned int index)
{
    updateMouseRay(mousePos,screenRes,s);
    upDateRayCollison(type,camPos,s,index);
}

void _Scene::updateMouseRay(glm::vec2 mousePos, glm::vec2 screenRes, _SceneEntity s)
{
    //calculate ray vector
    this->phys.setMousePointerRay(mousePos,s.getProjectionMatrix(),s.getViewMatrix(),screenRes);
    //debug helper  implentation
    pointerObject.x = this->phys.getrayEye().x; //sets the mousePointerObject position
    pointerObject.y = this->phys.getrayEye().y;
    //
}

void _Scene::upDateRayCollison(_Physics::PhysicsObjects type,glm::vec3 camPos,_SceneEntity s,unsigned int index)
{
    if(type == _Physics::Sphere)
    {//the radius will come from calulation of maxextent in assetLoader for current purposes its same as the scale
        float colliderSize = s.getScale();

        if(this->phys.hitSphere(glm::vec3(s.getPostion().x(),s.getPostion().y(),s.getPostion().z()),colliderSize,camPos))
        {
            //On event of collison with ray
            pointerObject.z = this->phys.raySphereIntersect(camPos,glm::vec3(s.getPostion().x(),s.getPostion().y(),s.getPostion().z()),colliderSize);
            //set values in the sceneEntity and ressetit it in the  relavant renderObject
            s.setIsHitByRay(true);
            s.setColor(QVector4D(0.6,0.0,0.0,0.8));
            renderObjects[index]->setSceneEntityInRenderer(s);
        }
        else
        {
            //On event
            s.setIsHitByRay(false);
            s.setColor(QVector4D(1.0,0.6,0.0,0.5));
            renderObjects[index]->setSceneEntityInRenderer(s);
        }
    }
    else if(type == _Physics::Box)//run operations for HitBox
    {

    }
    else if(type == _Physics::Mesh)//Run operation for Mesh collider
    {

    }
}


