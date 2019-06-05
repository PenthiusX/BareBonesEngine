#include <iostream>
#include "_glwidget.h"
#include "_tools.h"


/*
 * The _GLWidget Class:
 * This class is the Controller is a typical MVC where the
 * View and Controller are being handeled by the QT GUI frameowrk and
 * its wrapper functions for creating Opengl and GUI windowing context.
 * Created: 5_02_2019
 * Author: Aditya,Saurabh
*/

/*
* Constructor:_GLWidget(QWidget *parent) : QOpenGLWidget(parent)
* sets and passes the context for the Qopengl widget here for use.
* Created: 5_02_2019
*/
_GLWidget::_GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    idmatch = 0;
    this->isCamFocus = false;
    //keeps the event callbacks working for the GL widget
    setFocusPolicy(Qt::StrongFocus);
    //  makeCurrent();
}
_GLWidget::~_GLWidget()
{
    delete scene;
}

//  ▪   ▐ ▄ ▪  ▄▄▄▄▄▪   ▄▄▄· ▄▄▌  ▪  .▄▄ · ▄▄▄ .
//  ██ •█▌▐███ •██  ██ ▐█ ▀█ ██•  ██ ▐█ ▀. ▀▄.▀·
//  ▐█·▐█▐▐▌▐█· ▐█.▪▐█·▄█▀▀█ ██▪  ▐█·▄▀▀▀█▄▐▀▀▪▄
//  ▐█▌██▐█▌▐█▌ ▐█▌·▐█▌▐█ ▪▐▌▐█▌▐▌▐█▌▐█▄▪▐█▐█▄▄▌
//  ▀▀▀▀▀ █▪▀▀▀ ▀▀▀ ▀▀▀ ▀  ▀ .▀▀▀ ▀▀▀ ▀▀▀▀  ▀▀▀
/*
* Function: initializeGL() overrides the
* same function in the OpopenglFunctions class
* runs once the opengl context is initialised.
* Created: 5_02_2019
*/
void _GLWidget::initializeGL()
{
    //Initailise the scene in InitaliseGl
    //needs to be run after the openGl contxt is initialised
    scene = new _Scene();
    //needs this to make the GL widgit have the strongest focus when switching widgets.
    cam.setEyePosition(QVector3D(0.0, 0.0, -7.0));
    cam.setFocalPoint(QVector3D(0.0, 0.0, 0.0));
    cam.setFarClipDistance(30.0f);
    cam.setFOV(50);
    //
    //Hard coded vertices and indices
    std::vector<float> vertsV = {
        1.0,   1.0,  0.0f,	// top right
        1.0f, -1.0f, 0.0f,  // bottom right
        -1.0f, -1.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f  // top left
    };
    std::vector<unsigned int> indiceV = {0, 1, 3,
                                         1, 2, 3 };
    //Essential rear background object
    background_quad.setId(777);
    background_quad.setTag("background");
    background_quad.setShader(":/shaders/vshader_background.glsl", ":/shaders/fshader_background.glsl");//texture Compliable shader not complete//need to pass UVs externally//
    background_quad.setTexturePath(":textures/grid.jpg");//needs a texture compliable shader attached too
    background_quad.setPosition(QVector3D(0.0, 0.0, 0.0));
    background_quad.setRotation(QVector3D(0.0, 0.0, 0.0));
    background_quad.setScale(1.0);
    background_quad.setModelData(vertsV,indiceV);
    //Essential default pivot object
    pivot.setId(888);
    pivot.setTag("pivot");
    pivot.setShader(":/shaders/dmvshader.glsl", ":/shaders/dmfshader.glsl");//texture Compliable shader not complete//need to pass UVs externally//
    pivot.setColor(QVector4D(1.0,1.0,1.0,1.0));
    pivot.setPosition(QVector3D(0.0, 0.0, 0.0));
    pivot.setScale(0.8f);
    pivot.setModelData(":/models/pivot.obj");
    //Debug helper use mpoint.
    mpoint.setId(999);
    mpoint.setTag("mousePointerObject");
    mpoint.setIsTransfomationLocal(false);
    mpoint.setPosition(QVector3D(0.0,0.0,0.0));
    mpoint.setShader(":/shaders/dmvshader.glsl", ":/shaders/dmfshader.glsl");
    mpoint.setColor(QVector4D(0.5,0.5,0.5,1.0));
    mpoint.setScale(0.05f);
    mpoint.setModelData(":/models/sphere.obj");
    //-----------------
    s.setId(1);
    s.setTag("object1");
    s.setIsLineMode(true);
    s.setPhysicsObject(_SceneEntity::Sphere);
    s.setIsTransfomationLocal(false);//keep it false(true only if object need to move like physics boides or particles)
    s.setShader(":/shaders/dmvshader.glsl", ":/shaders/dmfshader.glsl");
    s.setColor(QVector4D(0.3,0.5,0.0,0.5));
    s.setPosition(QVector3D(0.0,2.0, 0.0));
    s.setScale(1.0f);
    s.setModelData(":/models/sphere.obj");
    //
    s1.setId(2);
    s1.setTag("object2");
    s1.setIsLineMode(true);
    s1.setPhysicsObject(_SceneEntity::Mesh);
    s1.setIsTransfomationLocal(false);
    s1.setPosition(QVector3D(0.0,-3.0, 0.0));
    s1.setShader(":/shaders/dmvshader.glsl", ":/shaders/dmfshader.glsl");
    s1.setColor(QVector4D(0.0,0.0,0.5,0.5));
    s1.setScale(1.0f);
    s1.setModelData(s.getVertexData(),s.getIndexData());//dont need to reparse modelfile
    //
    s2.setId(3);
    s2.setTag("clickSurface");
    s2.setPhysicsObject(_SceneEntity::Mesh);
    s2.setIsTransfomationLocal(false);
    s2.setPosition(QVector3D(0.0,0.0, 0.0));
    s2.setPivot(QVector3D(2.0,0.0,0.0));//sets the pivot offset from center
    s2.setShader(":/shaders/dmvshader.glsl", ":/shaders/dmfshader.glsl");
    s2.setColor(QVector4D(0.0,0.0,0.5,0.8));
    s2.setScale(1.0f);
    s2.setModelData(vertsV,indiceV);
    //-----------------
    scene->addCamera(cam);//camera essential
    scene->addSceneObject(background_quad); //add the backGround quad first for it to render last
    scene->addSceneObject(pivot);//pivot helper essential
    scene->addSceneObject(mpoint);//mousePoint helper
    //
    scene->addSceneObject(s);
    scene->addSceneObject(s1);
    //    scene->addSceneObject(s2);
    //-----------------
}
//         ▐ ▄     ▄▄▄  ▄▄▄ ..▄▄ · ▪  ·▄▄▄▄•▄▄▄ .
//  ▪     •█▌▐█    ▀▄ █·▀▄.▀·▐█ ▀. ██ ▪▀·.█▌▀▄.▀·
//   ▄█▀▄ ▐█▐▐▌    ▐▀▀▄ ▐▀▀▪▄▄▀▀▀█▄▐█·▄█▀▀▀•▐▀▀▪▄
//  ▐█▌.▐▌██▐█▌    ▐█•█▌▐█▄▄▌▐█▄▪▐█▐█▌█▌▪▄█▀▐█▄▄▌
//   ▀█▄▀▪▀▀ █▪    .▀  ▀ ▀▀▀  ▀▀▀▀ ▀▀▀·▀▀▀ • ▀▀▀

