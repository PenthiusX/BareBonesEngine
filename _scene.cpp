
#include "_scene.h"
#include "_tools.h"
#include <future>

/*
 * Class: _Scene()
 * This class define the scene manager , manages what needs to be rendered and what propertes need to be
 * set inside via a sceneentity object. essentially sets values in the scene entity object into the Renderer for drawing
 * Sets up Delegation to the class _Framebuffer,_Render and _Physics to work in one scene instance in cohision.
 * Autor: Aditya Mattoo

*/
//---------------------------------------------------------------------------------------
/*
 * Constructor: _Scene class
*/
_Scene::_Scene(){
    isCamera = false;
    fboObject = new _FrameBuffer();
    stencilObject = new _StencilBuffer();
    fboObject->initialise();
    isHelpers = false;
    loopIndex = 0;
    skyb.initialise();
}
_Scene::~_Scene(){

    for(auto m : meshesR){
        delete m;
    } meshesR.clear();
    for(auto m : lightsArray){
        delete m;
    }lightsArray.clear();

    delete fboObject;

}
//---------------------------------------------------------------------------------------
/*
  ▪   ▐ ▄ ▪  ▄▄▄▄▄▪   ▄▄▄· ▄▄▌  ▪  ·▄▄▄▄•▄▄▄ .
  ██ •█▌▐███ •██  ██ ▐█ ▀█ ██•  ██ ▪▀·.█▌▀▄.▀·
  ▐█·▐█▐▐▌▐█· ▐█.▪▐█·▄█▀▀█ ██▪  ▐█·▄█▀▀▀•▐▀▀▪▄
  ▐█▌██▐█▌▐█▌ ▐█▌·▐█▌▐█ ▪▐▌▐█▌▐▌▐█▌█▌▪▄█▀▐█▄▄▌
  ▀▀▀▀▀ █▪▀▀▀ ▀▀▀ ▀▀▀ ▀  ▀ .▀▀▀ ▀▀▀·▀▀▀ • ▀▀▀
*/
/*
* binds the propertes set by the scene objectes into the
* renderer/GPU  for rendering in the scene
*/
//---------------------------------------------------------------------------------------
void _Scene::addSceneObject(_SceneEntity s)
{
    // Only sets the scene object if the camera has been set already and scene object is active
    if (s.getIsActive() == true)
    {
        s.setOrderInIndex(meshesR.size());
        if (isCamera){
            initialiseMesh(s);
        }
        initialisePhysics(s);
        initialiseLights(s);
    }
}
//---------------------------------------------------------------------------------------
void _Scene::initialiseMesh(_SceneEntity s)
{
    s.setOrderInIndex(meshesR.size());
    r = new _Renderer();//creates a new renderare object for each sceneEntity that gets added to the scene
    r->setCamViewMatrix(cam.getEyePosition(), cam.getFocalPoint(), cam.getUpVector());
    r->setProjectionMatrix(resW,resH,cam.getFOV(),cam.getNearClipDistance(),cam.getFarClipDistance());
    r->initSceneEntityInRenderer(s);//sets the model data , matrix , tex and shders in the renderer
    meshesR.push_back(r);//add the renderer object to array for batch render
}
//---------------------------------------------------------------------------------------
void _Scene::initialisePhysics(_SceneEntity s)
{
    if(s.getIsPhysicsObject()){
        _Physics phys;
        phys.initialiseSceneEntity(s);
        physVector.push_back(phys);
    }
}
//---------------------------------------------------------------------------------------
void _Scene::initialiseLights(_SceneEntity s)
{
    I_Light* lightInterface = nullptr;
    if(s.getLightSource() != _SceneEntity::light::Empty)
    {
        if(s.getLightSource() == _SceneEntity::light::Point){
            lightInterface = new _PointLight(s.getTag().toStdString());
            lightInterface->setPosition(s.getPostion());
            lightInterface->setAmbDefSpec(glm::vec3(s.getColor().x(),s.getColor().y(),s.getColor().z()),glm::vec3(0.1),glm::vec3(1.0));
            lightInterface->setAdditonalParams3x3(glm::vec3(1.0),glm::vec3(0.09),glm::vec3(0.032));
            lightsArray.push_back(lightInterface);
        }
        if(s.getLightSource() == _SceneEntity::light::Directional){
            lightInterface = new _DirLight(s.getTag().toStdString());
            lightInterface->setPosition(s.getPostion());
            lightInterface->setAmbDefSpec(glm::vec3(s.getColor().x(),s.getColor().y(),s.getColor().z()),glm::vec3(1),glm::vec3(1));
            lightsArray.push_back(lightInterface);
        }
        if(s.getLightSource() == _SceneEntity::light::Spot){
            lightInterface = new _SpotLight(s.getTag().toStdString());
            lightInterface->setPosition(s.getPostion());
            lightInterface->setAmbDefSpec(glm::vec3(s.getColor().x(),s.getColor().y(),s.getColor().z()),glm::vec3(0.1),glm::vec3(1.0));
            lightInterface->setAdditonalParams2x3(glm::vec3(1.0),glm::vec3(1.0));
            lightsArray.push_back(lightInterface);
        }
    }
}
//---------------------------------------------------------------------------------------

