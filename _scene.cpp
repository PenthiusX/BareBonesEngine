
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
//---------------------------------------------------------------------------------------
/*
 * Constructor: _Scene class
*/
_Scene::_Scene(){
    isCamera = false;
    fboObject = new _FrameBuffer();
    stencilObject = new _StencilBuffer();
    //
    fboObject->initialise();
    isHelpers = false;
    loopIndex = 0;
    //
    sph = new _SceneEntity();bb= new _SceneEntity();s= new _SceneEntity();
    mpnt= new _SceneEntity();cnet= new _SceneEntity();max= new _SceneEntity();
    min= new _SceneEntity();pivot= new _SceneEntity();bg= new _SceneEntity();
    rayHitSceneEntity = new _SceneEntity();
    triCollidedSceneEntity = new _SceneEntity();
    //
    skyb.initialise();
    shadowBObject.init();
}
_Scene::~_Scene()
{
    for(auto m : renderEntities){delete m;}
    renderEntities.clear();
    for(auto m : lightsArray){delete m;}
    lightsArray.clear();
    for(auto p : physVector){delete p;}
    physVector.clear();

    delete fboObject;
    //delete rayHitSceneEntity;
    delete triCollidedSceneEntity;
}
//---------------------------------------------------------------------------------------
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
void _Scene::addSceneObject(_SceneEntity* s)
{
    // Only sets the scene object if the camera has been set already and scene object is active
    if (s->getIsActive() == true)
    {
        s->setOrderInIndex(renderEntities.size());
        if (isCamera){
            initialiseMesh(s);//initialise meshes for drawing
        }
        initialisePhysics(s);//For Physics calulations
        initialiseLights(s);//for Ligthing calculations
    }
}
//---------------------------------------------------------------------------------------
void _Scene::initialiseMesh(_SceneEntity* s)
{
    s->setOrderInIndex(renderEntities.size());
    renderEntity = new _Renderer();//creates a new render object for each sceneEntity that gets added to the scene
    renderEntity->setCamViewMatrix(cam.getEyePosition(), cam.getFocalPoint(), cam.getUpVector());
    renderEntity->setProjectionMatrix(resW,resH,cam.getFOV(),cam.getNearClipDistance(),cam.getFarClipDistance());
    renderEntity->initSceneEntityInRenderer(s);//sets the model data , matrix , tex and shders in the renderer
    renderEntity->setShadowDepthTex(shadowBObject.getShadowDepthTexture());
    renderEntities.push_back(renderEntity);//add the renderer object to array for batch render
}
//---------------------------------------------------------------------------------------
void _Scene::initialisePhysics(_SceneEntity* s)
{//only inits if the object has a physics component attached
    if(s->getIsPhysicsObject()){
        _Physics *phys = new _Physics();
        phys->initialiseSceneEntity(s,&this->cam);
        physVector.push_back(phys);
    }
}
//---------------------------------------------------------------------------------------
//Abstract Factory of Lights
//sets the lights based on no of scene Entites introduced to the scene
//with light modifier attached.This is set in _scene.initalise
void _Scene::initialiseLights(_SceneEntity* s)
{
    I_Light* lightInterface = nullptr;
    if(s->getLightSource() != _SceneEntity::light::Empty)
    {
        if(s->getLightSource() == _SceneEntity::light::Point){
            lightInterface = new _PointLight(s->getTag());
            lightInterface->setPosition(s->getPostion());
            lightInterface->setAmbDefSpec(glm::vec3(s->getColor().x(),s->getColor().y(),s->getColor().z()),glm::vec3(0.1),glm::vec3(1.0));
            lightInterface->setAdditonalParams3x3(glm::vec3(1.0),glm::vec3(0.09),glm::vec3(0.032));
            lightsArray.push_back(lightInterface);
        }
        if(s->getLightSource() == _SceneEntity::light::Directional){
            lightInterface = new _DirLight(s->getTag());
            lightInterface->setPosition(s->getPostion());
            lightInterface->setAmbDefSpec(glm::vec3(s->getColor().x(),s->getColor().y(),s->getColor().z()),glm::vec3(1),glm::vec3(1));
            lightsArray.push_back(lightInterface);
        }
        if(s->getLightSource() == _SceneEntity::light::Spot){
            lightInterface = new _SpotLight(s->getTag());
            lightInterface->setPosition(s->getPostion());
            lightInterface->setAmbDefSpec(glm::vec3(s->getColor().x(),s->getColor().y(),s->getColor().z()),glm::vec3(0.1),glm::vec3(1.0));
            lightInterface->setAdditonalParams2x3(glm::vec3(1.0),glm::vec3(1.0));
            lightsArray.push_back(lightInterface);
        }
    }
}
//---------------------------------------------------------------------------------------
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
    return renderEntities;
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
        for (uint i = 0; i < renderEntities.size(); i++){
            renderEntities[i]->setCamViewMatrix(c.getEyePosition(),c.getFocalPoint(),c.getUpVector());
            renderEntities[i]->setProjectionMatrix(resW,resH,cam.getFOV(),cam.getNearClipDistance(),cam.getFarClipDistance());
        }
}
/*
 * return the required Entity from index
*!!!Non Optimal function avoid using the find Funtions in a loop!!!!!
*!!!You risk increasing the complexity of the loop from On to O(posinIndex - n)^2!!!
*They need to be run only once to find the object.
*/
//---------------------------------------------------------------------------------------
_SceneEntity* _Scene::findSceneEntity(uint iD){
    _SceneEntity empty;
    for(uint f = 0 ; f < renderEntities.size() ; f++){
        if(renderEntities[f]->getSceneEntity()->getId() == iD)
            return renderEntities[f]->getSceneEntity();
    }
    return nullptr;
}
//---------------------------------------------------------------------------------------
_SceneEntity* _Scene::findSceneEntity(std::string tag){
    _SceneEntity empty;
    for(uint f = 0 ; f < renderEntities.size(); f++){
        if(renderEntities[f]->getSceneEntity()->getTag() == tag)
            return renderEntities[f]->getSceneEntity();
    }
    return nullptr;
}
/*
 * Returns the SceneEntity that is hitBy the mousePointer ray.
*/
_SceneEntity* _Scene::getSceneEntityHitWithRay(){
    return rayHitSceneEntity;
}
//---------------------------------------------------------------------------------------
void _Scene::removeSceneObject(uint index){
    renderEntities.erase(renderEntities.begin()+index);
}
void _Scene::removeSceneObject(_SceneEntity s){
    for(uint r = 0 ; r < renderEntities.size() ; r++)
        if(renderEntities[r]->getSceneEntity()->getId() == s.getId()){
            renderEntities[r] = NULL;
            renderEntities.erase(renderEntities.begin()+r);
        }
}
//---------------------------------------------------------------------------------------
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
//                pu = std::thread(&_Scene::updateAllPhysicsObjectsOnce,this);
//                ph = std::thread(&_Scene::updateHelpersOnce,this);
//                if(pu.joinable()){pu.detach();}
//                if(ph.joinable()){ph.detach();}
        updateAllPhysicsObjectsOnce();
        updateHelpersOnce();
    }
    else if(m == Qt::MiddleButton){
        //Physics+Helpers update on detached threads
        //pu = std::thread(&_Scene::updateAllPhysicsObjectsOnce,this);
        // ph = std::thread(&_Scene::updateHelpersOnce,this);
        //        if(pu.joinable()){pu.detach();}
        //        if(ph.joinable()){ph.detach();}
    }
}
//---------------------------------------------------------------------------------------
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
    for(uint i = 0; i < renderEntities.size(); i++){
        renderEntities[i]->setProjectionMatrix(w,h,cam.getFOV(),cam.getNearClipDistance(),cam.getFarClipDistance());}
    //FBO init and updateTexture on Resize
    fboObject->setupFramebuffer(w,h);//FBO buffer and textures getSetup here.
    shadowBObject.onResize(w,h);
}
//---------------------------------------------------------------------------------------
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
 * this is being called by the _GlWidget class. callstack : Glwidget.paintGL() -> this.render() next calls renderEntities[i]->draw(1)
