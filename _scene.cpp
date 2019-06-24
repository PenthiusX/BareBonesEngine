#include "_scene.h"
#include "_tools.h"

/*
 * Class: _Scene()
 * This class define the scene manager , manages what needs to be rendered and what propertes need to be
 * set inside via a sceneentity object. essentially sets values in the scene entity object into the Renderer for drawing
 * Sets up Delegation to the class _Framebuffer,_Render and _Physics to work in one scene instance in cohision.
 * Autor: Aditya Mattoo
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
  ▪   ▐ ▄ ▪  ▄▄▄▄▄▪   ▄▄▄· ▄▄▌  ▪  ·▄▄▄▄•▄▄▄ .
  ██ •█▌▐███ •██  ██ ▐█ ▀█ ██•  ██ ▪▀·.█▌▀▄.▀·
  ▐█·▐█▐▐▌▐█· ▐█.▪▐█·▄█▀▀█ ██▪  ▐█·▄█▀▀▀•▐▀▀▪▄
  ▐█▌██▐█▌▐█▌ ▐█▌·▐█▌▐█ ▪▐▌▐█▌▐▌▐█▌█▌▪▄█▀▐█▄▄▌
  ▀▀▀▀▀ █▪▀▀▀ ▀▀▀ ▀▀▀ ▀  ▀ .▀▀▀ ▀▀▀·▀▀▀ • ▀▀▀
*/
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
    if (s.getIsActive() == true){
        if (isCamera){
            r = new _Renderer();
            r->setCamViewMatrix(cam.getEyePosition(), cam.getFocalPoint(), cam.getUpVector());
            r->setProjectionMatrix(resW,resH,cam.getFOV(),cam.getNearClipDistance(),cam.getFarClipDistance());
            r->initSceneEntityInRenderer(s);
            _SceneEntity s =  r->getSceneEntity();
            s.setOrderInIndex(renderObjects.size());//sets the order value of sceneEntiy in scne.
            r->setSceneEntityInRenderer(s);
            renderObjects.push_back(r);
            //
            if(s.getIsPhysicsObject()){
                _Physics phys;
                phys.setSceneEntity(s);
                physVector.push_back(phys);
            }
        }
        else if(!isCamera){//use default values for camera if no camera set.
            r = new _Renderer();
            r->setCamViewMatrix(QVector3D(0.0, 0.0, -10.0), glm::vec3(0.0, 0.0, 0.0), QVector3D(0.0, 0.0, 0.0));//set a default camera value
            r->setProjectionMatrix(resW,resH,cam.getFOV(),cam.getNearClipDistance(),cam.getFarClipDistance());
            r->initSceneEntityInRenderer(s);
            _SceneEntity s =  r->getSceneEntity();
            s.setOrderInIndex(renderObjects.size());//sets the order value of sceneEntiy in scne.
            r->setSceneEntityInRenderer(s);
            renderObjects.push_back(r);
            //
            if(s.getIsPhysicsObject()){
                _Physics phys;
                phys.setSceneEntity(s);
                physVector.push_back(phys);
            }
        }
    }
    else
        qDebug() << "scene object has not been set Properly";
}
/*
   ▄▄ • ▄▄▄ .▄▄▄▄▄   .▄▄ · ▄▄▄ .▄▄▄▄▄
  ▐█ ▀ ▪▀▄.▀·•██     ▐█ ▀. ▀▄.▀·•██
  ▄█ ▀█▄▐▀▀▪▄ ▐█.▪   ▄▀▀▀█▄▐▀▀▪▄ ▐█.▪
  ▐█▄▪▐█▐█▄▄▌ ▐█▌· • ▐█▄▪▐█▐█▄▄▌ ▐█▌·
  ·▀▀▀▀  ▀▀▀  ▀▀▀  •  ▀▀▀▀  ▀▀▀  ▀▀▀
*/
/*
 * Function: getSceneObjects()
 * returns the vector array of sceneObjects.
 * this is being called by the _GlWidget class.
 * Created:26_02_2019
*/
std::vector<_Renderer*> _Scene::getSceneObjects(){
    return renderObjects;
}
/*
 * Function: addCamera(_Camera c)
 * function checks if the cmaera is attached and sets the local
 * camera object with the camera object passed via parameter for use in rendering
 * and setting the view matrix in the renderer
 * Created:26_02_2019
*/
void _Scene::addCamera(_Camera c){
    isCamera = true;
    cam = c;
}
/*
 * Function: updateCamera(_Camera c)
 * sets the camera updated values to every render entity matrix
 * Created:26_02_2019
*/
void _Scene::updateCamera(_Camera c){
    cam = c;
    if(isCamera == true)
        for (uint i = 0; i < renderObjects.size(); i++){
            renderObjects[i]->setCamViewMatrix(c.getEyePosition(),c.getFocalPoint(),c.getUpVector());
            renderObjects[i]->setProjectionMatrix(resW,resH,cam.getFOV(),cam.getNearClipDistance(),cam.getFarClipDistance());
        }
}
/*
 * Functuon: findSceneEntity()
 * return the required Entity from index
*!!!Non Optimal function avoid using the find Funtions in a loop!!!!!
*!!!You risk increasing the complexity of the loop from On to O(posinIndex - n)^2!!!
*They need to be run only once to find the object.
*/
/*
 * Created: 10_06_2019
*/
_SceneEntity _Scene::findSceneEntity(uint iD){
    _SceneEntity empty;
    for(int f = 0 ; f < renderObjects.size() ; f++){
        if(renderObjects[f]->getSceneEntity().getId() == iD)
            return renderObjects[f]->getSceneEntity();
    }
    return empty;
}
_SceneEntity _Scene::findSceneEntity(QString tag){
    _SceneEntity empty;
    for(int f = 0 ; f < renderObjects.size(); f++){
        if(renderObjects[f]->getSceneEntity().getTag() == tag)
            return renderObjects[f]->getSceneEntity();
    }
    return empty;
}
/* Function:getSceneEntityHitWithRay()
 * returns the SceneEntity that is hitBy the mousePointer ray.
 * Created: 10_06_2019
*/
_SceneEntity _Scene::getSceneEntityHitWithRay(){
    return rayHitSceneEntity;
}