/*
   ▄▄ • ▄▄▄ .▄▄▄▄▄   .▄▄ · ▄▄▄ .▄▄▄▄▄
  ▐█ ▀ ▪▀▄.▀·•██     ▐█ ▀. ▀▄.▀·•██
  ▄█ ▀█▄▐▀▀▪▄ ▐█.▪   ▄▀▀▀█▄▐▀▀▪▄ ▐█.▪
  ▐█▄▪▐█▐█▄▄▌ ▐█▌· • ▐█▄▪▐█▐█▄▄▌ ▐█▌·
  ·▀▀▀▀  ▀▀▀  ▀▀▀  •  ▀▀▀▀  ▀▀▀  ▀▀▀
*/
//---------------------------------------------------------------------------------------
/*
 * returns the vector array of sceneObjects.
 * this is being called by the _GlWidget class.
*/
std::vector<_Renderer*> _Scene::getSceneObjects(){
    return meshesR;
}
//---------------------------------------------------------------------------------------
/*
 * function checks if the cmaera is attached and sets the local
 * camera object with the camera object passed via parameter for use in rendering
 * and setting the view matrix in the renderer
*/
void _Scene::addCamera(_Camera c){
    isCamera = true;
    cam = c;
}
//---------------------------------------------------------------------------------------
/*
 * sets the camera updated values to every render entity matrix
*/
void _Scene::updateCamera(_Camera c){
    cam = c;
    if(isCamera == true)
        for (uint i = 0; i < meshesR.size(); i++){
            meshesR[i]->setCamViewMatrix(c.getEyePosition(),c.getFocalPoint(),c.getUpVector());
            meshesR[i]->setProjectionMatrix(resW,resH,cam.getFOV(),cam.getNearClipDistance(),cam.getFarClipDistance());
        }
}
/*
 * return the required Entity from index
*!!!Non Optimal function avoid using the find Funtions in a loop!!!!!
*!!!You risk increasing the complexity of the loop from On to O(posinIndex - n)^2!!!
*They need to be run only once to find the object.
*/
//---------------------------------------------------------------------------------------
_SceneEntity _Scene::findSceneEntity(uint iD){
    _SceneEntity empty;
    for(int f = 0 ; f < meshesR.size() ; f++){
        if(meshesR[f]->getSceneEntity().getId() == iD)
            return meshesR[f]->getSceneEntity();
    }
    return empty;
}
//---------------------------------------------------------------------------------------
_SceneEntity _Scene::findSceneEntity(QString tag){
    _SceneEntity empty;
    for(int f = 0 ; f < meshesR.size(); f++){
        if(meshesR[f]->getSceneEntity().getTag() == tag)
            return meshesR[f]->getSceneEntity();
    }
    return empty;
}
/*
 * Returns the SceneEntity that is hitBy the mousePointer ray.
*/
_SceneEntity _Scene::getSceneEntityHitWithRay(){
    return rayHitSceneEntity;
}
//---------------------------------------------------------------------------------------
void _Scene::removeSceneObject(uint index){
    meshesR.erase(meshesR.begin()+index);
}
void _Scene::removeSceneObject(_SceneEntity s){
    for(int r = 0 ; r < meshesR.size() ; r++)
        if(meshesR[r]->getSceneEntity().getId() == s.getId()){
            meshesR[r] == NULL;
            meshesR.erase(meshesR.begin()+r);
        }
}
//---------------------------------------------------------------------------------------
/*
  • ▌ ▄ ·.       ▄• ▄▌.▄▄ · ▄▄▄ .   ▄▄▄ .▄• ▄▌ ▄▄ .  ▐ ▄  ▄▄▄▄▄
  ·██ ▐███▪▪     █▪ █▌▐█ ▀. ▀▄.▀·   ▀▄.▀·█▪ █▌▀▄.▀· •█▌▐█ •██
  ▐█ ▌▐▌▐█· ▄█▀▄ █▌▐█▌▄▀▀▀█▄▐▀▀▪▄   ▐▀▀▪▄█▌ █▌▐▀▀▪▄ ▐█▐▐▌  ▐█.▪
  ██ ██▌▐█▌▐█▌.▐▌▐█▄█▌▐█▄▪▐█▐█▄▄▌   ▐█▄▄▌▐█▄█ ▐█▄▄▌ ██▐█▌  ▐█▌·
  ▀▀  █▪▀▀▀ ▀█▄▀▪ ▀▀▀  ▀▀▀▀  ▀▀▀     ▀▀▀  ▀▀  ▀▀▀  ▀▀ █▪  ▀▀▀
*/
//---------------------------------------------------------------------------------------
void _Scene::setMousePositionInScene(QVector2D mousePos,Qt::MouseButton m){
    if(m == Qt::RightButton){
        mousePositionR = mousePos;
    }
    else if(m == Qt::LeftButton){
        mousePositionL = mousePos;
        //Physics+Helpers update on detached threads
//        pu = std::thread(&_Scene::updateAllPhysicsObjectsOnce,this);
//        ph = std::thread(&_Scene::updateHelpersOnce,this);
//        if(pu.joinable()){pu.detach();}
//        if(ph.joinable()){ph.detach();}
        updateAllPhysicsObjectsOnce();
        updateHelpersOnce();
    }
    else if(m == Qt::MiddleButton){
        //Physics+Helpers update on detached threads
        pu = std::thread(&_Scene::updateAllPhysicsObjectsOnce,this);
        ph = std::thread(&_Scene::updateHelpersOnce,this);
        if(pu.joinable()){pu.detach();}
        if(ph.joinable()){ph.detach();}
    }
}
//---------------------------------------------------------------------------------------
/*
        ▐ ▄     ▄▄▄  ▄▄▄ ..▄▄ · ▪  ·▄▄▄▄•▄▄▄ .
 ▪     •█▌▐█    ▀▄ █·▀▄.▀·▐█ ▀. ██ ▪▀·.█▌▀▄.▀·
  ▄█▀▄ ▐█▐▐▌    ▐▀▀▄ ▐▀▀▪▄▄▀▀▀█▄▐█·▄█▀▀▀•▐▀▀▪▄
 ▐█▌.▐▌██▐█▌    ▐█•█▌▐█▄▄▌▐█▄▪▐█▐█▌█▌▪▄█▀▐█▄▄▌
  ▀█▄▀ ▀▀ █▪    .▀  ▀ ▀▀▀  ▀▀▀▀ ▀▀▀·▀▀▀ • ▀▀▀
*/
//---------------------------------------------------------------------------------------
/*
 * gets called on resize and all operations will run when the windows is resized
 * this is being called by the _GlWidget class.
*/
void _Scene::onResize(int w,int h){
    resW = w;
    resH = h;
    for(uint i = 0; i < meshesR.size(); i++){
        meshesR[i]->setProjectionMatrix(w,h,cam.getFOV(),cam.getNearClipDistance(),cam.getFarClipDistance());}
    //FBO init and updateTexture on Resize
    fboObject->setupFramebuffer(w,h);//FBO buffer and textures getSetup here.
}
//---------------------------------------------------------------------------------------
/*
  ▄• ▄▌ ▄▄▄··▄▄▄▄   ▄▄▄· ▄▄▄▄▄▄▄▄ .
  █▪██▌▐█ ▄███▪ ██ ▐█ ▀█ •██  ▀▄.▀·
  █▌▐█▌ ██▀·▐█· ▐█▌▄█▀▀█  ▐█.▪▐▀▀▪▄
  ▐█▄█▌▐█▪·•██. ██ ▐█ ▪▐▌ ▐█▌·▐█▄▄▌
   ▀▀▀ .▀   ▀▀▀▀▀•  ▀  ▀  ▀▀▀  ▀▀▀
*/
//---------------------------------------------------------------------------------------
/*
 * This function is render function that will call the glDraw fuinction in
 * the render final draw of all sceneEntity objects attached to scene.
 * this is being called by the _GlWidget class.
*/
void _Scene::render()
{
    fboObject->setUpdatedFrame();// The frames in context below will be captured
    //
//    skyb.draw(this->cam,resH,resW);//draw the skybox first to visualise it last.
    //
    for (uint i = 0,lrc=0; i < meshesR.size(); i++)
    {
        meshesR[i]->setOrthoProjectionMatrix(-10.0f, 10.0f, -10.0f, 10.0f,1.0,100.5);
        meshesR[i]->draw(shadowmapBuffer);//Rendering Scene Object/Primitives
        //~~~~~~~~~~~~~
        meshesR[i]->updateLightUniforms(lightsArray);//update the light uniform values in shader. From its relative LightSceneEntity
        lrc == lightsArray.size() ? lrc = 0:lrc;
        if(lightsArray[lrc]->getSignature() == meshesR[i]->getSceneEntity().getTag().toStdString()){
            //update the light objects with values from there relative meshentities in the secene
            lightsArray[lrc]->setPosition(meshesR[i]->getSceneEntity().getPostion());
            QVector4D col =  meshesR[i]->getSceneEntity().getColor();
            lightsArray[lrc]->setAmbDefSpec(glm::vec3(col.x(),col.y(),col.z()),glm::vec3(0.1),glm::vec3(1.0));
            if(lightsArray[lrc]->getLightType() == "PointLight"){
                 lightsArray[lrc]->setAdditonalParams3x3(glm::vec3(1.0),glm::vec3(0.09),glm::vec3(0.032));
            }
            lrc++;}
        //~~~~~~~~~~~~~~
    }
    fboObject->renderFrameOnQuad();// captured frame is loaded in buffers and rendered on *FBOquad*
    fboObject->setMousePos(mousePositionR); //sets the mouse pointervalues for the shader applied on the FBO quad
}
//---------------------------------------------------------------------------------------
/*
 * this function updated at setintervals based on a clock timer.
 */
