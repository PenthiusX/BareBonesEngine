#include <iostream>
#include "_glwidget.h"
#include "_tools.h"

/*
 * The _GLWidget Class:
 * This class is the Controller is a typical MVC where the
 * View and Controller are being handeled by the QT GUI frameowrk and
 * its wrapper functions for creating Opengl and GUI windowing context.
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
    id = 0;

}
_GLWidget::~_GLWidget()
{
    delete scene;
}
/*
* Function: initializeGL() overrides the
* same function in the OpopenglFunctions class
* runs once the opengl context is initialised.
* Created: 5_02_2019
*/
void _GLWidget::initializeGL()
{
    //needs this to make the GL widgit have the strongest focus when switching widgets.
    //keeps the event callbacks working for the GL widget
    setFocusPolicy(Qt::StrongFocus);
    //
    cam.setEyePosition(QVector3D(0.0, 0.0, -7.0));
    cam.setFocalPoint(QVector3D(0.0, 0.0, 0.0));
    cam.setFOV(30);
    //
    background_quad.setShader(":/shaders/vshader_background.glsl", ":/shaders/fshader_background.glsl");//texture Compliable shader not complete//need to pass UVs externally//
    background_quad.setTexturePath(":textures/grid.jpg");//needs a texture compliable shader attached too
    background_quad.setPosition(QVector3D(0.0, 0.0, 0.0));
    background_quad.setRotation(QVector3D(0.0, 0.0, 0.0));
    background_quad.setScale(1.0);
    //Hard coded vertices and indices
    std::vector<float> vertsV = {
        1.0,  1.0, 0.999f,	// top right
        1.0f, -1.0f, 0.999f,  // bottom right
        -1.0f, -1.0f, 0.999f, // bottom left
        -1.0f,  1.0f, 0.999f  // top left
       };
    std::vector<unsigned int> indiceV = {// note that we start from 0!
                                0, 1, 3,   // first triangle
                                1, 2, 3    // second triangle
                               };
    background_quad.setModelData(vertsV,indiceV);
    //
    s.setId(1);
    s.setShader(":/shaders/vshader1.glsl", ":/shaders/fshader1.glsl");
    s.setPosition(QVector3D(0.0,-0.0f, -0.0));
    s.setScale(1.0f);
    s.setModelData(":/models/sphere.obj");
    //
    s1.setId(0);
    s1.setShader(":/shaders/vshader.glsl", ":/shaders/fshader.glsl");
    s1.setPosition(QVector3D(0.0f, 0.0f, 0.0f));
    s1.setRotation(QVector3D(0.0, 0.0, 0.0));
    s1.setScale(.09f);
    s1.setModelData(":/models/stickman.obj");
    //
//    s2.setId(2);
//    s2.setShader(":/shaders/texVshader.glsl", ":/shaders/texFshader.glsl");//texture Compliable shader not complete//need to pass UVs externally//
//    s2.setPosition(QVector3D(0.0, 0.0f, 2.0));
//    s2.setRotation(QVector3D(0.0, 0.0, 0.0));
//    s2.setScale(1.f);
//    s2.setModelData(":/models/monkey.obj");
//    s2.setTexturePath(":textures/eye.png");//needs a texture compliable shader attached too
    //
    scene = new _Scene();
    scene->addCamera(cam);
    //
    scene->addSceneObject(background_quad);
    scene->addSceneObject(s);
    scene->addSceneObject(s1);
    scene->addSceneObject(s2);
}
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
 * Function: paintGl()
 * ovveriding thes function in OpopenglFunctions
 * Your proprietory Draw function this run in a loop
 * till the application ends.
 * Created: 5_02_2019