/*
 * Created: 5_06_2019
 */
void _Scene::removeSceneObject(uint index){
    renderObjects.erase(renderObjects.begin()+index);
}
void _Scene::removeSceneObject(_SceneEntity s){
    for(int r = 0 ; r < renderObjects.size() ; r++)
        if(renderObjects[r]->getSceneEntity().getId() == s.getId()){
            renderObjects[r] == NULL;
            renderObjects.erase(renderObjects.begin()+r);
        }
}
/*
  • ▌ ▄ ·.       ▄• ▄▌.▄▄ · ▄▄▄ .   ▄▄▄ .▄• ▄▌ ▄▄ .  ▐ ▄  ▄▄▄▄▄
  ·██ ▐███▪▪     █▪ █▌▐█ ▀. ▀▄.▀·   ▀▄.▀·█▪ █▌▀▄.▀· •█▌▐█ •██
  ▐█ ▌▐▌▐█· ▄█▀▄ █▌▐█▌▄▀▀▀█▄▐▀▀▪▄   ▐▀▀▪▄█▌ █▌▐▀▀▪▄ ▐█▐▐▌  ▐█.▪
  ██ ██▌▐█▌▐█▌.▐▌▐█▄█▌▐█▄▪▐█▐█▄▄▌   ▐█▄▄▌▐█▄█ ▐█▄▄▌ ██▐█▌  ▐█▌·
  ▀▀  █▪▀▀▀ ▀█▄▀▪ ▀▀▀  ▀▀▀▀  ▀▀▀     ▀▀▀  ▀▀  ▀▀▀  ▀▀ █▪  ▀▀▀
*/
/*
 * Created: 3_05_2019
*/
void _Scene::setMousePositionInScene(QVector2D mousePos,Qt::MouseButton m){
    if(m == Qt::RightButton){
        mousePositionR = mousePos;
    }
    else if(m == Qt::LeftButton){
        mousePositionL = mousePos;
        //Physics+Helpers update on Left MouseClick only
        updateAllPhysicsObjectsOnce();//Physics update//should be paralalised.
        updateHelpersOnce();//Helper Update for visual aid.
    }
    else if(m == Qt::MiddleButton){
        //        updateAllPhysicsObjectsOnce();//Physics update//should be paralalised.
        //        updateHelpersOnce();//Helper Update for visual aid.
    }
}
/*
        ▐ ▄     ▄▄▄  ▄▄▄ ..▄▄ · ▪  ·▄▄▄▄•▄▄▄ .
 ▪     •█▌▐█    ▀▄ █·▀▄.▀·▐█ ▀. ██ ▪▀·.█▌▀▄.▀·
  ▄█▀▄ ▐█▐▐▌    ▐▀▀▄ ▐▀▀▪▄▄▀▀▀█▄▐█·▄█▀▀▀•▐▀▀▪▄
 ▐█▌.▐▌██▐█▌    ▐█•█▌▐█▄▄▌▐█▄▪▐█▐█▌█▌▪▄█▀▐█▄▄▌
  ▀█▄▀▪▀▀ █▪    .▀  ▀ ▀▀▀  ▀▀▀▀ ▀▀▀·▀▀▀ • ▀▀▀
*/
/*
 * Function: onResize(int w,int h)
 * gets called on resize and all operations will run when the windows is resized
 * this is being called by the _GlWidget class.
 * Created:26_02_2019
*/
void _Scene::onResize(int w,int h){
    resW = w;
    resH = h;
    for (uint i = 0; i < renderObjects.size(); i++)
        renderObjects[i]->setProjectionMatrix(w,h,cam.getFOV(),cam.getNearClipDistance(),cam.getFarClipDistance());
    //FBO init and updateTexture on Resize
    fboObject->initialise();//initialised here buecause this is the closest function that runs right after the openglContext is initialised in _glwidgetclass
    fboObject->setupFramebuffer(w,h);//FBO buffer and textures getSetup here.
}
/*
  ▄• ▄▌ ▄▄▄··▄▄▄▄   ▄▄▄· ▄▄▄▄▄▄▄▄ .
  █▪██▌▐█ ▄███▪ ██ ▐█ ▀█ •██  ▀▄.▀·
  █▌▐█▌ ██▀·▐█· ▐█▌▄█▀▀█  ▐█.▪▐▀▀▪▄
  ▐█▄█▌▐█▪·•██. ██ ▐█ ▪▐▌ ▐█▌·▐█▄▄▌
   ▀▀▀ .▀   ▀▀▀▀▀•  ▀  ▀  ▀▀▀  ▀▀▀
*/
/*
 * Function: render()
 * This function is render function that will call the glDraw fuinction in
 * the render final draw of all sceneEntity objects attached to scene.
 * this is being called by the _GlWidget class.
 * Created:26_02_2019
*/
void _Scene::render()
{
    //sets the Frame for the framebufferObject.
    fboObject->setUpdatedFrame();// Rhe frames are being bound underneath in the draw() function below
    //--------------------------------------
    //Frame to render is below
    for (uint i = 0; i < renderObjects.size(); i++)
    {
        //Frame update----
        //Render all objects that are active.
        renderObjects[i]->draw();//calls the draw function unique to each renderObject.

        //Physics+Helpers
        //updateAllPhysicsObjectsLoop(i);//Physics Loop//should be paralalised.//Nothing happening here yet
        //updateHelpersLoop(i);//Helper update for visualAid.!!!Very slow ass function in loop no idea why Yet!!!!
    }
    //-----------------------------------------
    //Frame above is loaded in buffers and rendered on FBOquad below
    fboObject->setMousePos(mousePositionR); //sets the mouse pointervalues for the shader applied on the FBOquad
    fboObject->renderFrameOnQuad(); // sets the frame on the Quad that has been hardcoded into the function
}

