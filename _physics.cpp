#include "_physics.h"
#include <QDebug>

_Physics::_Physics(){}
_Physics::~_Physics(){}

std::vector<_Phy_Triangle> _Physics::genTriesforCollision(std::vector<float> vert, std::vector<unsigned int> index)
{
    // this point will not change on translation
    glm::vec3 vpoint;
    std::vector< glm::vec3> pv;

    for(int i = 0 ; i < vert.size() ; i += 3)
    {
        vpoint.x = vert[i];
        vpoint.y = vert[i+1];
        vpoint.z = vert[i+2];
        pv.push_back(vpoint);
    }
    for(int i = 0 ; i < index.size() ; i += 3)
    {
        _Phy_Triangle tri;
        tri.pointA = pv[index[i]];
        tri.pointB = pv[index[i+1]];
        tri.pointC = pv[index[i+2]];
        triVector.push_back(tri);
    }
    return triVector;
}

void _Physics::setMousePointerRay(glm::vec2 mousePressPosition, glm::mat4x4 glm_projection4x4, glm::mat4x4 glm_view4x4, glm::vec2 res)
{
    this->resW = (int)res.x;
    this->resH = (int)res.y;

//     qDebug() <<"mpos-"<< mousePressPosition.x << "." << mousePressPosition.y;
    // viewport coordinate system
    // normalized device coordinates
    auto x = (2.0f * mousePressPosition.x) / resW - 1.0f;
    auto y = 1.f - (2.0f * mousePressPosition.y) / resH;
    auto z = 1.f;
    rayNormalizedDeviceCoordinates = glm::vec3(x, y, z);

    // 4D homogeneous clip coordinates
    rayClip = glm::vec4(rayNormalizedDeviceCoordinates.x, rayNormalizedDeviceCoordinates.y, -1.f, 1.f);
//    qDebug() <<"rayClip-"<< rayClip.x<<"." <<rayClip.y << "." << rayClip.z;

    // 4D eye (camera) coordinates
    rayEye = glm::inverse(glm_projection4x4) * rayClip;
    rayEye = glm::vec4(rayEye.x,rayEye.y, -1.0, 0.0);
//   qDebug() <<"rayEye-"<< rayEye.x<<"." <<rayEye.y << "." << rayEye.z;

    ray_wor = glm::inverse(glm_view4x4) * rayEye;
//   qDebug() <<"rayW-"<< ray_wor.x<<"." <<ray_wor.y << "." << ray_wor.z;

    // don't forget to normalise the vector at some point
    ray_wor = glm::normalize(ray_wor);
/*   qDebug() <<"rayWN-"<< ray_wor.x<<"." <<ray_wor.y << "." << ray_wor.z;
     qDebug() <<"---------------------------------------------------------"*/;
}

// returns yes or no on intersection
bool _Physics::hitSphere(glm::vec3 center, float radius , glm::vec3 rayOrigin)
{
    glm::vec3 rayDir = this->ray_wor;

    glm::vec3 oc = rayOrigin - center;
    float a = glm::dot(rayDir,rayDir);
    float b = 2.0 * glm::dot(oc, rayDir);
    float c = dot(oc,oc) - radius*radius;
    float discriminant = b*b - 4*a*c;
    return (discriminant>0);
}

// rayOrigin: cam position
// rayDir: normalized ray direction
// s0: sphere center
// sr: sphere radius
// Returns distance from r0 to first intersecion with sphere,
// or -1.0 if no intersection.
float _Physics::raySphereIntersect(glm::vec3 rayOrigin, glm::vec3 center, float radius)
{
    glm::vec3 rayDir = this->ray_wor;
    float a = dot(rayDir, rayDir);
    glm::vec3 s0_r0 = rayOrigin - center;
    float b = 2.0 * glm::dot(rayDir, s0_r0);
    float c = glm::dot(s0_r0, s0_r0) - (radius * radius);
    if (b*b - 4.0*a*c < 0.0){
        return -1.0;
    }
    return (-b - sqrt((b*b) - 4.0*a*c))/(2.0*a);
}

/*
 * Returns the ray essentials
 *
 */
glm::vec3 _Physics::getRayWorld()
{
    return this->ray_wor;
}
glm::vec4 _Physics::getrayEye()
{
    return this->rayEye;
}
glm::vec3 _Physics::getrayNormalizedDeviceCoordinates()
{
    return this->rayNormalizedDeviceCoordinates;
}
glm::vec4 _Physics::getrayClip()
{
    return this->rayClip;
}
