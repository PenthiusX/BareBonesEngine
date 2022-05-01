#include <iostream>
#include <thread>

#include "_glwidget.h"
#include "_tools.h"

#include "_text.h"
#include "_material.h"
//---------------------------------------------------------------------------------------
/*
 * The _GLWidget Class:
 * This class is the Controller is a typical MVC where the
 * View and Controller are being handeled by the QT GUI frameowrk and
 * its wrapper functions for creating Opengl and GUI windowing context.
//---------------------------------------------------------------------------------------
*/
/*
* Constructor:_GLWidget(QWidget *parent) : QOpenGLWidget(parent)
* sets and passes the context for the Qopengl widget here for use.
*/
_GLWidget::_GLWidget(QWidget *parent) : QOpenGLWidget(parent){
    isCamFocus = false;
    isCTRL = false;
    //  keeps the event callbacks working for the GL widget
    this->setFocusPolicy(Qt::StrongFocus);
    qTimer.start();

    s3  = new _SceneEntity();
    s   = new _SceneEntity();
    s2  = new _SceneEntity();
    dl1 = new _SceneEntity();
    p1  = new _SceneEntity();
    p2  = new _SceneEntity();
    d1  = new _SceneEntity();
    sl1 = new _SceneEntity();

    glEnable(GL_MULTISAMPLE);

}
_GLWidget::~_GLWidget(){
    delete scene;
}
//---------------------------------------------------------------------------------------
/*
  ▪   ▐ ▄ ▪  ▄▄▄▄▄▪   ▄▄▄· ▄▄▌  ▪  .▄▄ · ▄▄▄ .
  ██ •█▌▐███ •██  ██ ▐█ ▀█ ██•  ██ ▐█ ▀. ▀▄.▀·
  ▐█·▐█▐▐▌▐█· ▐█.▪▐█·▄█▀▀█ ██▪  ▐█·▄▀▀▀█▄▐▀▀▪▄
  ▐█▌██▐█▌▐█▌ ▐█▌·▐█▌▐█ ▪▐▌▐█▌▐▌▐█▌▐█▄▪▐█▐█▄▄▌
  ▀▀▀▀▀ █▪▀▀▀ ▀▀▀ ▀▀▀ ▀  ▀ .▀▀▀ ▀▀▀ ▀▀▀▀  ▀▀▀*/
//---------------------------------------------------------------------------------------
/*
 * initializeGL() overrides the
* same function in the OpopenglFunctions class
* runs once the opengl context is initialised.
*/
//Text
 _Text text;
