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
    isCamFocus = false;
    isCTRL = false;
    //  keeps the event callbacks working for the GL widget
    this->setFocusPolicy(Qt::StrongFocus);
    qTimer.start();
}
_GLWidget::~_GLWidget()
{
    delete scene;
}
/*
  ▪   ▐ ▄ ▪  ▄▄▄▄▄▪   ▄▄▄· ▄▄▌  ▪  .▄▄ · ▄▄▄ .
  ██ •█▌▐███ •██  ██ ▐█ ▀█ ██•  ██ ▐█ ▀. ▀▄.▀·
  ▐█·▐█▐▐▌▐█· ▐█.▪▐█·▄█▀▀█ ██▪  ▐█·▄▀▀▀█▄▐▀▀▪▄
  ▐█▌██▐█▌▐█▌ ▐█▌·▐█▌▐█ ▪▐▌▐█▌▐▌▐█▌▐█▄▪▐█▐█▄▄▌
  ▀▀▀▀▀ █▪▀▀▀ ▀▀▀ ▀▀▀ ▀  ▀ .▀▀▀ ▀▀▀ ▀▀▀▀  ▀▀▀
*/
/*
* Function: initializeGL() overrides the
* same function in the OpopenglFunctions class
* runs once the opengl context is initialised.
* Created: 5_02_2019
*/
void _GLWidget::initializeGL()
{
    //needs to be run after the openGl contxt is initialised
    scene = new _Scene();
    //-------------Camera--------------
    cam.setEyePosition(QVector3D(0.0, 0.0, 10.0));
    cam.setFocalPoint(glm::vec3(0.0, 0.0, 0.0));
    cam.setFarClipDistance(100.0f);
    cam.setFOV(65);
    //------------Scene Objects--------
    s.setId(3);
    s.setTag("clickSurface");
    s.setPhysicsObject(_SceneEntity::Mesh,_SceneEntity::Helper);
    s.setIsTransformationLocal(false);
    s.setIsLineNoCullMode(true);
    s.setPosition(glm::vec3(0.0,0.0, 0.0));
    s.setShader(":/shaders/dmvshader.glsl", ":/shaders/dmfshader.glsl");
    s.setColor(QVector4D(0.0,0.5,0.5,0.9));
    s.setScale(2.0f);
    s.setModelData(":/models/hipolyore.obj");
    //Add stuff preloaded Scene Entities to scene;
    //--------Essentials---------------
    scene->addCamera(cam);//camera essential
    scene->addAllHelperTypesInScene();// pReLoad helpers into scene, these are fixed scene Entities.
    //-----Scene Objects---------
    scene->addSceneObject(s);
}
/*
         ▐ ▄     ▄▄▄  ▄▄▄ ..▄▄ · ▪  ·▄▄▄▄•▄▄▄ .
  ▪     •█▌▐█    ▀▄ █·▀▄.▀·▐█ ▀. ██ ▪▀·.█▌▀▄.▀·
   ▄█▀▄ ▐█▐▐▌    ▐▀▀▄ ▐▀▀▪▄▄▀▀▀█▄▐█·▄█▀▀▀•▐▀▀▪▄
  ▐█▌.▐▌██▐█▌    ▐█•█▌▐█▄▄▌▐█▄▪▐█▐█▌█▌▪▄█▀▐█▄▄▌
   ▀█▄▀▪▀▀ █▪    .▀  ▀ ▀▀▀  ▀▀▀▀ ▀▀▀·▀▀▀ • ▀▀▀
*/
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
/*
  ▄• ▄▌ ▄▄▄··▄▄▄▄   ▄▄▄· ▄▄▄▄▄▄▄▄ .
  █▪██▌▐█ ▄███▪ ██ ▐█ ▀█ •██  ▀▄.▀·
  █▌▐█▌ ██▀·▐█· ▐█▌▄█▀▀█  ▐█.▪▐▀▀▪▄
  ▐█▄█▌▐█▪·•██. ██ ▐█ ▪▐▌ ▐█▌·▐█▄▄▌
   ▀▀▀ .▀   ▀▀▀▀▀•  ▀  ▀  ▀▀▀  ▀▀▀
*/
/*
 * Function: paintGl()
 * ovveriding thes function in OpopenglFunctions
 * Your proprietory Draw function this run in a loop
 * till the application ends.
 * Created: 5_02_2019
*/
void _GLWidget::paintGL()//the renderloop
{
    //calculates Deltatime, should be used in Frame independent tranformations.
    currentTime = qTimer.elapsed() * 0.001;//sets the time elapsed
    deltaTime = currentTime - timeSinceLastFrame;//change in time Per frame
    //-------------------------
    scene->updateCamera(cam);//sets the specified camera to update in scene with the values pass in form the cam object
    //renders the scene with all the prequists pass into the scene via a  sceneEntity object.
    scene->render();
    //is to send QtOpenglGl a flag to update openglFrames
    this->update();
    _Tools::printFrameRate();//prints the frame rate in the application output
    //---------------------------
    timeSinceLastFrame = qTimer.elapsed() * 0.001;//sets the time past since the frame was completed
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
* Function: mousePressEvent(QMouseEvent *e)
* this is a overriden function from the QWidget parent
* runs each time the mouse is pressed.
* Created: 5_02_2019
*/
void _GLWidget::mousePressEvent(QMouseEvent *e){
    //convert global cursor pos to localWidgetPositions
    //needed for widgetfocus free mousePosition updates
    globalMPoint = this->mapFromGlobal(QCursor::pos());

    if(e->button() == Qt::LeftButton){
        //get mouse position only on left button click
        mousePressPositionL = QVector2D(e->localPos());
        scene->setMousePositionInScene(QVector2D(globalMPoint),Qt::LeftButton);//set mose pos in scene for use
        //sets cam focus on object with the iD that is selected
        cam.setFocalPoint(scene->getSceneEntityHitWithRay().getPostion());

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
* Function: mouseReleaseEvent(QMouseEvent *e)
* This is a overriden function from the QWidget parent
* runs each time the mouse is released.
* Created: 5_02_2019
*/

void _GLWidget::mouseReleaseEvent(QMouseEvent *e)
{
    //convert global cursor pos to localWidgetPositions
    //needed for widgetfocus free mousePosition updates
    globalMPoint = this->mapFromGlobal(QCursor::pos());
    if(e->button() == Qt::LeftButton){
        scene->setMousePositionInScene(QVector2D(globalMPoint),Qt::LeftButton);//set mose pos in scene for use
        //sets cam focus on object with the iD that is selected
        cam.setFocalPoint(scene->getSceneEntityHitWithRay().getPostion());
        //        qDebug() << "LpressRel";
    }
    if(e->button() == Qt::RightButton){
        //        qDebug() << "RpressRel";
    }
    if(e->button() == Qt::MiddleButton){
        //        qDebug() << "MpressRel";
//        scene->setMousePositionInScene(QVector2D(globalMPoint),Qt::MiddleButton);//set mose pos in scene for use
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
    //convert global cursor pos to localWidgetPositions
    //needed for widgetfocus free mousePosition updates
    globalMPoint = this->mapFromGlobal(QCursor::pos());
    //selet button is pressed when updating mousevalues
    if(e->buttons() == Qt::LeftButton){
        mousePositionL = QVector2D(e->localPos());
        //        qDebug() << "LpressMv";
    }
    if(e->buttons() == Qt::RightButton){
        mousePositionR = QVector2D(e->localPos());
        scene->setMousePositionInScene(mousePositionR,Qt::RightButton);//sets the mouse position in the scene
        //        qDebug() << "RpressMv";
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
            scene->getSceneObjects()[scene->getSceneEntityHitWithRay().getIndexPosInScene()]->setRotation(glm::vec3(rotRads.y() * damp, rotRads.x() * damp, 0.f));
            //        qDebug() << "MpressMv";
        }
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
    if(isCTRL){
        scroolScale = scene->getSceneEntityHitWithRay().getScale() + (numSteps * 0.005);
        scene->getSceneObjects()[scene->getSceneEntityHitWithRay().getIndexPosInScene()]->setscale(scroolScale);
    }
    else{
        scroolScale = cam.getFOV() - numSteps;
        cam.setFOV(scroolScale);
        scene->updateCamera(cam);
    }
}

/*
* Function: keyPressEvent(QKeyEvent * event)
* runns anytime a key is presses and returns which key through the
* event pointer of QKeyEvent object.
* Created: 25_02_2019
*/
void _GLWidget::keyPressEvent(QKeyEvent * event)//Primary Debug use, not a final controlls set.
{
    if (event->text() == "a" || event->text() == "A"){
        if (isCamFocus)cam.setEyePosition(QVector3D(cam.getEyePosition().x() - 0.1, cam.getEyePosition().y(), cam.getEyePosition().z()));
        else scene->getSceneObjects()[scene->getSceneEntityHitWithRay().getIndexPosInScene()]->translate(glm::vec3(-0.1f, -0.f, 0.0));
    }
    if (event->text() == "d" || event->text() == "D"){
        if (isCamFocus){
            cam.setEyePosition(QVector3D(cam.getEyePosition().x() + 0.1, cam.getEyePosition().y(), cam.getEyePosition().z()));
            scene->updateCamera(cam);
        }else scene->getSceneObjects()[scene->getSceneEntityHitWithRay().getIndexPosInScene()]->translate(glm::vec3(0.1f, 0.f, 0.0));
    }
    if (event->text() == "s" || event->text() == "S"){
        if (isCamFocus){
            cam.setEyePosition(QVector3D(cam.getEyePosition().x(), cam.getEyePosition().y(), cam.getEyePosition().z() + 0.2));
            scene->updateCamera(cam);
        }else scene->getSceneObjects()[scene->getSceneEntityHitWithRay().getIndexPosInScene()]->translate(glm::vec3(0.f, 0.1, 0.0));
    }
    if (event->text() == "w" || event->text() == "W"){
        if (isCamFocus == true){
            cam.setEyePosition(QVector3D(cam.getEyePosition().x(), cam.getEyePosition().y(), cam.getEyePosition().z() - 0.2));
            scene->updateCamera(cam);
        }else scene->getSceneObjects()[scene->getSceneEntityHitWithRay().getIndexPosInScene()]->translate(glm::vec3(-0.f, -0.1, 0.0));
    }
    if (event->text() == "r" || event->text() == "R"){
        if(isCamFocus)
            cam.setEyePosition(QVector3D(0.0, 0.0, 7.0));
        else{
            scene->getSceneObjects()[scene->getSceneEntityHitWithRay().getIndexPosInScene()]->setPosition(glm::vec3(0.0f, 0.0, 0.0));
            scene->getSceneObjects()[scene->getSceneEntityHitWithRay().getIndexPosInScene()]->setRotation(glm::vec3(0.0f, 0.0, 0.0));
            scene->getSceneObjects()[scene->getSceneEntityHitWithRay().getIndexPosInScene()]->setscale(1.0f);
            rotRads = QVector2D(0.0f,0.0f);
        }
    }
    if (event->text() == "c" || event->text() == "C")
        isCamFocus = true;
    if (event->text() == "p" || event->text() == "P")
        addRandomSceneEntitestoScene();
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
void _GLWidget::addRandomSceneEntitestoScene()
{
    for(int i = 0 ; i < 1 ; i++)
    {   //makeCurrent() is needed if you need the openglFunctions to pickup the currentcontext,
        //especially when generating buffer ids or binding varied data on runtime,this is a windowing context (in this case Qtwidget).
        makeCurrent();
        onPress = new _SceneEntity();
        onPress->setId(scene->getSceneObjects().size() + i);
        onPress->setIsTransformationLocal(false);
        onPress->setPhysicsObject(s.getPhysicsObjectType());
        onPress->setPosition(glm::vec3(_Tools::getRandomNumberfromRangeF(-10,10),_Tools::getRandomNumberfromRangeF(-10,10), _Tools::getRandomNumberfromRangeF(-5,10)));
        onPress->setRotation(glm::vec3(_Tools::getRandomNumberfromRangeF(-10,10),_Tools::getRandomNumberfromRangeF(-10,10), _Tools::getRandomNumberfromRangeF(-5,10)));
        onPress->setColor(QVector4D(_Tools::getRandomNumberfromRangeF(0,1),_Tools::getRandomNumberfromRangeF(0,1),_Tools::getRandomNumberfromRangeF(0,1),_Tools::getRandomNumberfromRangeF(0,1)));
        onPress->setShader(":/shaders/dmvshader.glsl", ":/shaders/dmfshader.glsl");
        onPress->setScale(_Tools::getRandomNumberfromRangeF(0.2,2));
        onPress->setModelData(s.getModelInfo());//dont need to reparse modelfile
        //onPress->setPhysicsObject(_Physics::Sphere);
        scene->addSceneObject(*onPress);
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
