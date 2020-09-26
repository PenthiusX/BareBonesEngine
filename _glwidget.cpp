#include <iostream>
#include "_glwidget.h"
#include "_tools.h"

#include "_text.h"
#include "_material.h"

/*
 * The _GLWidget Class:
 * This class is the Controller is a typical MVC where the
 * View and Controller are being handeled by the QT GUI frameowrk and
 * its wrapper functions for creating Opengl and GUI windowing context.

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

}
_GLWidget::~_GLWidget(){
    delete scene;
}
/*
  ▪   ▐ ▄ ▪  ▄▄▄▄▄▪   ▄▄▄· ▄▄▌  ▪  .▄▄ · ▄▄▄ .
  ██ •█▌▐███ •██  ██ ▐█ ▀█ ██•  ██ ▐█ ▀. ▀▄.▀·
  ▐█·▐█▐▐▌▐█· ▐█.▪▐█·▄█▀▀█ ██▪  ▐█·▄▀▀▀█▄▐▀▀▪▄
  ▐█▌██▐█▌▐█▌ ▐█▌·▐█▌▐█ ▪▐▌▐█▌▐▌▐█▌▐█▄▪▐█▐█▄▄▌
  ▀▀▀▀▀ █▪▀▀▀ ▀▀▀ ▀▀▀ ▀  ▀ .▀▀▀ ▀▀▀ ▀▀▀▀  ▀▀▀*/
/*
 * initializeGL() overrides the
* same function in the OpopenglFunctions class
* runs once the opengl context is initialised.
*/
//Text
 _Text text;
