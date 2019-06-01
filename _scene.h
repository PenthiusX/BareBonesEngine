#ifndef _SCENE_H
#define _SCENE_H
#include "_sceneentity.h"
#include "_renderer.h"
#include "_camera.h"
#include "_framebuffer.h"
#include "_assetloader.h"
#include <vector>

/*
 * Class: _Scene
 * This class define the scene manager , manages what needs to be rendered and what propertes need to be
 * set inside via a sceneentity object. essentially sets values in the scen entity object into the Renderer for drawing
 * Autor: Aditya Mattoo
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
    void setMousePositionInScene(QVector2D mousePos,Qt::MouseButton m);//Sets the mouse Positions into the scene object for use in the Physics and FBo class
    void updatePhysics(glm::vec2 mousePos,glm::vec3 camPos,glm::vec2 screenRes,_SceneEntity s,unsigned int index);//updates the Physcis
    void updateMouseRay(glm::vec2 mousePos,glm::vec2 screenRes,_SceneEntity s);
    void upDateRayCollisonTest(glm::vec3 camPos,_SceneEntity s,unsigned int index);
    glm::vec3 pointerObject;//debug helper implentation

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
