#ifndef _PHYSICS_H
#define _PHYSICS_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


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
}_Phy_Triangle;

typedef struct Phy_Sphere{
    glm::vec3 center;
    float radius;
}_Phy_Sphere;

class _Physics
{
public:
    _Physics();
    ~_Physics();

    std::vector<_Phy_Triangle> generatetrianglesfromVerticesIndices(std::vector<float>vert,std::vector<unsigned int> index);
    void setMousePointerRay(glm::vec2 mPressPos, glm::mat4x4 projectionmat, glm::mat4x4 viewmat, glm::vec2 res);//returns the worldSpace ray cast from mousePosition,must be run in update
    bool hitSphere(glm::vec3 center, float radius , glm::vec3 rayOrigin);
    float raySphereIntersect(glm::vec3 rayOrigin, glm::vec3 s0, float sr);
    //
    glm::vec3 getRayWorld();
    glm::vec4 getrayEye();
    glm::vec3 getrayNormalizedDeviceCoordinates();
    glm::vec4 getrayClip();

    void hitTriangle();
    void hitBox();

    enum PhysicsObjects {
        Sphere = 0,
        Box = 1,
        Mesh = 2,
    };

private:
    std::vector<_Phy_Triangle> triVector;
    std::vector<_Phy_Sphere> sphVector;
    //
    Phy_Sphere sp;
    Phy_Triangle tri;
    //
    glm::vec4 rayEye;
    glm::vec3 ray_wor;
    glm::vec3 rayNormalizedDeviceCoordinates;
    glm::vec4 rayClip;
    //
    int resW,resH;
};

#endif // _PHYSICS_H