void _Scene::fixedUpdate(float intervalTime)
{
    if(_Tools::setIntervals(intervalTime))//everything runs inside this if statement.
    {
        //   std::thread tUp(&_Scene::updateAllPhysicsObjectsLoop,this);
        //   if(tUp.joinable()){tUp.detach();}
        //   updateAllPhysicsObjectsLoop();
    }
}
//---------------------------------------------------------------------------------------
/*
 ▄▄▄· ▄ .▄ ▄· ▄▌.▄▄ · ▪   ▄▄· .▄▄ ·
▐█ ▄███▪▐█▐█▪██▌▐█ ▀. ██ ▐█ ▌▪▐█ ▀.
 ██▀·██▀▐█▐█▌▐█▪▄▀▀▀█▄▐█·██ ▄ ▄▀▀▀█▄
▐█ ·•██▌▐▀ ▐█▀·.▐█▄▪▐█▐█▌▐███▌▐█▄▪▐█
.▀   ▀▀▀ ·  ▀ •  ▀▀▀▀ ▀▀▀·▀▀▀  ▀▀▀▀
*/
//---------------------------------------------------------------------------------------
/*
 * update the physcs variables realtime or on MouseClick as currently configured
 * is called in the _scene class's render() function.
*/
void _Scene::updateAllPhysicsObjectsOnce(){
    if(physVector.size() > 0){
        for (uint index = 0; index < physVector.size(); index++){
            physVector[index].setSceneEntity(meshesR[physVector[index].getSceneEntity().getIndexPosInScene()]->getSceneEntity());

            //Passing some essentials into the updateLoop for physics
            //updates the physics object instance and runs the main physics updateOperations.
            physVector[index].updateMousePhysics(glm::vec2(mousePositionL.x(),mousePositionL.y()),
                                                 glm::vec3(cam.getEyePosition().x,//Camera Position
                                                           cam.getEyePosition().y,
                                                           cam.getEyePosition().z),
                                                 glm::vec2(resW,resH));

            //updates the status of scneEntity variable that get changed inside the Physis class on Collision Events.
            //style of implmentation can vary, its essentally updates the sceEntityObjet in the rendered if it is
            //getting collided with ray, So SceneEntity is the sharedVariable across classes Physics and Renderer
            uint pi = physVector[index].getSceneEntity().getIndexPosInScene();
            meshesR[pi]->setSceneEntityInRenderer(physVector[index].getSceneEntity());//Is needed if we need to see changes to the sceneEntity in the main render as well.

            if(meshesR[pi]->getSceneEntity().getisHitByRay()){
                rayHitSceneEntity = meshesR[pi]->getSceneEntity();
            }
        }
    }
}

