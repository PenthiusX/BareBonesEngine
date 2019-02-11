#ifndef _GLWIDGET_H
#define _GLWIDGET_H

#include <QOpenGLWidget>
//#include <QOpenGLFunctions>
//#include <qopenglextrafunctions.h>
//#include <vector>
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

private:
_Renderer r,x;
};

#endif // _GLWIDGET_H
