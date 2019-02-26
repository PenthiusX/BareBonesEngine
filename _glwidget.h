#ifndef _GLWIDGET_H
#define _GLWIDGET_H

#include <QOpenGLWidget>
#include "_renderer.h"


class _GLWidget :  public QOpenGLWidget
{
public:
_GLWidget();
explicit _GLWidget(QWidget *parent = 0);
protected:
void initializeGL() Q_DECL_OVERRIDE;
void resizeGL(int w, int h)Q_DECL_OVERRIDE;
void paintGL()Q_DECL_OVERRIDE;
//
void mousePressEvent(QMouseEvent *e) override;
void mouseReleaseEvent(QMouseEvent *e) override;

private:
unsigned int VBO;
unsigned int VAO;
unsigned int EBO;
unsigned int shaderProgram;

//Resolution variables
int width;
int height;

std::vector<_Renderer> sceneObject;

};

#endif // _GLWIDGET_H