*/
void _Scene::render()
{
    shadowBObject.startWriteToDepthBuffer();//wrtites the info drawn after
    drawMeshesForShadowBuffer(renderEntities);//draw the meshes that are throwing a shadow
    shadowBObject.stopWrite();//Binds 0 to stop write to the shadowFbuff
    //--
    fboObject->setUpdatedFrame();//wrtites the info drawn after
    //skyb.draw(this->cam,resH,resW);//draw the skybox first to visualise it last.
    //--
    drawMeshesWithLigthingInfo(renderEntities); // cutom
    //--
    //drawPhysicsVisualisations();
    //--
    fboObject->renderFrameOnQuad();//use the texture color and pass it as pixel info for the Quad used in front of the screen
    fboObject->setMousePos(mousePositionR);//sets the mouse pointervalues for the shader applied on the FBO quad
}
//-----------------------Render SubFunction called in render()
void _Scene::drawMeshesForShadowBuffer(std::vector<_Renderer *> meshesRVec)
{
    for (uint i = 0; i < meshesRVec.size(); i++)
    {
        if(meshesRVec[i]->getSceneEntity()->getTag() == "dlight"){
            sLightPos = meshesRVec[i]->getSceneEntity()->getPostion();
        }
        if(meshesRVec[i]->getSceneEntity()->getIsShadowCaster() == true){
            //sets the view to orthographics so as to have no perspective devide to the buffer image
            meshesRVec[i]->setOrthoProjectionMatrix(-20.0f, 20.0f, -20.0f, 20.0f,0.1,100.0);
            //shader just has the lightspaceMatrix * pos to capture the FBO depth tex for use in shadows
            meshesRVec[i]->draw(2);
        }
        meshesRVec[i]->setLightViewMatrix(sLightPos,glm::vec3(0),glm::vec3(0.0,1.0,0.0));
    }
}
//---------------------------Render SubFunction called in render()
void _Scene::drawMeshesWithLigthingInfo(std::vector<_Renderer *> sv)
{
    for (uint i = 0,lrc=0; i < renderEntities.size(); i++)
    {
        //-----Draw the Meshes-----
        renderEntities[i]->draw(1);//(int 1) = the index of the shader applied.

        //------Get and update Light information------
        //update the light uniform values in shader. From its relative LightSceneEntity in lightsArray
        renderEntities[i]->updateLightUniforms(lightsArray);
        lrc == lightsArray.size() ? lrc = 0:lrc;
        if(lightsArray[lrc]->getSignature() == renderEntities[i]->getSceneEntity()->getTag())
        {
            //update the light objects with values from there relative meshentities in the secene
            lightsArray[lrc]->setPosition(renderEntities[i]->getSceneEntity()->getPostion());
            QVector4D col =  renderEntities[i]->getSceneEntity()->getColor();
            lightsArray[lrc]->setAmbDefSpec(glm::vec3(col.x(),col.y(),col.z()),glm::vec3(0.1),glm::vec3(1.0));
            if(lightsArray[lrc]->getLightType() == "PointLight")
            {
                lightsArray[lrc]->setAdditonalParams3x3(glm::vec3(1.0),glm::vec3(0.09),glm::vec3(0.032));
            }
            lrc++;
        }
    }
}