*/
void _GLWidget::paintGL()//the renderloop
{
    scene->render();
    this->update();//is to send QtOpenglGl a flag to update openglFrames
}
/*
* Function: mousePressEvent(QMouseEvent *e)
* this is a overriden function from the QWidget parent
* runs each time the mouse is pressed.
* Created: 5_02_2019
*/
void _GLWidget::mousePressEvent(QMouseEvent *e)
{
    mousePressPosition = QVector2D(e->localPos());
}
/*
* Function: mouseReleaseEvent(QMouseEvent *e)
* This is a overriden function from the QWidget parent
* runs each time the mouse is released.
* Created: 5_02_2019
*/
void _GLWidget::mouseReleaseEvent(QMouseEvent *e)
{
    QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;
}
/*
* Function: mouseMoveEvent(QMouseEvent *e)
* This is a overriden function from the QWidget parent
* runs each time the mouse is pressed and moved.
* Created: 5_02_2019
*/
void _GLWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() == Qt::RightButton)
          qDebug() << "Only right button";

    QVector2D maxpoint = _Tools::retunrnMaxPoint(QVector2D(e->localPos()));
    if(e->localPos().x() < maxpoint.x() || e->localPos().y() < maxpoint.y())
        mousePressPosition = maxpoint;

    QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;
    for (unsigned int i = 0; i < scene->getSceneObjectsArray().size(); i++)
        if (scene->getSceneObjectsArray()[i]->getSceneEntity().getId() == id)
            scene->getSceneObjectsArray()[i]->rotate(QVector3D(diff.y() * 0.0005,diff.x() * 0.0005,0.f));
}
/*
* Function: wheelEvent(QWheelEvent *e)
* This is a overriden function from the QWidget parent
* runs each time the mouse wheel is scrolled.
* Created: 5_02_2019
*/
void _GLWidget::wheelEvent(QWheelEvent *e)
{
    int numDegrees = e->delta() / 8;
    int numSteps = numDegrees / 15;
    if (e->orientation() == Qt::Horizontal)
	{
        scroolScale = numSteps * 0.005;
        for (unsigned int i = 0; i < scene->getSceneObjectsArray().size(); i++)
		{
            if (scene->getSceneObjectsArray()[i]->getSceneEntity().getId() == id)
                scene->getSceneObjectsArray()[i]->scale(scroolScale);
        }
    }
    else
    {
        for (unsigned int i = 0; i < scene->getSceneObjectsArray().size(); i++)
		{
            if (scene->getSceneObjectsArray()[i]->getSceneEntity().getId() == id)
			{
				scroolScale = scene->getSceneObjectsArray()[i]->getSceneEntity().getScale() + (numSteps * 0.005);
                scene->getSceneObjectsArray()[i]->setscale(scroolScale);
            }
        }
    }
}
/*
* Function: keyPressEvent(QKeyEvent * event)
* runns anytime a key is presses and returns which key through the
* event pointer of QKeyEvent object.
* Created: 25_02_2019
*/
void _GLWidget::keyPressEvent(QKeyEvent * event)
{
    if (event->text() == "q" || event->text() == "Q")
        id += 1;
        if (id >= scene->getSceneObjectsArray().size())
            id = 0;

    if (event->text() == "d" || event->text() == "D")
        for (unsigned int i = 0; i < scene->getSceneObjectsArray().size(); i++)
            if (scene->getSceneObjectsArray()[i]->getSceneEntity().getId() == id)
                scene->getSceneObjectsArray()[i]->rotate(QVector3D(0.0f,-0.1f,0.0));

    if (event->text() == "a" || event->text() == "A")
        for (unsigned int i = 0; i < scene->getSceneObjectsArray().size(); i++)
            if (scene->getSceneObjectsArray()[i]->getSceneEntity().getId() == id)
                scene->getSceneObjectsArray()[i]->rotate(QVector3D(0.f, 0.1f, 0.0));

    if (event->text() == "w" || event->text() == "W")
        for (unsigned int i = 0; i < scene->getSceneObjectsArray().size(); i++)
            if (scene->getSceneObjectsArray()[i]->getSceneEntity().getId() == id)
                scene->getSceneObjectsArray()[i]->translate(QVector3D(0.1f, 0.0, 0.0));

    if (event->text() == "s" || event->text() == "S")
        for (unsigned int i = 0; i < scene->getSceneObjectsArray().size(); i++)
            if (scene->getSceneObjectsArray()[i]->getSceneEntity().getId() == id)
                scene->getSceneObjectsArray()[i]->translate(QVector3D(-0.1f, 0.0, 0.0));

    if (event->text() == "r" || event->text() == "R")
        for (unsigned int i = 0; i < scene->getSceneObjectsArray().size(); i++)
        {
            if (scene->getSceneObjectsArray()[i]->getSceneEntity().getId() == id)
			{
				scene->getSceneObjectsArray()[i]->setPosition(QVector3D(0.0f, 0.0, 0.0));
				//scene->getSceneObjectsArray()[i]->setscale(0.1);
			}
        }

    if (event->text() == "c" || event->text() == "C")
        for (unsigned int i = 0; i < scene->getSceneObjectsArray().size(); i++)
            if (scene->getSceneObjectsArray()[i]->getSceneEntity().getId() == id)
               qDebug() << scene->getSceneObjectsArray()[i]->getSceneEntity().getPostion().x() << "_" << scene->getSceneObjectsArray()[i]->getSceneEntity().getPostion().y() << "_" <<scene->getSceneObjectsArray()[i]->getSceneEntity().getPostion().z();
}