/*
 * update the physcs variables realtime and is relativel optmised to run in a loop.
 * is called in the _scene class's fixedUpdate() function.
 */
//---------------------------------------------------------------------------------------
void _Scene::updateAllPhysicsObjectsLoop()
{    if(physVector.size() > 0){
        //        physVector[loopIndex].setSceneEntity(meshesR[physVector[loopIndex].getSceneEntity().getIndexPosInScene()]->getSceneEntity());
        //        //Passing some essentials into the updateLoop for physics
        //        //updates the physics object instance and runs the main physics updateOperations.
        //        physVector[loopIndex].updateMousePhysics(glm::vec2(mousePositionL.x(),mousePositionL.y()),
        //                                            glm::vec3(cam.getEyePosition().x(),//Camera Position
        //                                                      cam.getEyePosition().y(),
        //                                                      cam.getEyePosition().z()),
        //                                            glm::vec2(resW,resH));


        //       TriTriIntersection test,!!FITTING TEST!!----needs  modification of isHit scenario
        bool is = false;
        is = physVector[loopIndex].updateObjObjPhysics(physVector);
        _SceneEntity ss = physVector[loopIndex].getSceneEntity();
        ss.setIsHitByTri(is);
        meshesR[ss.getIndexPosInScene()]->setSceneEntityInRenderer(ss);
        //        if(meshesR[ss.getIndexPosInScene()]->getSceneEntity().getIsHitByTri()){
        //            triCollidedSceneEntity = meshesR[ss.getIndexPosInScene()]->getSceneEntity();//sets the sceneEntity that has been set by ray.
        //        }

        qDebug() << is;
        //------------------------------------------------------------------------------------------
        loopIndex++;
        if(loopIndex >= physVector.size())
            loopIndex = 0;
    }
}
/*
   ▄ .▄▄▄▄ .▄▄▌   ▄▄▄·▄▄▄ .▄▄▄  .▄▄ ·
  ██▪▐█▀▄.▀·██•  ▐█ ▄█▀▄.▀·▀▄ █·▐█ ▀.
  ██▀▐█▐▀▀▪▄██▪   ██▀·▐▀▀▪▄▐▀▀▄ ▄▀▀▀█▄
  ██▌▐▀▐█▄▄▌▐█▌▐▌▐█▪·•▐█▄▄▌▐█•█▌▐█▄▪▐█
  ▀▀▀ · ▀▀▀ .▀▀▀ .▀    ▀▀▀ .▀  ▀ ▀▀▀▀
*/
//---------------------------------------------------------------------------------------
void _Scene::updateHelpersOnce()
{
    if(isHelpers == true){
        glm::vec4 mx,mn,cntrd;
        glm::vec3 rot,pos;
        for (uint i = 0; i < physVector.size(); i++)
        {
            if(physVector[i].getSceneEntity().getIsPhysicsHelper() && physVector[i].getSceneEntity().getisHitByRay())
            {
                glm::vec3 p = physVector[i].getRayTriIntersectionPoint();
                meshesR[mPointerIndex]->setPosition(p);
                //sc = physVector[i].getSceneEntity().getScale();

                //Temporary Helpers for Max min extents
                mx = physVector[i].getSceneEntity().getModelInfo().getMaxExtent();
                mn = physVector[i].getSceneEntity().getModelInfo().getMinExtent();
                cntrd = physVector[i].getSceneEntity().getModelInfo().getCentroid();

                pos = physVector[i].getSceneEntity().getPostion();
                rot = physVector[i].getSceneEntity().getRotation();
            }
        }

        meshesR[cIndex]->setPosition(cntrd);

        meshesR[mxIndex]->setPosition(glm::vec3(mx.x,mx.y,mx.z));
        meshesR[mxIndex]->setRotation(glm::vec3(1.5,0.0,0.0));
        //meshesR[mxIndex]->lookAt(cam.getEyePosition());//buggy lookat

        meshesR[minIndex]->setPosition(glm::vec3(mn.x,mn.y,mn.z));
        meshesR[minIndex]->setRotation(glm::vec3(1.5,0.0,0.0));
        //meshesR[minIndex]->lookAt(cam.getEyePosition());//buggy look at

        meshesR[pivotIndex]->setPosition(pos);
        meshesR[pivotIndex]->setRotation(rot);
    }
}