/*
 ▄▄▄· ▄ .▄ ▄· ▄▌.▄▄ · ▪   ▄▄· .▄▄ ·
▐█ ▄███▪▐█▐█▪██▌▐█ ▀. ██ ▐█ ▌▪▐█ ▀.
 ██▀·██▀▐█▐█▌▐█▪▄▀▀▀█▄▐█·██ ▄▄▄▀▀▀█▄
▐█▪·•██▌▐▀ ▐█▀·.▐█▄▪▐█▐█▌▐███▌▐█▄▪▐█
.▀   ▀▀▀ ·  ▀ •  ▀▀▀▀ ▀▀▀·▀▀▀  ▀▀▀▀
*/
/*
 * Function: updatePhysicsForAllObjects()
 * update the physcs variables realtime or on MouseClick as currently configured
 * is called in the _scene class's render() function.
 * Created: 22_05_2019
 */
void _Scene::updateAllPhysicsObjectsOnce()
{
    for (uint index = 0; index < physVector.size(); index++){
        //Passing some essentials into the updateLoop for physics
        //updates the physics object instance and runs the main physics updateOperations.
        physVector[index].updatePhysics(glm::vec2(mousePositionL.x(),mousePositionL.y()),
                                        glm::vec3(cam.getEyePosition().x(),//Camera Position
                                                  cam.getEyePosition().y(),
                                                  cam.getEyePosition().z()),
                                        glm::vec2(resW,resH));

        //updates the status of scneEntity variable that get changed inside the Physis calss on Collision Events.
        uint pi = physVector[index].getSceneEntity().getIndexPosInScene();
        renderObjects[pi]->setSceneEntityInRenderer(physVector[index].getSceneEntity());//Is needed if we need to see changes to the sceneEntity in the main render as well.

        if(renderObjects[pi]->getSceneEntity().getisHitByRay()){
            rayHitSceneEntity = renderObjects[index]->getSceneEntity();
        }
    }
}
void _Scene::updatePhysics(glm::vec2 mousePos,glm::vec3 camPos,glm::vec2 screenRes,uint index){

}
void _Scene::updateAllPhysicsObjectsLoop(uint index)
{
    //Nothing yet
}

