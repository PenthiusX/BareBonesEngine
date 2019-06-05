#ifndef _PHYSICS_H
#define _PHYSICS_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "_sceneentity.h"


/*
 * Class: _Physics
 * Creates an individual instace for the setting and
 * binding of the framebufferObjects
 * Created: 30_04_2019
 * Author: Aditya
*/

typedef struct Phy_Triangle{
    glm::vec3 pointA;
    glm::vec3 pointB;
    glm::vec3 pointC;
    glm::vec3 normal;
}_Phy_Triangle;

typedef struct Phy_Sphere{
    glm::vec3 center;
    float radius;
}_Phy_Sphere;

typedef struct Phy_Plane{
    glm::vec3 normal;  // Plane normal. Points x on the plane satisfy Dot(n,x) = d
    float a,b,c,d; //the (a, b, c, d) in a*x + b*y + c*z + d = 0.
}_Phy_Plane;

class _Physics
{
public:
    _Physics();
    ~_Physics();

    void setSceneEntity(_SceneEntity s);
    _SceneEntity getSceneEntity();
    //
    void genTriesforCollision(std::vector<float>vert,std::vector<unsigned int> index);
    void genNormalsForTries( std::vector<_Phy_Triangle> triV);
    //
    void setMousePointerRay(glm::vec2 mPressPos, glm::mat4x4 projectionmat, glm::mat4x4 viewmat, glm::vec2 res);//returns the worldSpace ray cast from mousePosition,must be run in update
    bool hitSphere(glm::vec3 center, float radius , glm::vec3 rayOrigin);
    float raySphereIntersect(glm::vec3 rayOrigin, glm::vec3 s0, float sr);

    glm::vec3 barycentricPointA(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 p);
    inline float triArea2D(float x1, float y1, float x2, float y2, float x3, float y3);
    glm::vec3 barycentricPointB(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 p, float &u, float &v, float &w);
    //
    Phy_Plane constructPlaneFromPoints(glm::vec3 V0, glm::vec3 V1,glm::vec3 V2);
    Phy_Plane constructPlaneFromPointVectors(glm::vec3 Pt, glm::vec3 V1, const glm::vec3 V2);
    Phy_Plane constructPlaneFromPointNormal(glm::vec3 Pt, glm::vec3 Normal);
    //
    bool rayIntersectsTriangle(glm::vec3 rayOrigin,glm::vec3 rayVector,_Phy_Triangle inTriangle,glm::vec3& outIntersectionPoint);
    //
    glm::vec3 getRayWorld();
    glm::vec4 getrayEye();
    glm::vec3 getrayNormalizedDeviceCoordinates();
    glm::vec4 getrayClip();
    //
    void hitTriangle();
    void hitBox();
    //
    void updatePhysics(glm::vec2 mousePos,glm::vec3 camPos,glm::vec2 screenRes,_SceneEntity s);

    enum PhysicsObjects{
        Sphere = 0,
        Box = 1,
        Mesh = 2,
    };

private:
    std::vector<_Phy_Triangle> triVector;
    //
    Phy_Sphere sp;
    Phy_Triangle tri;
    Phy_Plane pl;
    //
    glm::vec4 rayEye;
    glm::vec3 ray_wor;
    glm::vec3 rayNormalizedDeviceCoordinates;
    glm::vec4 rayClip;
    //
    int resW,resH;
    //
    _SceneEntity sceneEntity;
    //
    unsigned int triItert;
};

#endif // _PHYSICS_H
