#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "_sceneentity.h"
#include "_renderer.h"
#include "_camera.h"

/*
 * Class: _Physics
 * Creates an individual instace for the setting and
 * binding of the framebufferObjects
*/

typedef struct Phy_Triangle{
    glm::vec4 pointA;
    glm::vec4 pointB;
    glm::vec4 pointC;
    glm::vec4 normal;
}_Phy_Triangle;

typedef struct Phy_Sphere{
    glm::vec3 center;
    float radius;
}_Phy_Sphere;

typedef struct Phy_Plane{
    glm::vec3 normal;  // Plane normal. Points x on the plane satisfy Dot(n,x) = d
    float a,b,c,d; //the (a, b, c, d) in a*x + b*y + c*z + d = 0.
}_Phy_Plane;

typedef struct Phy_Box{
    glm::vec4 max;//max extent
    glm::vec4 min;//min extent
    glm::vec4 centroid;//center between two extents;
}_Phy_Box;

class _Physics
{
public:
    _Physics();
    ~_Physics();
    //
    void initialiseSceneEntity(_SceneEntity* s,_Camera* c);
    void setSceneEntity(_SceneEntity* s);
    _SceneEntity* getSceneEntity();
    //
    void genTriesforCollision(std::vector<float>vert,std::vector<uint> index);
    void genTriesforCollision(std::vector<VertexInfo> vi,std::vector<uint> index);
    void genNormalsForTries(std::vector<_Phy_Triangle> triV);
    std::vector<_Phy_Triangle> getPhysTries()const;
    //
    void setMousePointerRay(glm::vec2 mPressPos, glm::mat4x4 projectionmat, glm::mat4x4 viewmat, glm::vec2 res);//returns the worldSpace ray cast from mousePosition,must be run in update
    bool hitSphere(glm::vec3 center, float radius , glm::vec3 rayOrigin);
    float raySphereIntersectDistance(glm::vec3 rayOrigin, glm::vec3 s0, float sr);
    bool hitBoundingBox(Phy_Box b, glm::vec3 rayOrigin, glm::vec3 rDirection);
    //
    _Phy_Plane constructPlaneFromPoints(glm::vec3 V0, glm::vec3 V1,glm::vec3 V2);
    _Phy_Plane constructPlaneFromPointVectors(glm::vec3 Pt, glm::vec3 V1, const glm::vec3 V2);
    _Phy_Plane constructPlaneFromPointNormal(glm::vec3 Pt, glm::vec3 Normal);
    //
    glm::vec3 getRayTriIntersectionPoint() const;
    //
    void updateMousePhysics(glm::vec2 mousePos,glm::vec2 screenRes);
    bool updateObjObjPhysics(std::vector<_Physics*> _physicsObjArray);

    void drawVisualHelper();

private:
    _Camera* locPCam;
    //
    std::vector<_Phy_Triangle> triVector,triVectorCopy;
    //
    _Phy_Sphere sp;
    _Phy_Triangle tri;
    _Phy_Plane pl;
    _Phy_Box bx;
    //
    glm::vec4 rayEye;
    glm::vec3 ray_wor;
    glm::vec4 rayClip;
    //
    int resW,resH;
    //
    _SceneEntity* sceneEntity;
    glm::vec3 outIntersectionPoint;
    //
    glm::vec4 initialMax,initialMin;//Store the initial extent positions before transformations.
    //
    //Functions:
    bool rayIntersectsTriangle(glm::vec3 rayOrigin,glm::vec3 rayVector,_Phy_Triangle inTriangle,glm::vec3& outIntersectionPoint);
    bool rayIntersectsTriangles(std::vector<_Phy_Triangle> tries, glm::vec3 rayOrigin, glm::vec3 rayVector);
    void transFormPhysicsTriangles(glm::mat4x4 modelMatrix);//updates tranformation related values to the Physics Triangles
    void transFormBoxExtents(glm::mat4x4 rotScaleMatrix);
    void updateSphereExtents();

    bool triangleTriangleIntersectionTest(_Physics objA, _Physics objB);


    //Objects for Visualhelper creation
    _Renderer *r;

    void createVisualHelper();
    void updateVisualHelper();
};