void _GLWidget::initializeGL() {
    //needs to be run after the openGl contxt is initialised
    scene = new _Scene();
    //-------------Camera-----------------------------
    cam.setEyePosition(glm::vec3(0.0, 0.0, 10.0));
    cam.setFocalPoint(glm::vec3(0.0, 0.0, 0.0));
    cam.setFarClipDistance(100.0f);
    cam.setFOV(65);
    //-------------------------------------------------

    //-------------Lights------------------------------
    dl1->setId(8008);
    dl1->setTag("dlight");
    dl1->setAsLight(_SceneEntity::Directional);
    dl1->setModelData(":/models/cube.obj");
    dl1->setPhysicsObject(_SceneEntity::Sphere);
    dl1->setPosition(glm::vec3(-4.4f,15.76f, 1.3f));//hard coded value need to get passed into the shader
    dl1->setIsLineNoCullMode(false);
    dl1->setScale(0.20f);
    dl1->setColor(QVector4D(0.5,0.5,0.5,1.0));
    //
    p1->setId(8002);
    p1->setTag("plight1");
    p1->setAsLight(_SceneEntity::Point);
    p1->setModelData(":/models/sphere.obj");
    p1->setPhysicsObject(_SceneEntity::Sphere);
    p1->setPosition(glm::vec3(-1.3f,1.3f, 0.0f));//hard coded value need to get passed into the shader
    p1->setIsLineNoCullMode(false);
    p1->setScale(0.20f);
    p1->setColor(QVector4D(0.5,0.5,0.5,1.0));
    //->
    p2->setId(8003);
    p2->setTag("plight2");
    p2->setAsLight(_SceneEntity::Point);
    p2->setModelData(":/models/sphere.obj");
    p2->setPhysicsObject(_SceneEntity::Sphere);
    p2->setPosition(glm::vec3(1.3f,1.3f, 0.0f));//hard coded value need to get passed into the shader
    p2->setIsLineNoCullMode(false);
    p2->setScale(0.20f);
    p2->setColor(QVector4D(0.5,0.5,0.5,1.0));
    //--------------------------------------------------

    //------------Material Params-----------------------
    /*_Material*/ m = {};
    m.setDiffuseTexture(":/textures/Skull.jpg");//color texture
    m.setSpecularTexture(":/textures/SkullSpec.jpg");//spec texture
    m.setShine(0.5);
    m.setAmbient(glm::vec3( 0.0f, 0.0f, 0.0f));
    m.setDiffuse(glm::vec3( 0.7f, 0.7f, 0.7f));
    m.setSpecular(glm::vec3(0.1f, 0.1f, 0.1f));
    m.setShine(5.0);

    /*_Material*/ m2 = {};
    m2.setDiffuseTexture(":/textures/brickwall.jpg");//color texture
    m2.setSpecularTexture(":/textures/brickwallSpec.png");//spec texture
    m2.setShine(0.5);
    m2.setAmbient(glm::vec3( 0.0f, 0.0f, 0.0f));//0 means value only from light
    m2.setDiffuse(glm::vec3( 0.5f, 0.5f, 0.5f));//0 means value only from texture
    m2.setSpecular(glm::vec3(0.0f, 0.0f, 0.0f));//0 means value only from texture
    m2.setShine(32.0);
    //------GLRaster Ebablements ovveride---------------
    _SceneEntity::GlEnablements g;
    g.fillMode = _SceneEntity::GlEnablements::FrontAndBackFill;
    //--------------------------------------------------

    //------------------Main Meshes---------------------
    s->setId(8880);
    s->setTag("LitObject1");
    s->setIsShadowCaster(true);
    s->setGLModes(g);// glmode settings
    s->setMaterial(m);//material obhect
    s->setModelData("D:/DiamondPalRepo/DiamondPal/models/skull_blender.obj");
    s->setPhysicsObject(_SceneEntity::Sphere,_SceneEntity::Helper);
    s->setIsTransformationLocal(false);
    s->setIsLineNoCullMode(false);
    s->setPosition(glm::vec3(0.0,0.0,0.0));
    s->setShader(":/shaders/lightingVert.glsl",":/shaders/lightingFrag.glsl");
    s->setScale(0.3f);

    s2->setId(8881);
    s2->setTag("LitObject2");
    s2->setIsShadowCaster(true);
    s2->setGLModes(g);// glmode settings
    s2->setMaterial(m2);//material obhect
    s2->setModelData("D:/DiamondPalRepo/DiamondPal/models/plane_blender.obj");//Model data
    s2->setPhysicsObject(_SceneEntity::Box,_SceneEntity::Helper);//Physics object
    s2->setIsTransformationLocal(false);
    s2->setIsLineNoCullMode(false);
    s2->setPosition(glm::vec3(0.0,-4.0,0.0));//initial position
    s2->setShader(":/shaders/lightingVert.glsl",":/shaders/lightingFrag.glsl");
    s2->setScale(10.0f);

    s3->setId(8882);
    s3->setTag("LitObject3");
    s3->setIsShadowCaster(true);
    s3->setGLModes(g);// glmode settings
    s3->setMaterial(m2);//material obhect
    s3->setModelData("D:/DiamondPalRepo/DiamondPal/models/testCube.obj");//Model data
    s3->setPhysicsObject(_SceneEntity::Box,_SceneEntity::Helper);//Physics object
    s3->setIsTransformationLocal(false);
    s3->setIsLineNoCullMode(false);
    s3->setPosition(glm::vec3(-5.0,0.0,0.0));//initial position
    s3->setShader(":/shaders/lightingVert.glsl",":/shaders/lightingFrag.glsl");
    s3->setScale(2.0f);
    //--------Essentials---------------------------------
    scene->addCamera(cam);//camera essential
    //scene->addAllHelperTypesInScene();// pReLoad helpers into scene, these are fixed scene Entities.
    //-----Scene Objects---------------------------------
    scene->addSceneObject(s);
    scene->addSceneObject(s2);
    scene->addSceneObject(s3);
    //
    scene->addSceneObject(dl1);
    scene->addSceneObject(p1);
    scene->addSceneObject(p2);

    for(uint i = 0 ; i < scene->getSceneObjects().size();i++){
        triCount += (scene->getSceneObjects()[i]->getSceneEntity()->getModelInfo().getIndexArray().size() * 0.3333333333333);
    }
}
/*
         ▐ ▄     ▄▄▄  ▄▄▄ ..▄▄ · ▪  ·▄▄▄▄•▄▄▄ .
  ▪     •█▌▐█    ▀▄ █·▀▄.▀·▐█ ▀. ██ ▪▀·.█▌▀▄.▀·
   ▄█▀▄ ▐█▐▐▌    ▐▀▀▄ ▐▀▀▪▄▄▀▀▀█▄▐█·▄█▀▀▀•▐▀▀▪▄
  ▐█▌.▐▌██▐█▌    ▐█•█▌▐█▄▄▌▐█▄▪▐█▐█▌█▌▪▄█▀▐█▄▄▌
   ▀█▄▀▪▀▀ █▪    .▀  ▀ ▀▀▀  ▀▀▀▀ ▀▀▀·▀▀▀ • ▀▀▀*/
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
/*
  ▄• ▄▌ ▄▄▄··▄▄▄▄   ▄▄▄· ▄▄▄▄▄▄▄▄ .
  █▪██▌▐█ ▄███▪ ██ ▐█ ▀█ •██  ▀▄.▀·
  █▌▐█▌ ██▀·▐█· ▐█▌▄█▀▀█  ▐█.▪▐▀▀▪▄
  ▐█▄█▌▐█▪·•██. ██ ▐█ ▪▐▌ ▐█▌·▐█▄▄▌
   ▀▀▀ .▀   ▀▀▀▀▀•  ▀  ▀  ▀▀▀  ▀▀▀*/
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
    iter++;
    if(iter > 100){iter = 0; qInfo()<< 1/timePerDraw << "\n";}
    text.render(this,1/timePerDraw,scene->getSceneEntityHitWithRay()->getTag(),scene->getSceneEntityHitWithRay()->getPostion(),glm::vec3(triCount));
}
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
        scroolScale = scene->getSceneEntityHitWithRay()->getScale() + (numSteps * 0.005);
        scene->getSceneObjects()[scene->getSceneEntityHitWithRay()->getIndexPosInScene()]->setscale(scroolScale);
    }
    else if(!isCTRL){
        scroolScale = cam.getFOV() - numSteps;
        cam.setFOV(scroolScale);
        scene->updateCamera(cam);
    }
}

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

void _GLWidget::keyReleaseEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Control)
        isCTRL = false;
    if (event->text() == "c" || event->text() == "C")
        isCamFocus = false;
}

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
        delete onPress;
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
