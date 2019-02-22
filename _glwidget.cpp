#include "_glwidget.h"
#include <iostream>
/*
 * The _GLWidget Class:
 * Created: 5_02_2019
 * Author: Aditya,Saurabh
*/

/*Constructor*/
_GLWidget::_GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}
/*
* Function: initializeGL() overrides the 
* function initializeGL() in the OpopenglFunctions class
* runs once the opengl context is initialised.
*/
void _GLWidget::initializeGL()
{
    sceneObject.push_back(_Renderer());
    //sceneObject.push_back(_Renderer());
    sceneObject[0].setShader();//takes a default shader
   // sceneObject[1].setShader(":/shaders/vshader1.glsl",":/shaders/fshader1.glsl");//loads an explicitly defined shader

	/*Hard coded vertices*/
    std::vector<float> vertsV = 
	{
        0.5,  0.5f, 0.0f,	// top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f  // top left
    };
	/*Hard coded Indices*/
    std::vector<int> indiceV = {// note that we start from 0!
                                0, 1, 3,   // first triangle
                                1, 2, 3    // second triangle
                               };

    sceneObject[0].setBuffers(vertsV,indiceV);
	sceneObject[0].setMatrices(800,600);
    //vertsV[0] = 1.0f;
    //sceneObject[1].setBuffers(vertsV,indiceV);
}
/*
 * Function: resizeGL(int w, int h) overides the
 * function in OpopenglFunctions class.
 * pasees the current width and height 
 * of the layout via - int w and int h
*/
void _GLWidget::resizeGL(int w, int h)
{
	this->width = w;
	this->height = h;
	sceneObject[0].setMatrices(w,h);
}
/*
 * Function: paintGl() ovveriding thes
 * function in OpopenglFunctions
 * Your proprietory Draw function
 * this run in a loop till the application ends
*/
void _GLWidget::paintGL()//the renderloop
{
	sceneObject[0].draw();
	this->update();
    //sceneObject[1].draw();
}
