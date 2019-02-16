#include "_glwidget.h"
#include <iostream>


_GLWidget::_GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}
/*
* initializeGL(int w, int h) overrides the 
* function 
*/
void _GLWidget::initializeGL()
{
    sceneObject.push_back(_Renderer());
    sceneObject.push_back(_Renderer());
    sceneObject[0].setShader();
    sceneObject[1].setShader(":/shaders/vshader1.glsl",":/shaders/fshader1.glsl");

    std::vector<float> vertsV = {
        0.5,  0.5f, 0.0f,	// top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f  // top left
       };
    std::vector<int> indiceV = {// note that we start from 0!
                                0, 1, 3,   // first triangle
                                1, 2, 3    // second triangle
                               };

    sceneObject[0].setBuffers(vertsV,indiceV);
    vertsV[0] = 1.0f;
    sceneObject[1].setBuffers(vertsV,indiceV);
}
/*
 * resizeGL(int w, int h) overides the
 * function in OpopenglFunctions class.
 * pasees the current width and height 
 * of the layout via - int w and int h
*/
void _GLWidget::resizeGL(int w, int h)
{

}
/*
 * paintGl function : ovveriding the
 * function in OpopenglFunctions
 * Your proprietory Draw function
 * this run in a loop till the
 * Application ends
*/
void _GLWidget::paintGL()//the renderloop
{
    sceneObject[0].draw();
    sceneObject[1].draw();
}
