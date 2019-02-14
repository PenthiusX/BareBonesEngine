#include "_glwidget.h"
#include <iostream>

_GLWidget::_GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}

void _GLWidget::initializeGL()
{
    renderers.push_back(_Renderer());
    renderers.push_back(_Renderer());

    renderers[0].setShader();
    renderers[1].setShader();

    std::vector<float> vertsV = {
        0.5,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };
    std::vector<int> indiceV = {// note that we start from 0!
                                0, 1, 3,   // first triangle
                                1, 2, 3    // second triangle
    };

    renderers[0].setBuffers(vertsV,indiceV);
    renderers[1].setBuffers(vertsV,indiceV);
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