/*
   ▄ .▄▄▄▄ .▄▄▌   ▄▄▄·▄▄▄ .▄▄▄  .▄▄ ·
  ██▪▐█▀▄.▀·██•  ▐█ ▄█▀▄.▀·▀▄ █·▐█ ▀.
  ██▀▐█▐▀▀▪▄██▪   ██▀·▐▀▀▪▄▐▀▀▄ ▄▀▀▀█▄
  ██▌▐▀▐█▄▄▌▐█▌▐▌▐█▪·•▐█▄▄▌▐█•█▌▐█▄▪▐█
  ▀▀▀ · ▀▀▀ .▀▀▀ .▀    ▀▀▀ .▀  ▀ ▀▀▀▀
*/
/*
 * Created:22_06_2019
 */
void _Scene::updateHelpersOnce()
{
    glm::vec4 mx,mn,cntrd;
    glm::vec3 rot,pos;
    for (uint i = 0; i < physVector.size(); i++){
        if(physVector[i].getSceneEntity().getIsPhysicsHelper() && physVector[i].getSceneEntity().getisHitByRay())
        {
            glm::vec3 p = physVector[i].getRayTriIntersectionPoint();
            renderObjects[mPointerIndex]->setPosition(p);
            //sc = physVector[i].getSceneEntity().getScale();

            //Temporary Helpers for Max min extents
            mx = physVector[i].getSceneEntity().getModelInfo().getMaxExtent();
            mn = physVector[i].getSceneEntity().getModelInfo().getMinExtent();
            cntrd = physVector[i].getSceneEntity().getModelInfo().getCentroid();

            pos = physVector[i].getSceneEntity().getPostion();
            rot = physVector[i].getSceneEntity().getRotation();
        }
    }
    renderObjects[cIndex]->setPosition(cntrd);

    renderObjects[mxIndex]->setPosition(glm::vec3(mx.x,mx.y,mx.z));
    //renderObjects[mxIndex]->setscale(sc * 0.05);
    renderObjects[mxIndex]->lookAt(cam.getEyePosition());//buggy lookat
    renderObjects[mxIndex]->setRotation(glm::vec3(1.5,0.0,0.0));

    renderObjects[minIndex]->setPosition(glm::vec3(mn.x,mn.y,mn.z));
    //renderObjects[minIndex]->setscale(sc * 0.05);
    //renderObjects[minIndex]->lookAt(cam.getEyePosition());//buggy look at
    renderObjects[minIndex]->setRotation(glm::vec3(1.5,0.0,0.0));

    renderObjects[pivotIndex]->setPosition(pos);
    renderObjects[pivotIndex]->setRotation(rot);
}
//Not in use
void _Scene::updateHelpersLoop(uint index)
{
    if(renderObjects[index]->getSceneEntity().getIsPhysicsHelper() &&  renderObjects[index]->getSceneEntity().getisHitByRay()){
        // binding the pivot object to focus object

        renderObjects[pivotIndex]->setPosition(renderObjects[index]->getSceneEntity().getPostion());
        renderObjects[pivotIndex]->setRotation(renderObjects[index]->getSceneEntity().getRotation());
    }
}

