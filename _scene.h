#ifndef _SCENE_H
#define _SCENE_H
#include "_sceneentity.h"
#include "_renderer.h"
#include "_camera.h"
#include "_framebuffer.h"

#include <vector>

/*
 * Class: _Scene
 * This class define the scene manager , manages what needs to be rendered and what propertes need to be
 * set inside via a sceneentity object. essentially sets values in the scen entity object into the Renderer for drawing
 * Autor: Aditya
 * Created:26_02_2019
*/
class _Scene
{
public:
    _Scene();
    ~_Scene();
    std::vector<_Renderer*> getSceneObjects();
    void addSceneObject(_SceneEntity s);
    void addCamera(_Camera c);
    void updateCamera(_Camera c);
    void onResize(int w,int h);
    void render();

    void setMousePositionInScene(QVector2D mousePos,std::string type);
    void updatePhysics(_Physics::PhysicsObjects type, glm::vec2 mousePos,glm::vec3 camPos,glm::vec2 screenRes,_SceneEntity s,unsigned int index);

private:
    std::vector<_SceneEntity> sceneEntityVector;
    std::vector<_Renderer*> renderObjects;
    _FrameBuffer *fboObject;//framebuffer object in the current instance
    _Renderer* r;
    _Camera cam;
    bool isCamera;
    QVector2D mousePositionR,mousePositionL;
    int resW,resH;
    //Physics
    _Physics phys;
};

#endif // _SCENE_H