/*
 * Function: resizeGL(int w, int h) overides the
 * function in OpopenglFunctions class.
 * pasees the current width and height
 * of the layout via - int w and int h
 * Created: 5_02_2019
*/
void _GLWidget::resizeGL(int w, int h)
{
    scene->onResize(w, h);
}
//  ▄• ▄▌ ▄▄▄··▄▄▄▄   ▄▄▄· ▄▄▄▄▄▄▄▄ .
//  █▪██▌▐█ ▄███▪ ██ ▐█ ▀█ •██  ▀▄.▀·
//  █▌▐█▌ ██▀·▐█· ▐█▌▄█▀▀█  ▐█.▪▐▀▀▪▄
//  ▐█▄█▌▐█▪·•██. ██ ▐█ ▪▐▌ ▐█▌·▐█▄▄▌
//   ▀▀▀ .▀   ▀▀▀▀▀•  ▀  ▀  ▀▀▀  ▀▀▀

/*
 * Function: paintGl()
 * ovveriding thes function in OpopenglFunctions
 * Your proprietory Draw function this run in a loop
 * till the application ends.
 * Created: 5_02_2019
*/

void _GLWidget::paintGL()//the renderloop
{
    //debug use,sets camfocus on object with the iD that is selected---------------
    for (unsigned int i = 0; i < scene->getSceneObjects().size(); i++){
        if (scene->getSceneObjects()[i]->getSceneEntity().getId() == idmatch){
            cam.setFocalPoint(scene->getSceneObjects()[i]->getSceneEntity().getPostion());
            // index 1 is set Exclusively for the pivot object
            scene->getSceneObjects()[1]->setPosition(scene->getSceneObjects()[i]->getSceneEntity().getPostion());
            scene->getSceneObjects()[1]->setRotation(scene->getSceneObjects()[i]->getSceneEntity().getRotation());
        }
    }
    scene->updateCamera(cam);//sets the specified camera to update in scene with the values pass in form the cam object
    scene->render();//renders the scene with all the prequists pass into the scene via a  sceneEntity object.
    this->update();//is to send QtOpenglGl a flag to update openglFrames
    _Tools::printFrameRate();//prints the frame rate in the application output
}


