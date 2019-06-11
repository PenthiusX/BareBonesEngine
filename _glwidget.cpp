#include <iostream>
#include "_glwidget.h"
#include "_tools.h"
#include <QDebug>
#define PI 3.1415926535897932384626433832795

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
    //needs this to make the GL widgit have the strongest focus when switching widgets.
    cam.setEyePosition(QVector3D(0.0, 0.0, -7.0));
    cam.setFocalPoint(QVector3D(0.0, 0.0, 0.0));
    cam.setFarClipDistance(100.0f);
    cam.setFOV(65);
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
    pivot.setScale(1.0f);
    pivot.setModelData(":/models/pivot.obj");
    //Debug helper use mpoint.
    mpoint.setId(999);
    mpoint.setTag("mousePointerObject");
    mpoint.setIsTransfomationLocal(false);
    mpoint.setPosition(QVector3D(0.0,0.0,0.0));
    mpoint.setShader(":/shaders/dmvshader.glsl", ":/shaders/dmfshader.glsl");
    mpoint.setColor(QVector4D(0.5,0.5,0.5,1.0));
    mpoint.setScale(0.1f);
    mpoint.setModelData(":/models/sphere.obj");
    //-----------------
    s.setId(1);
    s.setTag("object1");
    s.setIsLineMode(true);
    s.setPhysicsObject(_SceneEntity::Sphere);
    s.setIsTransfomationLocal(false);//keep it false(true only if object need to move like physics boides or particles)
    s.setShader(":/shaders/dmvshader.glsl", ":/shaders/dmfshader.glsl");
    s.setColor(QVector4D(0.3,0.5,0.0,0.9));
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
    s1.setColor(QVector4D(0.5,1.0,1.0,0.9));
    s1.setScale(0.4f);
    s1.setModelData(":/models/cube.obj");//dont need to reparse modelfile
    //
    s2.setId(3);
    s2.setTag("clickSurface");
    s2.setPhysicsObject(_SceneEntity::Mesh);
    s2.setIsTransfomationLocal(false);
    s2.setPosition(QVector3D(0.0,0.0, 0.0));
    s2.setPivot(QVector3D(2.0,0.0,0.0));//sets the pivot offset from center
    s2.setShader(":/shaders/dmvshader.glsl", ":/shaders/dmfshader.glsl");
    s2.setColor(QVector4D(0.0,0.0,0.5,0.9));
    s2.setScale(1.0f);
    s2.setModelData(vertsV,indiceV);
    //-----------------------------------
    //Initailise the scene in InitaliseGl
    //needs to be run after the openGl contxt is initialised
    scene = new _Scene();
    scene->addCamera(cam);//camera essential
    scene->addSceneObject(background_quad); //add the backGround quad first for it to render last
    scene->addSceneObject(pivot);//pivot helper essential
    scene->addSceneObject(mpoint);//mousePoint helper
    //
    scene->addSceneObject(s);
    scene->addSceneObject(s1);
    scene->addSceneObject(s2);

    //    generated_model.setId(666);//keep the id it will be required while updating texture
    //    generated_model.setShader(":/shaders/generated_model_vertex_edge.glsl", ":/shaders/generated_model_fragment.glsl");
    //    generated_model.setPosition(QVector3D(0.125, -1.045, 0.0));
    //    generated_model.setRotation(QVector3D(0.0, 0.0, 0.0));
    //    generated_model.setIsTransfomationLocal(false);
    //    generated_model.setScale(0.524);

    //    std::vector<float> vertsG;
    //    std::vector<unsigned int> indiceG;

    //    glm::ivec2 resolution = glm::ivec2(200,576);//wrap texture size
    //    unsigned int index[4] = {0,0,0,0};

    //    for (unsigned int h = 0; h < resolution.y; h++){
    //        for (unsigned int w = 0; w < resolution.x; w++){

    //            glm::vec2 pixel_cord = glm::vec2(w,h);
    //            glm::vec2 texture_cord = glm::vec2((pixel_cord.x +0.5)/resolution.x,(pixel_cord.y +0.5)/resolution.y);

    //            //texture_positions
    //            vertsG.push_back(texture_cord.x);//x = s
    //            vertsG.push_back(texture_cord.y);//y = t
    //            vertsG.push_back(0.0);//z = 0.0
    //        }
    //    }

    //    glm::ivec2 step_size = glm::ivec2(2,8);

    //    for (unsigned int h = 0; h < resolution.y; h+=step_size.y){
    //        for (unsigned int w = 0; w < resolution.x; w+=step_size.x){

    //            glm::vec2 pixel_cord = glm::vec2(w,h);

    //            //indexes of neibhouring vertexes
    //            index[0] = _Tools::indexFromPixelCordinates(pixel_cord,resolution);
    //            index[1] = _Tools::indexFromPixelCordinates(pixel_cord+glm::vec2(step_size.x,0),resolution);
    //            index[2] = _Tools::indexFromPixelCordinates(pixel_cord+glm::vec2(step_size.x,step_size.y),resolution);
    //            index[3] = _Tools::indexFromPixelCordinates(pixel_cord+glm::vec2(0,step_size.y),resolution);

    //            if((pixel_cord.y < resolution.y))
    //            {
    //                //indexs of fisrt triangle in quad
    //                indiceG.push_back(index[0]);
    //                indiceG.push_back(index[1]);
    //                indiceG.push_back(index[2]);

    //                //indexs of second triangle in quad
    //                indiceG.push_back(index[0]);
    //                indiceG.push_back(index[2]);
    //                indiceG.push_back(index[3]);
    //            }
    //        }
    //    }

    //    generated_model.setModelData(vertsG,indiceG);
    //    scene->addSceneObject(generated_model);
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
    cam.setFocalPoint(scene->findSceneEntity(idmatch).getPostion());
    // index 1 is set Exclusively for the pivot object
    scene->getSceneObjects()[1]->setPosition(scene->findSceneEntity(idmatch).getPostion());
    scene->getSceneObjects()[1]->setRotation(scene->findSceneEntity(idmatch).getRotation());

    scene->updateCamera(cam);//sets the specified camera to update in scene with the values pass in form the cam object
    scene->render();//renders the scene with all the prequists pass into the scene via a  sceneEntity object.
    this->update();//is to send QtOpenglGl a flag to update openglFrames
    _Tools::printFrameRate(1);//prints the frame rate in the application output
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
            scene->getSceneObjects()[scene->findSceneEntity(idmatch).getIndexPosInScene()]->setRotation(QVector3D(rotRads.y() * damp, rotRads.x() * damp, 0.f));
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
        scene->getSceneObjects()[scene->findSceneEntity(idmatch).getIndexPosInScene()]->setscale(scroolScale);
    }
    else
    {
        scroolScale = scene->findSceneEntity(idmatch).getScale() + (numSteps * 0.005);
        scene->getSceneObjects()[scene->findSceneEntity(idmatch).getIndexPosInScene()]->setscale(scroolScale);
    }
}

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
        if (isCamFocus)
            cam.setEyePosition(QVector3D(cam.getEyePosition().x() - 0.1, cam.getEyePosition().y(), cam.getEyePosition().z()));
        else
            scene->getSceneObjects()[scene->findSceneEntity(idmatch).getIndexPosInScene()]->translate(QVector3D(-0.1f, -0.f, 0.0));
    }

    if (event->text() == "a" || event->text() == "A"){
        if (isCamFocus){
            cam.setEyePosition(QVector3D(cam.getEyePosition().x() + 0.1, cam.getEyePosition().y(), cam.getEyePosition().z()));
            scene->updateCamera(cam);
        }else
            scene->getSceneObjects()[scene->findSceneEntity(idmatch).getIndexPosInScene()]->translate(QVector3D(0.1f, 0.f, 0.0));
    }

    if (event->text() == "w" || event->text() == "W"){
        if (isCamFocus){
            cam.setEyePosition(QVector3D(cam.getEyePosition().x(), cam.getEyePosition().y(), cam.getEyePosition().z() + 0.2));
            scene->updateCamera(cam);
        }else
            scene->getSceneObjects()[scene->findSceneEntity(idmatch).getIndexPosInScene()]->translate(QVector3D(0.f, 0.1, 0.0));
    }

    if (event->text() == "s" || event->text() == "S"){
        if (isCamFocus == true){
            cam.setEyePosition(QVector3D(cam.getEyePosition().x(), cam.getEyePosition().y(), cam.getEyePosition().z() - 0.2));
            scene->updateCamera(cam);
        }else
            scene->getSceneObjects()[scene->findSceneEntity(idmatch).getIndexPosInScene()]->translate(QVector3D(-0.f, -0.1, 0.0));
    }

    if (event->text() == "r" || event->text() == "R"){
        if(isCamFocus){
            cam.setEyePosition(QVector3D(0.0, 0.0, -7.0));
        }
        else{
            scene->getSceneObjects()[scene->findSceneEntity(idmatch).getIndexPosInScene()]->setPosition(QVector3D(0.0f, 0.0, 0.0));
            scene->getSceneObjects()[scene->findSceneEntity(idmatch).getIndexPosInScene()]->setRotation(QVector3D(0.0f, 0.0, 0.0));
            scene->getSceneObjects()[scene->findSceneEntity(idmatch).getIndexPosInScene()]->setscale(1.0f);
            rotRads = QVector2D(0.0f,0.0f);
        }
    }
    if (event->text() == "c" || event->text() == "C")
        this->isCamFocus = !isCamFocus;
    if (event->text() == "p" || event->text() == "P")
        addRandomSceneEntitestoScene();
    if (event->text() == "l" || event->text() == "L")
        removeSceneEntityFromScene();
    //        ist = !ist;
    //        applyStuffToallEntites(ist);
}