//Not in use//---------------------------------------------------------------------------------------
void _Scene::updateHelpersLoop(uint index){
    if(meshesR[index]->getSceneEntity().getIsPhysicsHelper() &&  meshesR[index]->getSceneEntity().getisHitByRay()){
        // binding the pivot object to focus object
        meshesR[pivotIndex]->setPosition(meshesR[index]->getSceneEntity().getPostion());
        meshesR[pivotIndex]->setRotation(meshesR[index]->getSceneEntity().getRotation());
    }
}
//---------------------------------------------------------------------------------------
void _Scene::setHelperIndexVars(){
    pivotIndex = findSceneEntity("pivot").getIndexPosInScene();
    cIndex = findSceneEntity("cent").getIndexPosInScene();
    mxIndex = findSceneEntity("max").getIndexPosInScene();
    minIndex = findSceneEntity("min").getIndexPosInScene();
    mPointerIndex = findSceneEntity("mousePointerObject").getIndexPosInScene();
}
//---------------------------------------------------------------------------------------
void _Scene::addAllHelperTypesInScene()
{
    this->isHelpers = true;
    //----------Physics Helpers-------
    sph.setId(1);
    sph.setTag("boundingSphere");
    sph.setIsLineMode(true);
    sph.setPhysicsObject(_SceneEntity::Sphere,_SceneEntity::NoHelper);
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
    bb.setPhysicsObject(_SceneEntity::Box,_SceneEntity::NoHelper);
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
    addSceneObject(mpnt);
    addSceneObject(cnet);
    addSceneObject(min);
    addSceneObject(max);
    addSceneObject(pivot);
    //
    setHelperIndexVars();
}
//---------------------------------------------------------------------------------------