/*
 * this only draws the Visualisation info and does not update the physics objects
 */
void _Scene::drawPhysicsVisualisations()
{
    for(uint iter = 0 ; iter < physVector.size(); iter++)
    {
       if(physVector[iter]->getSceneEntity()->getPhysicsObjectType() == _SceneEntity::Mesh)
            physVector[iter]->drawVisualHelper();
            updateHelpersLoop(iter);
    }
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
 *  * It holds a refrence of its respective sceneEntity and gets updated values if affected.
*/
void _Scene::updateAllPhysicsObjectsOnce(){
    std::vector<_SceneEntity*> rayhitList;
    if(physVector.size() > 0){
        for (uint index = 0; index < physVector.size(); index++)
        {
            physVector[index]->updateMousePhysics(glm::vec2(mousePositionL.x(),mousePositionL.y()),
                                                  glm::vec2(resW,resH));

            if(physVector[index]->getSceneEntity()->getisHitByRay())
            {
                rayhitList.push_back(physVector[index]->getSceneEntity());
                qInfo() << physVector[index]->getRayTriIntersectionPoint().x <<"," <<
                           physVector[index]->getRayTriIntersectionPoint().y <<"," <<
                           physVector[index]->getRayTriIntersectionPoint().z<<"\n";
            }

        }
        for(uint i = 0 ; i < rayhitList.size() ; i++){
            rayHitSceneEntity = rayhitList[0];//select the first item from the list of hit object is one behind the other
            rayhitList.clear();
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
//        physVector[loopIndex]->updateMousePhysics(glm::vec2(mousePositionL.x(),mousePositionL.y()),
//                                              glm::vec2(resW,resH));

        //       TriTriIntersection test,!!FITTING TEST!!----needs  modification of isHit scenario
        bool is = false;
        is = physVector[loopIndex]->updateObjObjPhysics(physVector);
        _SceneEntity* ss = physVector[loopIndex]->getSceneEntity();
        ss->setIsHitByTri(is);
        renderEntities[ss->getIndexPosInScene()]->setSceneEntityInRenderer(ss);
        //        if(meshesRVec[ss.getIndexPosInScene()]->getSceneEntity().getIsHitByTri()){
        //            triCollidedSceneEntity = meshesRVec[ss.getIndexPosInScene()]->getSceneEntity();//sets the sceneEntity that has been set by ray.
        //        }

        qDebug() << is;
        //------------------------------------------------------------------------------------------
        loopIndex++;
        if(loopIndex >= physVector.size())
            loopIndex = 0;
    }
}
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
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
            if(physVector[i]->getSceneEntity()->getIsPhysicsHelper() && physVector[i]->getSceneEntity()->getisHitByRay())
            {
                glm::vec3 p = physVector[i]->getRayTriIntersectionPoint();
                renderEntities[mPointerIndex]->setPosition(p);
                //sc = physVector[i].getSceneEntity().getScale();

                //Temporary Helpers for Max min extents
                mx = physVector[i]->getSceneEntity()->getModelInfo().getMaxExtent();
                mn = physVector[i]->getSceneEntity()->getModelInfo().getMinExtent();
                cntrd = physVector[i]->getSceneEntity()->getModelInfo().getCentroid();

                pos = physVector[i]->getSceneEntity()->getPostion();
                rot = physVector[i]->getSceneEntity()->getRotation();
            }
        }

        renderEntities[cIndex]->setPosition(cntrd);

        renderEntities[mxIndex]->setPosition(glm::vec3(mx.x,mx.y,mx.z));
        renderEntities[mxIndex]->setRotation(glm::vec3(1.5,0.0,0.0));
        //meshesRVec[mxIndex]->lookAt(cam.getEyePosition());//buggy lookat

        renderEntities[minIndex]->setPosition(glm::vec3(mn.x,mn.y,mn.z));
        renderEntities[minIndex]->setRotation(glm::vec3(1.5,0.0,0.0));
        //meshesRVec[minIndex]->lookAt(cam.getEyePosition());//buggy look at

        renderEntities[pivotIndex]->setPosition(pos);
        renderEntities[pivotIndex]->setRotation(rot);
    }
}

