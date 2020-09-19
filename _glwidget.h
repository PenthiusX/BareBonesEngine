#pragma once

#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QTimer>
#include "_scene.h"
#include "_material.h"
/*
 * Class: _GLWidget
 * This class is the Controller is a typical MVC where the
 * View and Controller are being handeled by the QT GUI frameowrk and
 * its wrapper functions for creating Opengl and GUI windowing context.
 * Author: Aditya
*/

class _GLWidget :  public QOpenGLWidget
{
public:
    _GLWidget();
    explicit _GLWidget(QWidget *parent = 0);
    ~_GLWidget();

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h)Q_DECL_OVERRIDE;
    void paintGL()Q_DECL_OVERRIDE;
    //
    virtual void mousePressEvent(QMouseEvent *e) override;
    virtual void mouseReleaseEvent(QMouseEvent *e) override;
    virtual void mouseMoveEvent(QMouseEvent *e) override;
    virtual void wheelEvent(QWheelEvent *e) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;

private:
    void addRandomSceneEntitestoScene(uint count);
    void removeSceneEntityFromScene();

    //Variables
    uint VBO;
    uint VAO;
    uint EBO;
    uint shaderProgram;
    QOpenGLContext *context;

    std::vector<_Scene*> sceneVector;//Vector of scene for drawing multiple scene Contexts//!!prototype implmentaition not implemented and checked!!!
    _Scene *scene;//Scene include renderer , camera and sceneentity classes, so no need to reinclude header
    _SceneEntity s,s2,*onPress;//custom Objects
    _SceneEntity dl1,p1,p2,d1,sl1;
    _AssetLoader assetLoader;
    _Camera cam;

    //Varible for InputCallbackFunctions
    double scroolScale;
    QVector2D rotRads;
    bool isCamFocus;
    bool isCTRL;
    QVector2D mousePressPositionL,mousePressPositionR,mousePressPositionM;
    QPoint globalMPoint;
    QVector2D mousePositionL,mousePositionR,mousePositionM;
    QVector3D oldPosForCam,newPosForCam;
    //
    QElapsedTimer qTimer;
    float deltaTime;
    float currentTime;
    float timeSinceLastFrame;
    int ic = 0;//interval counter
    //
    _Material m,m2;
};
