#include <iostream>
#include "_glwidget.h"

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
	cam.setEyePosition(QVector3D(0.0, 0.0, -7.0));
	cam.setFocalPoint(QVector3D(0.0, 0.0, 0.0));

	s.setId(0);
	s.setShader(":/shaders/vshader1.glsl", ":/shaders/fshader1.glsl");
	s.setPosition(QVector3D(1.0,-0.3f, -3.0));
	s.setScale(0.5);
	s.setModelData(":/models/torus.obj");

	s1.setId(1);
	s1.setShader(":/shaders/vshader.glsl", ":/shaders/fshader.glsl");
	s1.setPosition(QVector3D(0.0, 0.7, 4.0));
	s1.setRotation(QQuaternion(QVector3D(0.0, 0.0, 0.0)));
	s1.setScale(1.5);
    s1.setModelData(":/models/cone.obj");

    s2.setId(2);
    s2.setShader(":/shaders/vshader.glsl", ":/shaders/fshader.glsl");
    s2.setPosition(QVector3D(0.0, 1.7, 2.0));
    s2.setRotation(QQuaternion(QVector3D(0.0, 0.0, 0.0)));
    s2.setScale(1.5);
    s2.setModelData(":/models/monkey.obj");
    s2.setTexture(":textures/cube.png");

	scene = new _Scene();
	scene->addCamera(cam);

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
	this->width = w;
	this->height = h;
	scene->onResize(w, h);
}
/*
 * Function: paintGl() 7
 * ovveriding thes function in OpopenglFunctions
 * Your proprietory Draw function this run in a loop 
 * till the application ends.
 * Created: 5_02_2019
*/
void _GLWidget::paintGL()//the renderloop
{
	scene->render();
	this->update();
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
		for (int i = 0; i < scene->getSceneObjectsArray().size(); i++)
			if (scene->getSceneObjectsArray()[i]->getSceneEntity().getId() == id)
				scene->getSceneObjectsArray()[i]->updateTrasformations(QVector3D(0.0,0.1,0.0));

	if (event->text() == "a" || event->text() == "A")
		for (int i = 0; i < scene->getSceneObjectsArray().size(); i++)
			if (scene->getSceneObjectsArray()[i]->getSceneEntity().getId() == id)
				scene->getSceneObjectsArray()[i]->updateTrasformations(QVector3D(0.0, -0.1, 0.0));

	if (event->text() == "w" || event->text() == "W")
		for (int i = 0; i < scene->getSceneObjectsArray().size(); i++)
			if (scene->getSceneObjectsArray()[i]->getSceneEntity().getId() == id)
				scene->getSceneObjectsArray()[i]->updateTrasformations(QVector3D(0.1, 0.0, 0.0));

	if (event->text() == "s" || event->text() == "S")
		for (int i = 0; i < scene->getSceneObjectsArray().size(); i++)
			if (scene->getSceneObjectsArray()[i]->getSceneEntity().getId() == id)
				scene->getSceneObjectsArray()[i]->updateTrasformations(QVector3D(-0.1, 0.0, 0.0));
}
