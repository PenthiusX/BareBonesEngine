#include "_glwidget.h"
#include <iostream>

_GLWidget::_GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}

/*
float vertices[] =
{
    0.5f,  0.5f, 0.0f,  // top right
    0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left
};
unsigned int indices[] = {  // note that we start from 0!
                            0, 1, 3,   // first triangle
                            1, 2, 3    // second triangle
                         };

float texCoords[] =
{
    0.0f, 0.0f,  // lower-left corner
    1.0f, 0.0f,  // lower-right corner
    0.5f, 1.0f   // top-center corner
};
*/

void _GLWidget::initializeGL()
{
    //---------------------------------

    initializeOpenGLFunctions();

    renderers.push_back(_Renderer());
    renderers.push_back(_Renderer());

    renderers[0].init(1.0f);
    renderers[1].init(0.5f);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void _GLWidget::resizeGL(int w, int h)
{

}

void _GLWidget::paintGL()//the renderloop
{
    renderers[0].draw();
    renderers[1].draw();
}