void _GLWidget::initializeGL(){
    //needs to be run after the openGl contxt is initialised
    scene = new _Scene();
    //-------------Camera-----------------------------
    cam.setEyePosition(glm::vec3(-15.0, 20.0, 30.0));
    cam.setFocalPoint(glm::vec3(0.0, 0.0, 0.0));
    cam.setFarClipDistance(100.0f);
    cam.setFOV(65);
    //-------------------------------------------------

    //-------------Lights------------------------------
    dl1->setId(8008);
    dl1->setTag("dlight");
    dl1->setAsLight(_SceneEntity::Directional);
    dl1->setModelData(":/models/cube.obj");
    dl1->setPhysicsObject(_SceneEntity::Sphere,_SceneEntity::Helper);
    dl1->setPosition(glm::vec3(7.5f,6.56f, 6.1f));//hard coded value need to get passed into the shader
    dl1->setIsLineNoCullMode(false);
    dl1->setScale(0.15f);
    dl1->setColor(QVector4D(0.5,0.5,0.5,1.0));

//    _SceneEntity * dl2 = new _SceneEntity();
//    dl2->setId(8009);
//    dl2->setTag("dlight2");
//    dl2->setAsLight(_SceneEntity::Directional);
//    dl2->setModelData(":/models/cube.obj");
//    dl2->setPhysicsObject(_SceneEntity::Sphere,_SceneEntity::Helper);
//    dl2->setPosition(glm::vec3(-7.5f,6.56f, -6.1f));//hard coded value need to get passed into the shader
//    dl2->setIsLineNoCullMode(false);
//    dl2->setScale(0.15f);
//    dl2->setColor(QVector4D(0.5,0.5,0.5,1.0));
    //
    p1->setId(8002);
    p1->setTag("plight1");
    p1->setIsShadowCaster(true);
    p1->setAsLight(_SceneEntity::Point);
    p1->setModelData(":/models/sphere.obj");
    p1->setPhysicsObject(_SceneEntity::Sphere,_SceneEntity::Helper);
    p1->setPosition(glm::vec3(-14.9f,9.40001f, -12.5f));//hard coded value need to get passed into the shader
    p1->setIsLineNoCullMode(false);
    p1->setScale(0.8);
    p1->setColor(QVector4D(0.5,0.5,0.5,1.0));
    //->
    p2->setId(8003);
    p2->setTag("plight2");
    p2->setIsShadowCaster(true);
    p2->setAsLight(_SceneEntity::Point);
    p2->setModelData(":/models/sphere.obj");
    p2->setPhysicsObject(_SceneEntity::Sphere,_SceneEntity::Helper);
    p2->setPosition(glm::vec3(-2.5f,5.3f, 0.3f));//hard coded value need to get passed into the shader
    p2->setIsLineNoCullMode(false);
    p2->setScale(0.20f);
    p2->setColor(QVector4D(0.5,0.5,0.5,1.0));

    sl1->setId(8004);
    sl1->setTag("slight2");
    sl1->setIsShadowCaster(true);
    sl1->setAsLight(_SceneEntity::Spot);
    sl1->setModelData(":/models/sphere.obj");
    sl1->setPhysicsObject(_SceneEntity::Sphere,_SceneEntity::Helper);
    sl1->setPosition(glm::vec3(2.5f,6.56f, 6.1f));//hard coded value need to get passed into the shader
    sl1->setIsLineNoCullMode(false);
    sl1->setScale(0.50f);
    sl1->setColor(QVector4D(0.5,0.5,0.5,1.0));
    //--------------------------------------------------

    //------------Material Params-----------------------
    /*_Material*/ m = {};
    m.setDiffuseTexture(":/textures/Skull.jpg");//color texture
    m.setSpecularTexture(":/textures/SkullSpec.jpg");//spec texture
    m.setAmbient(glm::vec3( 0.0f, 0.0f, 0.0f));
    m.setDiffuse(glm::vec3( 0.7f, 0.7f, 0.7f));
    m.setSpecular(glm::vec3(0.1f, 0.1f, 0.1f));
    m.setShine(5.0);

    _Material m2 = {};
    m2.setDiffuseTexture(":/textures/brickwall.jpg");//color texture
    m2.setSpecularTexture(":/textures/brickwallSpec.png");//spec texture
    m2.setAmbient(glm::vec3( 0.0f, 0.0f, 0.0f));//0 means value only from light
    m2.setDiffuse(glm::vec3( 0.5f, 0.5f, 0.5f));//0 means value only from texture
    m2.setSpecular(glm::vec3(0.0f, 0.0f, 0.0f));//0 means value only from texture
    m2.setShine(32.0);

    _Material m3 = {};
    m3.setDiffuseTexture(":/textures/diffuse_bkp2k.jpg");//color texture
    m3.setSpecularTexture(":/textures/specular_bkp2k.jpg");//spec texture
    m3.setAmbient(glm::vec3( 0.0f, 0.0f, 0.0f));//0 means value only from light
    m3.setDiffuse(glm::vec3( 0.5f, 0.5f, 0.5f));//0 means value only from texture
    m3.setSpecular(glm::vec3(0.0f, 0.0f, 0.0f));//0 means value only from texture
    m3.setShine(32.0);

    _Material m4 = {};
    //m4.setDiffuseTexture("D:/WorkSpace/BareBonesEngine/models/Animated/051F_03SET_02SHOT/tex/051F_03SET_02SHOT_Diffuse.tif");//color texture
    m4.setDiffuseTexture("D:/WorkSpace/BareBonesEngine/models/Animated/Free_Warriors_Turbosquid/textures/egyptian_red_B_texture.jpg");//color texture
    m4.setSpecularTexture("D:/WorkSpace/BareBonesEngine/models/Animated/051F_03SET_02SHOT/tex/051F_03SET_02SHOT_Specular.png");//spec texture
    m4.setAmbient(glm::vec3( 0.0f, 0.0f, 0.0f));//0 means value only from light
    m4.setDiffuse(glm::vec3( 0.5f, 0.5f, 0.5f));//0 means value only from texture
    m4.setSpecular(glm::vec3(0.0f, 0.0f, 0.0f));//0 means value only from texture
    m4.setShine(32.0);

    //------GLRaster Ebablements ovveride---------------
    _SceneEntity::GlEnablements g;
    g.fillMode = _SceneEntity::GlEnablements::FrontAndBackFill;
    //--------------------------------------------------

    //------------------Main Meshes---------------------
    s->setId(8880);
    s->setTag("Lady");
    s->setIsShadowCaster(true);
    s->setGLModes(g);// glmode settings
    s->setMaterial(m4);//material obhect
    //s->setModelData("D:/WorkSpace/BareBonesEngine/models/Animated/051F_03SET_02SHOT/MODEL/051F_03SET_02SHOT.fbx");
    //s->setModelData("D:/WorkSpace/BareBonesEngine/models/Animated/Free_Warriors_Turbosquid/models/FBX(animated)/blue/egyptian_B.fbx");
    s->setModelData("D:/WorkSpace/BareBonesEngine/models/Animated/boxSnake.fbx");
    //s->setModelData("D:/WorkSpace/BareBonesEngine/models/Animated/vampire/dancing_vampire.dae");
    //s->setModelData("E:/WorkStudy/CG/ogldev-sourceAssimpAndAnimation/ogldev-source/tutorial25_youtube/models/example1_two_bone.fbx");
    s->setPhysicsObject(_SceneEntity::Box,_SceneEntity::Helper);
    s->setIsTransformationLocal(false);
    s->setIsLineNoCullMode(false);
    s->setPosition(glm::vec3(0.0,0.0,0.0));
//    s->setShader(":/shaders/animtestV.glsl",":/shaders/animtestF.glsl");
    s->setShader(":/shaders/animtestV.glsl",":/shaders/animtestF.glsl");
    s->setScale(0.005f);

    s2->setId(8881);
    s2->setTag("LitPlane");
    s2->setIsShadowCaster(true);
    s2->setGLModes(g);// glmode settings
    s2->setMaterial(m2);//material obhect
    s2->setModelData("D:/WorkSpace/BareBonesEngine/models/plane_blender.obj");//Model data
    s2->setPhysicsObject(_SceneEntity::Mesh,_SceneEntity::Helper);//Physics object
    s2->setIsTransformationLocal(false);
    s2->setIsLineNoCullMode(false);
    s2->setPosition(glm::vec3(0.0,-0.2,0.0));//initial position
    s2->setShader(":/shaders/lightingVertUVyFlipped.glsl",":/shaders/lightingFrag.glsl");
    s2->setScale(15.0f);

    s3->setId(8882);
    s3->setTag("MonkeyFace");
    s3->setIsShadowCaster(true);
    s3->setGLModes(g);// glmode settings
    s3->setMaterial(m3);//material obhect
    s3->setModelData("D:/WorkSpace/BareBonesEngine/models/monkey.obj");//Model data
    s3->setPhysicsObject(_SceneEntity::Box,_SceneEntity::Helper);//Physics object
    s3->setIsTransformationLocal(false);
    s3->setIsLineNoCullMode(false);
    s3->setPosition(glm::vec3(0.3,8.4,-0.34));//initial position
    s3->setRotation(glm::vec3(0.04,-0.43,0.0));
    s3->setShader(":/shaders/lightingVertUVyFlipped.glsl",":/shaders/lightingFrag.glsl");
    s3->setScale(1.7f);

    //--------Essentials---------------------------------
    scene->addCamera(cam);//camera essential
    scene->addAllHelperTypesInScene();// pReLoad helpers into scene, these are fixed scene Entities.
    //Scene Objects-----------
    scene->addSceneObject(s3);
    scene->addSceneObject(s);
    //Lights------------------
    scene->addSceneObject(dl1);
//    scene->addSceneObject(dl2);//shader not having option to load up multiple dir light , WIP
    scene->addSceneObject(p1);
    scene->addSceneObject(p2);//Need to work on proceduraly load up more point lights in shader if this array size increases. Currentlt maxed at 2 harcoded in shader
    scene->addSceneObject(sl1);
    //---------Ground----------
    scene->addSceneObject(s2);//place the ground last for picking it last in intersection test,(a workaround, needs a proper solution)
    //-------------------------
    for(uint i = 0 ; i < scene->getSceneObjects().size();i++){
        triCount += (scene->getSceneObjects()[i]->getSceneEntity()->getModelInfo().getIndexArray().size() * 0.3333333333333);
    }
}
//---------------------------------------------------------------------------------------
/*
         ▐ ▄     ▄▄▄  ▄▄▄ ..▄▄ · ▪  ·▄▄▄▄•▄▄▄ .
  ▪     •█▌▐█    ▀▄ █·▀▄.▀·▐█ ▀. ██ ▪▀·.█▌▀▄.▀·
   ▄█▀▄ ▐█▐▐▌    ▐▀▀▄ ▐▀▀▪▄▄▀▀▀█▄▐█·▄█▀▀▀•▐▀▀▪▄
  ▐█▌.▐▌██▐█▌    ▐█•█▌▐█▄▄▌▐█▄▪▐█▐█▌█▌▪▄█▀▐█▄▄▌
   ▀█▄▀▪▀▀ █▪    .▀  ▀ ▀▀▀  ▀▀▀▀ ▀▀▀·▀▀▀ • ▀▀▀*/
