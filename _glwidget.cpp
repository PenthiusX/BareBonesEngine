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
	x = 0;
}
_GLWidget::~_GLWidget()
{
	delete sc;
}
/*
* Function: initializeGL() overrides the 
* function initializeGL() in the OpopenglFunctions class
* runs once the opengl context is initialised.
* Created: 5_02_2019
*/
void _GLWidget::initializeGL()
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//does not work
	///*Hard coded vertices*/
	std::vector<float> vertsV1 =
	{
		 0.5,   0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f  // top left
	};
	/*Hard coded Indices*/
	std::vector<unsigned int> indiceV1 = {// note that we start from 0!
								0, 1, 3,   // first triangle
								1, 2, 3    // second triangle
	};

/*Hard coded vertices*/
std::vector<float> vertsV = 
{
-0.500,-0.5000,  0.500,
-0.500,-0.5000, -0.500,
0.5000,-0.5000, -0.500,
0.5000,-0.5000, 0.5000,
-0.500,0.50000, 0.5000,
0.5000,0.50000, 0.5000,
0.5000,0.50000, -0.500,
-0.500,0.50000, -0.500
};

/*Hard coded Indices*/
std::vector<unsigned int> indiceV = 
{
	2, 4, 1,
	8, 6, 5,
	5, 2, 1,
	6, 3, 2,
	3, 8, 4,
	1, 8, 5,
	2, 3, 4,
	8, 7, 6,
	5, 6, 2,
	6, 7, 3,
	3, 7, 8,
	1, 4, 8
};

for (int i = 0; i < indiceV.size(); i++)
{
	indiceV[i] = indiceV[i] - 1;
}
	cam.setEyePosition(QVector3D(0.0, 0.0, -2.0));
	cam.setFocalPoint(QVector3D(0.0, 0.0, 0.0));

	s.setId(1);
	s.setShaderPath(":/shaders/vshader.glsl", ":/shaders/fshader.glsl");
	s.setPosition(QVector3D(0.0,2.0, 0.0));
	s.setScale(1);
	s.setModelData(vertsV, indiceV);

	s1.setId(2);
	s1.setShaderPath(":/shaders/vshader.glsl", ":/shaders/fshader.glsl");
	s1.setPosition(QVector3D(2.0, -2.0, 0.0));
	s1.setScale(1.0);
	s1.setModelData(vertsV1, indiceV1);

	sc = new _Scene();
	sc->addCamera(cam);
	sc->addSceneObject(s1);
	sc->addSceneObject(s1);

	/*
    sceneObject.push_back(_Renderer());
    sceneObject.push_back(_Renderer());
    sceneObject[0].setShader();//takes a default shader
    sceneObject[1].setShader();//loads an explicitly defined shader

    sceneObject[0].setBuffers(vertsV,indiceV);
    vertsV[0] = 1.0f;
    sceneObject[1].setBuffers(vertsV,indiceV);
	*/
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
	sc->onResize(w,h);
	//sceneObject[0].setMatrices(w,h);
	//sceneObject[1].setMatrices(w, h);
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
	//sceneObject[0].draw();
	//sceneObject[1].draw();
	sc->render();
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
*
*/
void _GLWidget::keyPressEvent(QKeyEvent * event)
{
	if (event->text() == "d" || event->text() == "D")
	{
		x += 1.0;
		//sc->getSceneObjects()[0].setModelMatrix(QVector3D(x,0.0,0.0),1.0,QQuaternion(QVector3D(0.0,0.0,0.0)));
	}
}