//   ▄▄·        ▐ ▄ ▄▄▄▄▄▄▄▄        ▄▄▌  ▄▄▌  .▄▄ ·
//  ▐█ ▌▪▪     •█▌▐█•██  ▀▄ █·▪     ██•  ██•  ▐█ ▀.
//  ██ ▄▄ ▄█▀▄ ▐█▐▐▌ ▐█.▪▐▀▀▄  ▄█▀▄ ██▪  ██▪  ▄▀▀▀█▄
//  ▐███▌▐█▌.▐▌██▐█▌ ▐█▌·▐█•█▌▐█▌.▐▌▐█▌▐▌▐█▌▐▌▐█▄▪▐█
//  ·▀▀▀  ▀█▄▀▪▀▀ █▪ ▀▀▀ .▀  ▀ ▀█▄▀▪.▀▀▀ .▀▀▀  ▀▀▀▀

/*
* Function: mousePressEvent(QMouseEvent *e)
* this is a overriden function from the QWidget parent
* runs each time the mouse is pressed.
* Created: 5_02_2019
*/
void _GLWidget::mousePressEvent(QMouseEvent *e)
{
    if(e->buttons() == Qt::LeftButton)
    {//get mouse position only on left button click
        mousePressPositionL = QVector2D(e->localPos());
        //        scene->setMousePositionInScene(this->mousePositionL,Qt::LeftButton);
    }
    if(e->buttons() == Qt::RightButton)
    {//get mouse position only on left button click
        mousePressPositionR = QVector2D(e->localPos());
    }
}
/*
* Function: mouseReleaseEvent(QMouseEvent *e)
* This is a overriden function from the QWidget parent
* runs each time the mouse is released.
* Created: 5_02_2019
*/
void _GLWidget::mouseReleaseEvent(QMouseEvent *e)
{
    QVector2D diff = QVector2D(e->localPos()) - mousePressPositionL;

    if(e->buttons() == Qt::LeftButton)
    {//get mouse position only on left button click
        mousePressPositionL = QVector2D(e->localPos());
        //        scene->setMousePositionInScene(this->mousePositionL,Qt::LeftButton);
    }
    if(e->buttons() == Qt::RightButton)
    {//get mouse position only on left button click
        mousePressPositionR = QVector2D(e->localPos());
    }
}
/*
* Function: mouseMoveEvent(QMouseEvent *e)
* This is a overriden function from the QWidget parent
* runs each time the mouse is pressed and moved.
* Created: 5_02_2019
*/
void _GLWidget::mouseMoveEvent(QMouseEvent *e)
{
    //selet button is pressed when updating mousevalues
    if(e->buttons() == Qt::LeftButton)
    {
        mousePositionL = QVector2D(e->localPos());
        scene->setMousePositionInScene(this->mousePositionL,Qt::LeftButton);
        //RotateTarget with mouse
        {
            QVector2D mosPosL = mousePressPositionL;
            QVector2D maxpoint = _Tools::retunrnMaxPoint(QVector2D(e->localPos()));
            if (e->localPos().x() < maxpoint.x() || e->localPos().y() < maxpoint.y()){
                mosPosL = maxpoint;
            }
            double damp = 0.00005;//to decrese the magnitude of the value coming in from the mousepos
            rotRads  += mousePositionL - mosPosL;
            for (unsigned int i = 0; i < scene->getSceneObjects().size(); i++){
                if (scene->getSceneObjects()[i]->getSceneEntity().getId() == idmatch){
                    scene->getSceneObjects()[i]->setRotation(QVector3D(rotRads.y() * damp, rotRads.x() * damp, 0.f));
                }
            }
        }
    }
    if(e->buttons() == Qt::RightButton)
    {
        mousePositionR = QVector2D(e->localPos());
        scene->setMousePositionInScene(this->mousePositionR,Qt::RightButton);//sets the mouse position in the scene for use
    }
}
/*
* Function: wheelEvent(QWheelEvent *e)
* This is a overriden function from the QWidget parent
* runs each time the mouse wheel is scrolled.
* Created: 5_02_2019
*
* Controlls:
* scrool wheel up to scale object relatively
*/
void _GLWidget::wheelEvent(QWheelEvent *e)
{
    //Scale target with mouseWheel
    int numDegrees = e->delta() / 8;
    int numSteps = numDegrees / 15;
    if (e->orientation() == Qt::Horizontal)
    {
        scroolScale = numSteps * 0.005;
        for (unsigned int i = 0; i < scene->getSceneObjects().size(); i++)
            if (scene->getSceneObjects()[i]->getSceneEntity().getId() == idmatch)
                scene->getSceneObjects()[i]->setscale(scroolScale);
    }
    else
    {
        for (unsigned int i = 0; i < scene->getSceneObjects().size(); i++){
            if (scene->getSceneObjects()[i]->getSceneEntity().getId() == idmatch)
            {
                scroolScale = scene->getSceneObjects()[i]->getSceneEntity().getScale() + (numSteps * 0.005);
                scene->getSceneObjects()[i]->setscale(scroolScale);
                // qInfo() << scroolScale;
            }
        }
    }
}
bool ist = false;
/*
* Function: keyPressEvent(QKeyEvent * event)
* runns anytime a key is presses and returns which key through the
* event pointer of QKeyEvent object.
* Created: 25_02_2019
*
 Controls: WASD to move ,
 * C to switch focut to  camera or model. R to reset to default with respect to focus
 * Q to switch between models(models need to have a sequntial ID for this to work properly)
*/
void _GLWidget::keyPressEvent(QKeyEvent * event)//Primary Debug use, not a final controlls set.
{
    if ((event->text() == "q" || event->text() == "Q"))
        idmatch += 1;
    if (idmatch >= scene->getSceneObjects().size())
        idmatch = 0;

    if (event->text() == "d" || event->text() == "D"){
        if (isCamFocus == true){
            cam.setEyePosition(QVector3D(cam.getEyePosition().x() - 0.1, cam.getEyePosition().y(), cam.getEyePosition().z()));
        }else{
            for (unsigned int i = 0; i < scene->getSceneObjects().size(); i++) {
                if (scene->getSceneObjects()[i]->getSceneEntity().getId() == idmatch) {
                    scene->getSceneObjects()[i]->translate(QVector3D(-0.1f, -0.f, 0.0));
                }
            }
        }
    }

    if (event->text() == "a" || event->text() == "A"){
        if (isCamFocus == true){
            cam.setEyePosition(QVector3D(cam.getEyePosition().x() + 0.1, cam.getEyePosition().y(), cam.getEyePosition().z()));
            scene->updateCamera(cam);
        }else{
            for (unsigned int i = 0; i < scene->getSceneObjects().size(); i++) {
                if (scene->getSceneObjects()[i]->getSceneEntity().getId() == idmatch) {
                    scene->getSceneObjects()[i]->translate(QVector3D(0.1f, 0.f, 0.0));
                }
            }
        }
    }

    if (event->text() == "w" || event->text() == "W"){
        if (isCamFocus == true){
            cam.setEyePosition(QVector3D(cam.getEyePosition().x(), cam.getEyePosition().y(), cam.getEyePosition().z() + 0.2));
            scene->updateCamera(cam);
        }else{
            for (unsigned int i = 0; i < scene->getSceneObjects().size(); i++) {
                if (scene->getSceneObjects()[i]->getSceneEntity().getId() == idmatch) {
                    scene->getSceneObjects()[i]->translate(QVector3D(0.f, 0.1, 0.0));
                }
            }
        }
    }

    if (event->text() == "s" || event->text() == "S"){
        if (isCamFocus == true){
            cam.setEyePosition(QVector3D(cam.getEyePosition().x(), cam.getEyePosition().y(), cam.getEyePosition().z() - 0.2));
            scene->updateCamera(cam);
        }else{
            for (unsigned int i = 0; i < scene->getSceneObjects().size(); i++) {
                if (scene->getSceneObjects()[i]->getSceneEntity().getId() == idmatch) {
                    scene->getSceneObjects()[i]->translate(QVector3D(-0.f, -0.1, 0.0));
                }
            }
        }
    }

    if (event->text() == "r" || event->text() == "R"){
        if(isCamFocus == true){
            cam.setEyePosition(QVector3D(0.0, 0.0, -7.0));
        }
        else{
            for (unsigned int i = 0; i < scene->getSceneObjects().size(); i++){
                if (scene->getSceneObjects()[i]->getSceneEntity().getId() == idmatch){
                    scene->getSceneObjects()[i]->setPosition(QVector3D(0.0f, 0.0, 0.0));
                    scene->getSceneObjects()[i]->setRotation(QVector3D(0.0f, 0.0, 0.0));
                    scene->getSceneObjects()[i]->setscale(scene->getSceneObjects()[i]->getSceneEntity().getScale());
                    rotRads = QVector2D(.0f, .0f);
                }
            }
        }
    }

    if (event->text() == "c" || event->text() == "C"){
        this->isCamFocus = !isCamFocus;
    }
    if (event->text() == "p" || event->text() == "P"){
        addRandomSceneEntitestoScene();
    }

    if (event->text() == "l" || event->text() == "L"){
        ist = !ist;
        //        applyStuffToallEntites(ist);
        removeSceneEntityFromScene();
    }
}