//---------------------------------------------------------------------------------------
/*
    overides the
 * function in OpopenglFunctions class.
 * pasees the current width and height
 * of the layout via - int w and int h
*/
void _GLWidget::resizeGL(int w, int h){
    scene->onResize(w, h);
    text.onResize(w,h);
}
//---------------------------------------------------------------------------------------
/*
  ▄• ▄▌ ▄▄▄··▄▄▄▄   ▄▄▄· ▄▄▄▄▄▄▄▄ .
  █▪██▌▐█ ▄███▪ ██ ▐█ ▀█ •██  ▀▄.▀·
  █▌▐█▌ ██▀·▐█· ▐█▌▄█▀▀█  ▐█.▪▐▀▀▪▄
  ▐█▄█▌▐█▪·•██. ██ ▐█ ▪▐▌ ▐█▌·▐█▄▄▌
   ▀▀▀ .▀   ▀▀▀▀▀•  ▀  ▀  ▀▀▀  ▀▀▀*/
//---------------------------------------------------------------------------------------
/*
 * ovveriding thes function in OpopenglFunctions
 * Your proprietory Draw function this run in a loop
 * till the application ends.
*/
uint iter =0;
void _GLWidget::paintGL()//the renderloop
{
    //calculates Deltatime, should be used in Frame independent tranformations.
    currentTime = qTimer.elapsed() * 0.001;//sets the time elapsed
    deltaTime = currentTime - timeSinceLastFrame;//change in time Per frame
    //-------------------------
    scene->updateCamera(cam);//sets the specified camera to update in scene with the values pass in form the cam object
    //renders the scene with all the prequists pass into the scene via a  sceneEntity object.
    scene->render();
    //update function that updates at fixed intervals.
    scene->fixedUpdate(0.4);
    //is to send QtOpenglGl a flag to update openglFrames
    this->update();
    //prints the frame rate in the application outputmvpx
    //---------------------------
    timeSinceLastFrame = qTimer.elapsed() * 0.001;//sets the time past since the frame was completed
    float timePerDraw = timeSinceLastFrame - currentTime;

    //Rendering info as text on screen
    makeCurrent();
    iter++;
   // if(iter > 100){iter = 0; qInfo()<< 1/timePerDraw << "\n";}
    text.render(this,1/timePerDraw,scene->getSceneEntityHitWithRay()->getTag(),
                scene->getSceneEntityHitWithRay()->getPostion(),
                scene->getSceneEntityHitWithRay()->getRotation(),
                glm::vec3(scene->getSceneEntityHitWithRay()->getScale()),
                (uint)triCount);
    doneCurrent();
    //
}
//---------------------------------------------------------------------------------------
/*
   ▄▄·        ▐ ▄ ▄▄▄▄▄▄▄▄        ▄▄▌  ▄▄▌  .▄▄ ·
  ▐█ ▌▪▪     •█▌▐█•██  ▀▄ █·▪     ██•  ██•  ▐█ ▀.
  ██ ▄▄ ▄█▀▄ ▐█▐▐▌ ▐█.▪▐▀▀▄  ▄█▀▄ ██▪  ██▪  ▄▀▀▀█▄
  ▐███▌▐█▌.▐▌██▐█▌ ▐█▌·▐█•█▌▐█▌.▐▌▐█▌▐▌▐█▌▐▌▐█▄▪▐█
  ·▀▀▀  ▀█▄▀▪▀▀ █▪ ▀▀▀ .▀  ▀ ▀█▄▀▪.▀▀▀ .▀▀▀  ▀▀▀▀
*
* Controls: WASD to move ,
* C to switch focut to  camera or model. R to reset to default with respect to focus
* Click on Model to change focus
* Press and Hold C to move camera
* Press and Hold Ctrl to rotate object
*/
//---------------------------------------------------------------------------------------
/*
* this is a overriden function from the QWidget parent
* runs each time the mouse is pressed.
*/
void _GLWidget::mousePressEvent(QMouseEvent *e){
    //convert global cursor pos to localWidgetPositions
    //needed for widgetfocus free mousePosition updates
    globalMPoint = this->mapFromGlobal(QCursor::pos());

    if(e->button() == Qt::LeftButton){
        //get mouse position only on left button click
        mousePressPositionL = QVector2D(e->localPos());
        //sets the left button click on for picking in the scene for use in physics
        //        qDebug() << "Lpress";
    }
    if(e->button() == Qt::RightButton){
        //get mouse position only on left button click
        mousePressPositionR = QVector2D(e->localPos());
        //        qDebug() << "Rpress";
    }
    if(e->button() == Qt::MiddleButton){
        mousePressPositionM = QVector2D(e->localPos());
        //        qDebug() << "Mpress";
    }
}
//---------------------------------------------------------------------------------------
/*
* This is a overriden function from the QWidget parent
* runs each time the mouse is released.
*/
void _GLWidget::mouseReleaseEvent(QMouseEvent *e)
{
    //convert global cursor pos to localWidgetPositions
    //needed for widgetfocus free mousePosition updates
    globalMPoint = this->mapFromGlobal(QCursor::pos());
    if(e->button() == Qt::LeftButton){
        scene->setMousePositionInScene(QVector2D(globalMPoint),Qt::LeftButton);//set mose pos in scene for use
        cam.setFocalPoint(scene->getSceneEntityHitWithRay()->getPostion()); //sets cam focus on object with the objet that is hitwith ray in scene
        //        qDebug() << "LpressRel";
    }
    if(e->button() == Qt::RightButton){
        //        qDebug() << "RpressRel";
    }
    if(e->button() == Qt::MiddleButton){
        //        qDebug() << "MpressRel";
        scene->setMousePositionInScene(mousePressPositionL,Qt::MiddleButton);//set mose pos in scene for use
    }
}
//---------------------------------------------------------------------------------------
/*
* This is a overriden function from the QWidget parent
* runs each time the mouse is pressed and moved.
*/
void _GLWidget::mouseMoveEvent(QMouseEvent *e)
{
    //convert global cursor pos to localWidgetPositions
    //needed for widgetfocus free mousePosition updates
    globalMPoint = this->mapFromGlobal(QCursor::pos());
    //selet button is pressed when updating mousevalues
    if(e->buttons() == Qt::LeftButton){
        mousePositionL = QVector2D(e->localPos());
        //qDebug() << "LpressMv";
    }
    if(e->buttons() == Qt::RightButton){
        mousePositionR = QVector2D(e->localPos());
        scene->setMousePositionInScene(mousePositionR,Qt::RightButton);//sets the mouse position in the scene
        //qDebug() << "RpressMv";
    }
    if(e->buttons() == Qt::MiddleButton){
        mousePositionM = QVector2D(e->localPos());
        //RotateTarget with mouse
        if(isCTRL){
            QVector2D mosPos = mousePositionM;
            QVector2D maxpoint = _Tools::retunrnMaxPoint(QVector2D(e->localPos()));
            if (e->localPos().x() < maxpoint.x() || e->localPos().y() < maxpoint.y()){
                mosPos = maxpoint;
            }
            double damp = 0.01;//to decrese the magnitude of the value coming in from the mousepos
            rotRads  += QVector2D(globalMPoint) - mosPos;
            scene->getSceneObjects()[scene->getSceneEntityHitWithRay()->getIndexPosInScene()]->setRotation(glm::vec3(rotRads.y() * damp, rotRads.x() * damp, 0.f));
            //qDebug() << "MpressMv";
        }
    }
}
//---------------------------------------------------------------------------------------
/*
* This is a overriden function from the QWidget parent
* runs each time the mouse wheel is scrolled.
*
* Controlls:
* scrool wheel up to scale object relatively
*/
void _GLWidget::wheelEvent(QWheelEvent *e)
{
    //Scale target with mouseWheel
    int numDegrees = e->delta() / 8;
    int numSteps = numDegrees / 15;
    if(isCTRL){
        scroolScale = scene->getSceneEntityHitWithRay()->getScale() + (numSteps * 0.05);
        scene->getSceneObjects()[scene->getSceneEntityHitWithRay()->getIndexPosInScene()]->setscale(scroolScale);
    }
    else if(!isCTRL){
        scroolScale = cam.getFOV() - numSteps;
        cam.setFOV(scroolScale);
        scene->updateCamera(cam);
    }
}
//---------------------------------------------------------------------------------------
/*
* runns anytime a key is presses and returns which key through the
* event pointer of QKeyEvent object.
*/
void _GLWidget::keyPressEvent(QKeyEvent * event)//Primary Debug use, not a final controlls set.
{
    if (event->text() == "a" || event->text() == "A"){
        if (isCamFocus)cam.setEyePosition(glm::vec3(cam.getEyePosition().x - 0.15, cam.getEyePosition().y, cam.getEyePosition().z));
        else scene->getSceneObjects()[scene->getSceneEntityHitWithRay()->getIndexPosInScene()]->translate(glm::vec3(-0.1f, -0.f, 0.0));
    }
    if (event->text() == "d" || event->text() == "D"){
        if (isCamFocus){
            cam.setEyePosition(glm::vec3(cam.getEyePosition().x + 0.15, cam.getEyePosition().y, cam.getEyePosition().z));
        }else scene->getSceneObjects()[scene->getSceneEntityHitWithRay()->getIndexPosInScene()]->translate(glm::vec3(0.1f, 0.f, 0.0));
    }
    if (event->text() == "s" || event->text() == "S"){
        if (isCamFocus){
            cam.setEyePosition(glm::vec3(cam.getEyePosition().x, cam.getEyePosition().y - 0.15, cam.getEyePosition().z));
        }else scene->getSceneObjects()[scene->getSceneEntityHitWithRay()->getIndexPosInScene()]->translate(glm::vec3(0.f, -0.1, 0.0));
    }
    if (event->text() == "w" || event->text() == "W"){
        if (isCamFocus == true){
            cam.setEyePosition(glm::vec3(cam.getEyePosition().x, cam.getEyePosition().y + 0.15, cam.getEyePosition().z));
        }else scene->getSceneObjects()[scene->getSceneEntityHitWithRay()->getIndexPosInScene()]->translate(glm::vec3(-0.f, 0.1, 0.0));
    }
    if (event->text() == "q" || event->text() == "Q"){
        if (isCamFocus == true){
            cam.setEyePosition(glm::vec3(cam.getEyePosition().x, cam.getEyePosition().y, cam.getEyePosition().z + 0.15));
        }else scene->getSceneObjects()[scene->getSceneEntityHitWithRay()->getIndexPosInScene()]->translate(glm::vec3(-0.f, -0.0, 0.1));
    }
    if (event->text() == "e" || event->text() == "E"){
        if (isCamFocus == true){
            cam.setEyePosition(glm::vec3(cam.getEyePosition().x, cam.getEyePosition().y , cam.getEyePosition().z - 0.15));
        }else scene->getSceneObjects()[scene->getSceneEntityHitWithRay()->getIndexPosInScene()]->translate(glm::vec3(-0.f, -0.0, -0.1));
    }
    if (event->text() == "r" || event->text() == "R"){//reset
        if(isCamFocus)
            cam.setEyePosition(glm::vec3(0.0, 0.0, 7.0));
        else{
            uint ind = scene->getSceneEntityHitWithRay()->getIndexPosInScene();
            scene->getSceneObjects()[ind]->setPosition(glm::vec3(0.0f, 0.0, 0.0));
            scene->getSceneObjects()[ind]->setRotation(glm::vec3(0.0f, 0.0, 0.0));
            scene->getSceneObjects()[ind]->setscale(1.0f);
            rotRads = QVector2D(0.0f,0.0f);
        }
    }
    if (event->text() == "c" || event->text() == "C")
        isCamFocus = true;
    if (event->text() == "p" || event->text() == "P")
        addRandomSceneEntitestoScene(1);
    if (event->text() == "l" || event->text() == "L")
        removeSceneEntityFromScene();
    if(event->key() == Qt::Key_Control)
        isCTRL = true;
}
//---------------------------------------------------------------------------------------
void _GLWidget::keyReleaseEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Control)
        isCTRL = false;
    if (event->text() == "c" || event->text() == "C")
        isCamFocus = false;
}
//---------------------------------------------------------------------------------------
/*
  • ▌ ▄ ·. ▪  .▄▄ ·  ▄▄·            ▄▄▄·▄▄▄ .▄▄▄   ▄▄▄· ▄▄▄▄▄▪         ▐ ▄ .▄▄ ·
  ·██ ▐███▪██ ▐█ ▀. ▐█ ▌▪    ▪     ▐█ ▄█▀▄.▀·▀▄ █·▐█ ▀█ •██  ██ ▪     •█▌▐█▐█ ▀.
  ▐█ ▌▐▌▐█·▐█·▄▀▀▀█▄██ ▄▄     ▄█▀▄  ██▀·▐▀▀▪▄▐▀▀▄ ▄█▀▀█  ▐█.▪▐█· ▄█▀▄ ▐█▐▐▌▄▀▀▀█▄
  ██ ██▌▐█▌▐█▌▐█▄▪▐█▐███▌    ▐█▌.▐▌▐█▪·•▐█▄▄▌▐█•█▌▐█ ▪▐▌ ▐█▌·▐█▌▐█▌.▐▌██▐█▌▐█▄▪▐█
  ▀▀  █▪▀▀▀▀▀▀ ▀▀▀▀ ·▀▀▀      ▀█▄▀▪.▀    ▀▀▀ .▀  ▀ ▀  ▀  ▀▀▀ ▀▀▀ ▀█▄▀▪▀▀ █▪ ▀▀▀▀
*/
//Press P to activate.
void _GLWidget::addRandomSceneEntitestoScene(uint count)
{
    for(int i = 0 ; i < count ; i++)
    {   //makeCurrent() is needed if you need the openglFunctions to pickup the currentcontext,
        //especially when generating buffer ids or binding varied data on runtime,this is a windowing context (in this case Qtwidget).
        makeCurrent();
        onPress = new _SceneEntity();
        onPress->setId(scene->getSceneObjects().size() + i);
        onPress->setIsTransformationLocal(false);
        onPress->setIsShadowCaster(true);
        onPress->setPhysicsObject(_SceneEntity::PhysicsBody::Sphere);
//      onPress->setPhysicsObject(_SceneEntity::Mesh,_SceneEntity::Helper);
        onPress->setPosition(glm::vec3(_Tools::getRandomNumberfromRangeF(-10,10),_Tools::getRandomNumberfromRangeF(-10,10), _Tools::getRandomNumberfromRangeF(-5,10)));
        onPress->setRotation(glm::vec3(_Tools::getRandomNumberfromRangeF(-10,10),_Tools::getRandomNumberfromRangeF(-10,10), _Tools::getRandomNumberfromRangeF(-5,10)));
        onPress->setColor(QVector4D(_Tools::getRandomNumberfromRangeF(0,1),_Tools::getRandomNumberfromRangeF(0,1),_Tools::getRandomNumberfromRangeF(0,1),_Tools::getRandomNumberfromRangeF(0,1)));
        onPress->setShader(":/shaders/lightingVert.glsl", ":/shaders/lightingFrag.glsl");
        onPress->setMaterial(m);
        onPress->setScale(_Tools::getRandomNumberfromRangeF(0.2,2));
        onPress->setModelData("D:/DiamondPalRepo/DiamondPal/models/torus_blender.obj");//dont need to reparse modelfile
        scene->addSceneObject(onPress);
        qInfo()<< "created" << i <<"th object" << "id" << onPress->getId();
        //delete onPress;
        doneCurrent();
    }
}
/*
 * Press L to activate.
*/
void _GLWidget::removeSceneEntityFromScene(){
    //    scene->removeSceneObject(s1);
    if(scene->getSceneObjects().size() > 7)
        scene->removeSceneObject(scene->getSceneObjects().size()-1);
}
//---------------------------------------------------------------------------------------