/*
 * Randomly generate scene objects and add to scene
 * currenty is buggy and not proper
 * still in development
 */

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
    //    scene->removeSceneObject(s1);
    if(scene->getSceneObjects().size() > 3)
        scene->removeSceneObject(scene->getSceneObjects().size()-1);
}

/* Function : update_background_image(char *img, unsigned int w, unsigned int h)
 * upadtes the texture when new camera image is grabbed or saved image is to be displayed
 * to upadte the texture image 8 bit grayscale image is required hence do not set the texure
 * while creating scenentity else color format mismatch will occur between GL_RGBA and GL_RED
 * since default texture format is GL_RGBA
 *
 * created: 11_04_2019
 * Contributor : Saurabh
*/
void _GLWidget::update_background_image(char *img, unsigned int w, unsigned int h)
{
    static _Renderer *render_object = nullptr;
    for (unsigned int i = 0; i < scene->getSceneObjects().size(); i++)
    {
        render_object = scene->getSceneObjects()[i];

        if (render_object->getSceneEntity().getId() == background_quad.getId())
        {
            if(render_object->isTexturePresent())
            {
                //updating predefined texture
                //                render_object->setTexture(img,w,h);
            }
            else
            {
                //setting up new 8 bit grayscale GL_RGBA texture for first time
                //                render_object->setupTexture(img,w,h,GL_RGBA);
            }
            doneCurrent();
        }
    }
}

