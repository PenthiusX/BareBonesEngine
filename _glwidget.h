#ifndef _GLWIDGET_H
#define _GLWIDGET_H

#include <QOpenGLWidget>
#include <QMouseEvent>
#include "_scene.h"

class _GLWidget :  public QOpenGLWidget
{
public:
_GLWidget();
explicit _GLWidget(QWidget *parent = 0);
~_GLWidget();
bool isInitialised();
void setXRotation(int angle);
void setYRotation(int angle);
void setZRotation(int angle);
void rotateGeneratedmodel(float angle, glm::vec3 axis, bool with_stage);
public slots:
void update_background_image(char *img, unsigned int w, unsigned int h);
void showGeneratedModel(char *img, unsigned int w, unsigned int h);
void rotateGeneratedModel(float angle);
protected:
void initializeGL() Q_DECL_OVERRIDE;
void resizeGL(int w, int h)Q_DECL_OVERRIDE;
void paintGL()Q_DECL_OVERRIDE;
//
void mousePressEvent(QMouseEvent *e) override;
void mouseReleaseEvent(QMouseEvent *e) override;
virtual void  keyPressEvent(QKeyEvent *event) override;

void mouseMoveEvent(QMouseEvent *event);


QPoint m_lastPos;
int m_xRot;
int m_yRot;
int m_zRot;
private:
unsigned int VBO;
unsigned int VAO;
unsigned int EBO;
unsigned int shaderProgram;

//Resolution variables
int width;
int height;
bool initialised=false;

_Scene* scene;//Scene include renderer , camera and sceneentity classes, so no need to reinclude header
_SceneEntity s, s1,s2;
_SceneEntity generated_model;
_SceneEntity background_quad;
_AssetLoader assetLoader;
_Camera cam;
 QVector2D mousePressPosition;

 //Varible for keyboardCallbakc
 unsigned int id;
};

#endif // _GLWIDGET_H
