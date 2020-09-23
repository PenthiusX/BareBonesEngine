#ifndef _SCENE_H
#define _SCENE_H

#include "_renderer.h"
#include "_camera.h"
#include "_bufferobjects.h"
#include "_physics.h"
#include "_sceneentity.h"
#include "_light.h"
#include <vector>
#include <thread>         // std::thread

//Test skybox
#include "_skboxtest.h"

/*
 * Class: _Scene
 * This class define the scene manager , manages what needs to be rendered and what propertes need to be
 * set inside via a sceneentity object. essentially sets values in the scen entity object into the Renderer for drawing
 * Autor: Aditya Mattoo
*/
class _Scene
{
public:
    _Scene();
    ~_Scene();
    //
    std::vector<_Renderer*> getSceneObjects();
    void addSceneObject(_SceneEntity s);
    void removeSceneObject(uint index);
    void removeSceneObject(_SceneEntity s);
    //
    void addCamera(_Camera c);
    void updateCamera(_Camera c);
    void onResize(int w,int h);
    void render();
    void fixedUpdate(float intervalTime);
    //
    void setMousePositionInScene(QVector2D mousePos,Qt::MouseButton m);//Sets the mouse Positions into the scene object for use in the Physics and FBo
    //Helpers
    void addAllHelperTypesInScene();
    _SceneEntity findSceneEntity(uint iD);
    _SceneEntity findSceneEntity(QString tag);
    //Physics
    _SceneEntity getSceneEntityHitWithRay();

    enum shaderSelect{
        basic,
        lighting,
        shadowmapBuffer,
    };

private:
    //Inithandlers
    void initialiseMesh(_SceneEntity s);
    void initialisePhysics(_SceneEntity s);
    void initialiseLights(_SceneEntity s);
    //
    std::vector<_SceneEntity> sceneEntityVector;
    std::vector<_Renderer*> meshesR,lightsR;
    _FrameBuffer *fboObject;//framebuffer object in the current instance
    _StencilBuffer *stencilObject;
    _Renderer* r;
    _Camera cam;
    bool isCamera;
    QVector2D mousePositionR,mousePositionL;
    int resW,resH;
    //Helpers
    bool isHelpers;
    _SceneEntity sph,bb,s,mpnt,cnet,max,min,pivot,bg;
    uint pivotIndex,mPointerIndex;
    uint cIndex;
    uint mxIndex;
    uint minIndex;
    void updateHelpersOnce();
    void updateHelpersLoop(uint index);
    void setHelperIndexVars();
    //Physics
    std::vector<_Physics> physVector;
    _SceneEntity rayHitSceneEntity,triCollidedSceneEntity;
    uint hc;//physics and helper object counter
    uint loopIndex;
    void updateAllPhysicsObjectsLoop();
    void updateAllPhysicsObjectsOnce();
    //Lights
    std::vector<I_Light*> lightsArray;
    I_Light *lp;
    //Thread
    std::thread pu;
    std::thread ph;
    //Skybox
    TestSkyBox skyb;
};

#endif // _SCENE_H