void _GLWidget::showGeneratedModel(char *img, unsigned int w, unsigned int h)
{
    static _Renderer *render_object = nullptr;
    for (unsigned int i = 0; i < scene->getSceneObjects().size(); i++)
    {
        render_object = scene->getSceneObjects()[i];

        if (render_object->getSceneEntity().getId() == generated_model.getId())
        {
            if(render_object->isTexturePresent()){
                //updating predefined texture
                //                render_object->setTexture(img,w,h);
            }
            else {
                //setting up new 8 bit grayscale GL_RGBA texture for first time
                //                render_object->setupTexture(img,w,h,GL_RGBA);
            }

        }
    }
}

void _GLWidget::rotateGeneratedModel(float angle)
{
    rotateGeneratedmodel(angle, glm::vec3(0.0f, 1.0f, 0.0f),true);
}

void _GLWidget::rotateGeneratedmodel(float angle,glm::vec3 axis,bool with_stage)
{
    static _Renderer *render_object = nullptr;

    if(initialised)
    {
        for (unsigned int i = 0; i < scene->getSceneObjects().size(); i++)
        {
            render_object = scene->getSceneObjects()[i];

            if (render_object->getSceneEntity().getId() == generated_model.getId())
            {
                if(with_stage)
                {
                    //rotate generatedModel with stage
                }
                else
                {
                    //rotate generatedModel
                }
            }
        }
    }
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