//  • ▌ ▄ ·. ▪  .▄▄ ·  ▄▄·            ▄▄▄·▄▄▄ .▄▄▄   ▄▄▄· ▄▄▄▄▄▪         ▐ ▄ .▄▄ ·
//  ·██ ▐███▪██ ▐█ ▀. ▐█ ▌▪    ▪     ▐█ ▄█▀▄.▀·▀▄ █·▐█ ▀█ •██  ██ ▪     •█▌▐█▐█ ▀.
//  ▐█ ▌▐▌▐█·▐█·▄▀▀▀█▄██ ▄▄     ▄█▀▄  ██▀·▐▀▀▪▄▐▀▀▄ ▄█▀▀█  ▐█.▪▐█· ▄█▀▄ ▐█▐▐▌▄▀▀▀█▄
//  ██ ██▌▐█▌▐█▌▐█▄▪▐█▐███▌    ▐█▌.▐▌▐█▪·•▐█▄▄▌▐█•█▌▐█ ▪▐▌ ▐█▌·▐█▌▐█▌.▐▌██▐█▌▐█▄▪▐█
//  ▀▀  █▪▀▀▀▀▀▀ ▀▀▀▀ ·▀▀▀      ▀█▄▀▪.▀    ▀▀▀ .▀  ▀ ▀  ▀  ▀▀▀ ▀▀▀ ▀█▄▀▪▀▀ █▪ ▀▀▀▀

