#ifndef _GLWIDGET_H
#define _GLWIDGET_H

#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QTimer>
#include "_scene.h"
#include "_physics.h"

/*
 * Class: _GLWidget
 * This class is the Controller is a typical MVC where the
 * View and Controller are being handeled by the QT GUI frameowrk and
 * its wrapper functions for creating Opengl and GUI windowing context.
 * Created: 5_02_2019
 * Author: Aditya,Saurabh
*/

class _GLWidget :  public QOpenGLWidget
{
public:
    _GLWidget();
    explicit _GLWidget(QWidget *parent = 0);
    ~_GLWidget();

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
    virtual void mousePressEvent(QMouseEvent *e) override;
    virtual void mouseReleaseEvent(QMouseEvent *e) override;
    virtual void mouseMoveEvent(QMouseEvent *e) override;
    virtual void wheelEvent(QWheelEvent *e) override;
    virtual void  keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;

    void applyStuffToallEntites(bool isit);

private:
    void addRandomSceneEntitestoScene();
    void removeSceneEntityFromScene();

    //Variables
    uint VBO;
    uint VAO;
    uint EBO;
    uint shaderProgram;
    QOpenGLContext *context;

    std::vector<_Scene*> sceneVector;//Vector of scene for drawing multiple scene Contexts//!!prototype implmentaition not implemented and checked!!!
    _Scene *scene;//Scene include renderer , camera and sceneentity classes, so no need to reinclude header

    _SceneEntity generated_model;//sau
    _SceneEntity s,*onPress;//custom Objects

    _AssetLoader assetLoader;
    _Camera cam;

    //Varible for InputCallbackFunctions
    double scroolScale;
    QVector2D rotRads;
    bool isCamFocus;

//    //Resolution variables
//    int width;
//    int height;
//    bool initialised=false;

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

};

#endif // _GLWIDGET_H
