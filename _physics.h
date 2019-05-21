#ifndef _PHYSICS_H
#define _PHYSICS_H

#include <vector>
#include <QOpenGLExtraFunctions>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


typedef struct Phy_Triangle{
    glm::vec3 pointA;
    glm::vec3 pointB;
    glm::vec3 pointC;
}_Phy_Triangle;

typedef struct Phy_Sphere{
    glm::vec3 center;
    float radius;
}_Phy_Sphere;

class _Physics : protected QOpenGLExtraFunctions
{
public:
    _Physics();
    ~_Physics();

    std::vector<_Phy_Triangle> generatetrianglesfromVerticesIndices(std::vector<float>vert,std::vector<unsigned int> index);
    glm::vec3 emitMousePointerRay(glm::vec2 mPressPos, glm::mat4x4 projectionmat, glm::mat4x4 viewmat);//returns the worldSpace ray cast from mousePosition,must be run in update
    bool hitSphere(const glm::vec3& center, float radius, glm::vec3 rayDir , glm::vec3 rayOrigin);
    void hitTriangle();
    void hitBox();

private:
    std::vector<_Phy_Triangle> triVector;
    std::vector<_Phy_Sphere> sphVector;
};

#endif // _PHYSICS_H