//Press P to activate.
void _GLWidget::addRandomSceneEntitestoScene()
{
    for(int i = 0 ; i < 1 ; i++)
    {//makeCurrent() is needed if you need the openglFunctions to pickup the currentcontext,
        //especially when generating buffer ids or binding varied data on runtime,this is a windowing context (in this case Qtwidget).
        makeCurrent();
        onPress = new _SceneEntity();
        onPress->setId(scene->getSceneObjects().size() + i);
        onPress->setIsTransfomationLocal(false);
        onPress->setPhysicsObject(_SceneEntity::Mesh);
        onPress->setPosition(QVector3D(_Tools::getRandomNumberfromRange(-10,10),_Tools::getRandomNumberfromRange(-10,10), _Tools::getRandomNumberfromRange(-5,10)));
        onPress->setColor(QVector4D(_Tools::getRandomNumberfromRange(0,1),_Tools::getRandomNumberfromRange(0,1),_Tools::getRandomNumberfromRange(0,1),_Tools::getRandomNumberfromRange(0,1)));
        onPress->setShader(":/shaders/dmvshader.glsl", ":/shaders/dmfshader.glsl");
        onPress->setScale(_Tools::getRandomNumberfromRange(0.2,2));
        onPress->setModelData(s2.getVertexData(),s2.getIndexData());//dont need to reparse modelfile
        //onPress->setPhysicsObject(_Physics::Sphere);
        scene->addSceneObject(*onPress);
        delete onPress;
        qInfo() << i <<"th object";
        doneCurrent();
    }
}
/*
 *
*/
void _GLWidget::removeSceneEntityFromScene()
{
    scene->removeSceneObject(s1);
    //scene->removeSceneObject(2);
}

//Press L to activate.
void _GLWidget::applyStuffToallEntites(bool isit)
{
    for(int i = 1 ; i < scene->getSceneObjects().size() ; i++)
    {
        _SceneEntity s;//Alternate implementation to Making changes to the SceneEntity.
        s = scene->getSceneObjects()[i]->getSceneEntity();//copy the existing scene entity,
        s.setIsActive(isit);// makes specific changes then,
        scene->getSceneObjects()[i]->setSceneEntityInRenderer(s);//reSet it inside the SceneRenderer.
        qDebug() << "setting is active to" << isit <<"for" << scene->getSceneObjects()[i]->getSceneEntity().getTag();
    }
}