void _Scene::setHelperIndexVars()
{
    pivotIndex = findSceneEntity("pivot").getIndexPosInScene();
    cIndex = findSceneEntity("cent").getIndexPosInScene();
    mxIndex = findSceneEntity("max").getIndexPosInScene();
    minIndex = findSceneEntity("min").getIndexPosInScene();
    mPointerIndex = findSceneEntity("mousePointerObject").getIndexPosInScene();
}
/*
 * Created:22_06_2019
*/
void _Scene::addAllHelperTypesInScene()
{
    //Code to be excluded only for Test purposes
    //Hard coded vertices and indices
    std::vector<float> vertsV = {
        1.0,   1.0,  0.0f,	// top right
        1.0f, -1.0f, 0.0f,  // bottom right
        -1.0f, -1.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f  // top left
    };
    std::vector<uint> indiceV = {0, 1, 3,
                                 1, 2, 3 };

    //PreLoad ScenenEnties with desired properties.
    //implemented 12_06_2018
    _AssetLoader quad;
    _ModelInfo m;
    m.setName("quad");
    m.setVertexArray(vertsV);
    m.setIndexArray(indiceV);
    quad.setModelInfo(m);

    //Essential rear background object
    bg.setId(777);
    bg.setTag("background");
    bg.setShader(":/shaders/vshader_background.glsl", ":/shaders/fshader_background.glsl");//texture Compliable shader not complete//need to pass UVs externally//
    bg.setTexturePath(":textures/grid.jpg");//needs a texture compliable shader attached too
    bg.setScale(1.0);
    bg.setModelData(quad);
    //
    addSceneObject(bg); //add the backGround quad first for it to render last
    //----------Physics Helpers-------
    sph.setId(1);
    sph.setTag("boundingSphere");
    sph.setIsLineMode(true);
    sph.setPhysicsObject(_SceneEntity::Sphere);
    sph.setIsTransformationLocal(false);//keep it false(true only if object need to move like physics boides or particles)
    sph.setShader(":/shaders/dmvshader.glsl", ":/shaders/dmfshader.glsl");
    sph.setColor(QVector4D(0.3,0.5,0.0,0.9));
    sph.setScale(1.0f);
    sph.setModelData(":/models/sphere.obj");
    sph.setIsActive(false);
    //---
    bb.setId(2);
    bb.setTag("boundingBox");
    bb.setIsLineMode(true);
    bb.setPhysicsObject(_SceneEntity::Box,_SceneEntity::Helper);
    bb.setIsTransformationLocal(false);
    bb.setShader(":/shaders/dmvshader.glsl", ":/shaders/dmfshader.glsl");
    bb.setColor(QVector4D(0.5,1.0,1.0,0.9));
    bb.setScale(1.0f);
    bb.setModelData(":/models/cube.obj");//dont need to reparse modelfile
    bb.setIsActive(false);
    //
    addSceneObject(sph);
    addSceneObject(bb);

    //----------Orentation Helpers---------------
    pivot.setId(888);
    pivot.setTag("pivot");
    pivot.setShader(":/shaders/dmvshader.glsl", ":/shaders/dmfshader.glsl");//texture Compliable shader not complete//need to pass UVs externally//
    pivot.setColor(QVector4D(1.0,1.0,1.0,1.0));
    pivot.setScale(1.0f);
    pivot.setModelData(":/models/pivot.obj");
    //---
    mpnt.setId(999);
    mpnt.setTag("mousePointerObject");
    mpnt.setIsTransformationLocal(false);
    mpnt.setShader(":/shaders/dmvshader.glsl", ":/shaders/dmfshader.glsl");
    mpnt.setScale(0.02f);
    mpnt.setModelData(sph.getModelInfo());
    //---
    cnet.setId(991);
    cnet.setTag("cent");
    cnet.setIsTransformationLocal(false);
    cnet.setShader(":/shaders/dmvshader.glsl", ":/shaders/dmfshader.glsl");
    cnet.setScale(0.07f);
    cnet.setModelData(sph.getModelInfo());
    //---
    max.setId(992);
    max.setTag("max");
    max.setShader(":/shaders/dmvshader.glsl", ":/shaders/dmfshader.glsl");
    max.setColor(QVector4D(0.5,0.5,0.5,1.0));
    max.setScale(0.1f);
    max.setModelData(":/models/helpers/max.obj");
    //---
    min.setId(993);
    min.setTag("min");
    min.setShader(":/shaders/dmvshader.glsl", ":/shaders/dmfshader.glsl");
    min.setScale(0.1f);
    min.setModelData(":/models/helpers/min.obj");
    //
    addSceneObject(pivot);
    addSceneObject(mpnt);
    addSceneObject(cnet);
    addSceneObject(min);
    addSceneObject(max);

    setHelperIndexVars();
}
