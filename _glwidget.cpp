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
	delete scene;
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
	std::vector<unsigned int> indiceV1 = 
	{// note that we start from 0!
		0, 1, 3,   // first triangle  
		1, 2, 3    // second triangle
	};

	std::vector<float> vertsV2 =
	{
		1.0,   1.0f, 0.0f,  // top right
		1.0f, -1.0f, 0.0f, // bottom right
	   -0.5f, -1.0f, 0.0f, // bottom left
	   -1.0f,  1.0f, 0.0f  // top left
	};
	/*Hard coded Indices*/
	std::vector<unsigned int> indiceV2 =
	{// note that we start from 0!
		0, 1, 3,   // first triangle  
		1, 2, 3    // second triangle
	};

/*Hard coded vertices*/
std::vector<float> vertsV = 
{
 0.500000,-0.500000,-0.500000,
 0.500000,-0.500000, 0.500000,
-0.500000,-0.500000, 0.500000,
-0.500000,-0.500000,-0.500000,
 0.500000, 0.500000,-0.500000,
 0.500000, 0.500000, 0.500000,
-0.500000, 0.500000, 0.500000,
-0.500000, 0.500000,-0.500000
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
	cam.setEyePosition(QVector3D(0.0, 0.0, -7.0));
	cam.setFocalPoint(QVector3D(0.0, 0.0, 0.0));

	s.setId(0);
	s.setShaderPath(":/shaders/vshader1.glsl", ":/shaders/fshader1.glsl");
	s.setPosition(QVector3D(1.0,-0.3f, -3.0));
	s.setRotation(QQuaternion(90,0.0,0.1,0.0));
	s.setScale(0.5);
	s.setModelData(vertsV, indiceV);

	s1.setId(1);
	s1.setShaderPath(":/shaders/vshader.glsl", ":/shaders/fshader.glsl");
	s1.setPosition(QVector3D(0.0, 0.7, 4.0));
	s1.setRotation(QQuaternion(QVector3D(0.0, 0.0, 0.0)));
	s1.setScale(1.5);
	s1.setModelData(vertsV, indiceV);

	scene = new _Scene();
	scene->addCamera(cam);

	scene->addSceneObject(s);
	scene->addSceneObject(s1);
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
	scene->onResize(w,h);
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
int id = 0;
void _GLWidget::keyPressEvent(QKeyEvent * event)
{
	if (event->text() == "q" || event->text() == "Q")
	{
		id += 1;
		if (id > 1)
		{
			id = 0;
		}
	}

	if (event->text() == "d" || event->text() == "D")
	{
		//sc->getSceneObjects()[0]->setModelMatrix(QVector3D(+0.0,+0.01,+0.0), 1.0,QQuaternion(QVector3D(0.0,0.0,0.0)));
		for (int i = 0; i < scene->getRenderObjects().size(); i++)
		{
			if (scene->getRenderObjects()[i]->getSceneEntity().getId() == id)
			{
				scene->getRenderObjects()[i]->updateTrasformations(QVector3D(0.0,0.1,0.0));
			}
		}
	}

	if (event->text() == "a" || event->text() == "A")
	{
		//sc->getSceneObjects()[0]->setModelMatrix(QVector3D(+0.0,+0.01,+0.0), 1.0,QQuaternion(QVector3D(0.0,0.0,0.0)));
		for (int i = 0; i < scene->getRenderObjects().size(); i++)
		{
			if (scene->getRenderObjects()[i]->getSceneEntity().getId() == id)
			{
				scene->getRenderObjects()[i]->updateTrasformations(QVector3D(0.0, -0.1, 0.0));
			}
		}
	}

	if (event->text() == "w" || event->text() == "W")
	{
		//sc->getSceneObjects()[0]->setModelMatrix(QVector3D(+0.0,+0.01,+0.0), 1.0,QQuaternion(QVector3D(0.0,0.0,0.0)));
		for (int i = 0; i < scene->getRenderObjects().size(); i++)
		{
			if (scene->getRenderObjects()[i]->getSceneEntity().getId() == id)
			{
				scene->getRenderObjects()[i]->updateTrasformations(QVector3D(0.1, 0.0, 0.0));
			}
		}
	}

	if (event->text() == "s" || event->text() == "S")
	{
		//sc->getSceneObjects()[0]->setModelMatrix(QVector3D(+0.0,+0.01,+0.0), 1.0,QQuaternion(QVector3D(0.0,0.0,0.0)));
		for (int i = 0; i < scene->getRenderObjects().size(); i++)
		{
			if (scene->getRenderObjects()[i]->getSceneEntity().getId() == id)
			{
				scene->getRenderObjects()[i]->updateTrasformations(QVector3D(-0.1, 0.0, 0.0));
			}
		}
	}
}
