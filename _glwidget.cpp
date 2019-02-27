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
}
/*
* Function: initializeGL() overrides the 
* function initializeGL() in the OpopenglFunctions class
* runs once the opengl context is initialised.
* Created: 5_02_2019
*/
void _GLWidget::initializeGL()
{
	/*Hard coded vertices*/
	std::vector<float> vertsV =
	{
		0.5,  0.5f, 0.0f,	// top right
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f  // top left
	};
	/*Hard coded Indices*/
	std::vector<unsigned int> indiceV = {// note that we start from 0!
								0, 1, 3,   // first triangle
								1, 2, 3    // second triangle
	};

	s = new _SceneEntity();
    s->setShaderPath(":/shaders/vshader.glsl", ":/shaders/fshader.glsl");
    s->setPosition(QVector3D(1.0, 0.0, 0.0));
    s->setModelData(vertsV,indiceV);

    sceneObject.push_back(_Renderer());
    sceneObject.push_back(_Renderer());
    sceneObject[0].setShader();//takes a default shader
    sceneObject[1].setShader();//loads an explicitly defined shader

    sceneObject[0].setBuffers(vertsV,indiceV);
    vertsV[0] = 1.0f;
    sceneObject[1].setBuffers(vertsV,indiceV);
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
	sceneObject[0].setMatrices(w,h);
	sceneObject[1].setMatrices(w, h);
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
	sceneObject[0].draw();
	//sceneObject[1].draw();
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
	std::cout << e << std::endl;
}
/*
* Function: mouseReleaseEvent(QMouseEvent *e)
* This is a overriden function from the QWidget parent
* runs each time the mouse is released.
* Created: 5_02_2019
*/
void _GLWidget::mouseReleaseEvent(QMouseEvent *e)
{
	std::cout << e << std::endl;
}