void _Scene::updateHelpersLoop(uint i){
    if(isHelpers == true){
        glm::vec4 mx,mn,cntrd;
        glm::vec3 rot,pos;

            if(physVector[i]->getSceneEntity()->getIsPhysicsHelper() && physVector[i]->getSceneEntity()->getisHitByRay())
            {
                glm::vec3 p = physVector[i]->getRayTriIntersectionPoint();
                renderEntities[mPointerIndex]->setPosition(p);
                //sc = physVector[i].getSceneEntity().getScale();

                //Temporary Helpers for Max min extents
                mx = physVector[i]->getSceneEntity()->getModelInfo().getMaxExtent();
                mn = physVector[i]->getSceneEntity()->getModelInfo().getMinExtent();
                cntrd = physVector[i]->getSceneEntity()->getModelInfo().getCentroid();

                pos = physVector[i]->getSceneEntity()->getPostion();
                rot = physVector[i]->getSceneEntity()->getRotation();
            }


        renderEntities[cIndex]->setPosition(cntrd);

        renderEntities[mxIndex]->setPosition(glm::vec3(mx.x,mx.y,mx.z));
        renderEntities[mxIndex]->setRotation(glm::vec3(1.5,0.0,0.0));
        //meshesRVec[mxIndex]->lookAt(cam.getEyePosition());//buggy lookat

        renderEntities[minIndex]->setPosition(glm::vec3(mn.x,mn.y,mn.z));
        renderEntities[minIndex]->setRotation(glm::vec3(1.5,0.0,0.0));
        //meshesRVec[minIndex]->lookAt(cam.getEyePosition());//buggy look at

        renderEntities[pivotIndex]->setPosition(pos);
        renderEntities[pivotIndex]->setRotation(rot);
    }
}
//---------------------------------------------------------------------------------------
void _Scene::setHelperIndexVars(){
    pivotIndex = findSceneEntity("pivot")->getIndexPosInScene();
    cIndex = findSceneEntity("cent")->getIndexPosInScene();
    mxIndex = findSceneEntity("max")->getIndexPosInScene();
    minIndex = findSceneEntity("min")->getIndexPosInScene();
    mPointerIndex = findSceneEntity("mousePointerObject")->getIndexPosInScene();
}
//---------------------------------------------------------------------------------------
void _Scene::addAllHelperTypesInScene()
{
        this->isHelpers = true;
        //----------Physics Helpers-------
        sph->setId(1);
        sph->setTag("boundingSphere");
        sph->setIsLineMode(true);
        sph->setPhysicsObject(_SceneEntity::Sphere,_SceneEntity::NoHelper);
        sph->setIsTransformationLocal(false);//keep it false(true only if object need to move like physics boides or particles)
        sph->setShader(":/shaders/dmvshader.glsl", ":/shaders/dmfshader.glsl");
        sph->setColor(QVector4D(0.3,0.5,0.0,0.9));
        sph->setScale(1.0f);
        sph->setModelData(":/models/sphere.obj");
        sph->setIsActive(false);
        //---
        bb->setId(2);
        bb->setTag("boundingBox");
        bb->setIsLineMode(true);
        bb->setPhysicsObject(_SceneEntity::Box,_SceneEntity::NoHelper);
        bb->setIsTransformationLocal(false);
        bb->setShader(":/shaders/dmvshader.glsl", ":/shaders/dmfshader.glsl");
        bb->setColor(QVector4D(0.5,1.0,1.0,0.9));
        bb->setScale(1.0f);
        bb->setModelData(":/models/cube.obj");//dont need to reparse modelfile
        bb->setIsActive(false);
        //
        addSceneObject(sph);
        addSceneObject(bb);
        //----------Orentation Helpers---------------
        pivot->setId(888);
        pivot->setTag("pivot");
        pivot->setShader(":/shaders/dmvshader.glsl", ":/shaders/dmfshader.glsl");//texture Compliable shader not complete//need to pass UVs externally//
        pivot->setColor(QVector4D(1.0,1.0,1.0,1.0));
        pivot->setScale(1.0f);
        pivot->setModelData(":/models/pivot.obj");
        //---
        mpnt->setId(999);
        mpnt->setTag("mousePointerObject");
        mpnt->setIsTransformationLocal(false);
        mpnt->setShader(":/shaders/dmvshader.glsl", ":/shaders/dmfshader.glsl");
        mpnt->setScale(0.09f);
        mpnt->setModelData(sph->getModelInfo());
        //---
        cnet->setId(991);
        cnet->setTag("cent");
        cnet->setIsTransformationLocal(false);
        cnet->setShader(":/shaders/dmvshader.glsl", ":/shaders/dmfshader.glsl");
        cnet->setScale(0.07f);
        cnet->setModelData(sph->getModelInfo());
        //---
        max->setId(992);
        max->setTag("max");
        max->setShader(":/shaders/dmvshader.glsl", ":/shaders/dmfshader.glsl");
        max->setColor(QVector4D(0.5,0.5,0.5,1.0));
        max->setScale(0.1f);
        max->setModelData(":/models/helpers/max.obj");
        //---
        min->setId(993);
        min->setTag("min");
        min->setShader(":/shaders/dmvshader.glsl", ":/shaders/dmfshader.glsl");
        min->setScale(0.1f);
        min->setModelData(":/models/helpers/min.obj");
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
//---------------------------------------------------------------------------------------
