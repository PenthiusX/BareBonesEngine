#include "_physics.h"

_Physics::_Physics() : QOpenGLExtraFunctions(QOpenGLContext::currentContext()){}
_Physics::~_Physics(){}

std::vector<_Phy_Triangle> _Physics::generatetrianglesfromVerticesIndices(std::vector<float> vert, std::vector<unsigned int> index)
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

void _Physics::emitMousePointerRay(glm::vec2 mousePressPosition, glm::mat4x4 glm_projection4x4, glm::mat4x4 glm_view4x4)
{
    // Where The Viewport Values Will Be Stored
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);           // Retrieves The Viewport Values (X, Y, Width, Height)

    // qDebug() << mousePressPosition;
    // viewport coordinate system
    // normalized device coordinates
    auto x = (2.0f * mousePressPosition.x) / viewport[2] - 1.0f;
    auto y = 1.f - (2.0f * mousePressPosition.y) / viewport[3];
    auto z = 1.f;
    auto rayNormalizedDeviceCoordinates = glm::vec3(x, y, z);

    // 4D homogeneous clip coordinates
    auto rayClip = glm::vec4(rayNormalizedDeviceCoordinates.x, rayNormalizedDeviceCoordinates.y, -1.f, 1.f);

    // 4D eye (camera) coordinates
    glm::vec4 rayEye = glm::inverse(glm_projection4x4) * rayClip;
    rayEye = glm::vec4(rayEye.x,rayEye.y, -1.0, 0.0);

    glm::vec3 ray_wor = glm::inverse(glm_view4x4) * rayEye;
    // don't forget to normalise the vector at some point
    return ray_wor = glm::normalize(ray_wor);
}


bool _Physics::hitSphere(const glm::vec3& center, float radius, glm::vec3 rayDir , glm::vec3 rayOrigin)
{
    glm::vec3 oc = rayOrigin - center;
    float a = glm::dot(rayDir,rayDir);
    float b = 2.0 * glm::dot(oc, rayDir);
    float c = dot(oc,oc) - radius*radius;
    float discriminant = b*b - 4*a*c;
    return (discriminant>0);
}
