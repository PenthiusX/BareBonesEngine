#include "_physics.h"
#include <QDebug>

_Physics::_Physics(){}
_Physics::~_Physics(){}

void _Physics::setSceneEntity(_SceneEntity s)
{
    this->sceneEntity = s;
    //Initialise based on SceneEntity;
    if(this->sceneEntity.getPhysicsObjectType() == _Physics::Sphere)
    {
        sp.center.x = sceneEntity.getPostion().x();sp.center.y = sceneEntity.getPostion().y();sp.center.z = sceneEntity.getPostion().z();
        sp.radius = sceneEntity.getScale();//temporary//will be replaced by max extents
    }
    else if(this->sceneEntity.getPhysicsObjectType() == _Physics::Box)
    {
        //pending
    }
    else if(this->sceneEntity.getPhysicsObjectType() == _Physics::Mesh)
    {
        genTriesforCollision(this->sceneEntity.getVertexData(),this->sceneEntity.getIndexData());
        triVectorCopy = triVector;
    }
    triItert = 0;
}

_SceneEntity _Physics::getSceneEntity()
{
    return this->sceneEntity;
}

void _Physics::genTriesforCollision(std::vector<float> vert, std::vector<unsigned int> index)
{
    // this point will not change on translation
    glm::vec4 vpoint;
    std::vector< glm::vec4> pv;
    for(int i = 0 ; i < vert.size() ; i += 3)
    {
        vpoint.x = vert[i];
        vpoint.y = vert[i+1];
        vpoint.z = vert[i+2];
        vpoint.w = 1.0f;
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
}

/* Function: setMousePointerRay()
 * takes the mouse position and casts a ray in world space from it.
 * Created:30_05_2019
 */
void _Physics::setMousePointerRay(glm::vec2 mousePressPosition, glm::mat4x4 glm_projection4x4, glm::mat4x4 glm_view4x4, glm::vec2 res)
{
    this->resW = (int)res.x;
    this->resH = (int)res.y;

    // viewport coordinate system
    // normalized device coordinates
    auto x = (2.0f * mousePressPosition.x) / resW - 1.0f;
    auto y = 1.f - (2.0f * mousePressPosition.y) / resH;
    auto z = 1.f;
    rayNormalizedDeviceCoordinates = glm::vec3(x, y, z);

    // 4D homogeneous clip coordinates
    rayClip = glm::vec4(rayNormalizedDeviceCoordinates.x, rayNormalizedDeviceCoordinates.y, -1.f, 1.f);
    // 4D eye (camera) coordinates
    rayEye = glm::inverse(glm_projection4x4) * rayClip;
    rayEye = glm::vec4(rayEye.x,rayEye.y, -1.0, 0.0);
    ray_wor = glm::inverse(glm_view4x4) * rayEye;
    // don't forget to normalise the vector at some point
    ray_wor = glm::normalize(ray_wor);
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

/*
 * Function: raySphereIntersect()
 * Returns distance from r0 to first intersecion with sphere,
 * or -1.0 if no intersection.
 * Created:30_05_2019
*/
float _Physics::raySphereIntersect(glm::vec3 rayOrigin, glm::vec3 center, float radius)
{
    glm::vec3 rayDir = this->ray_wor;
    float a = dot(rayDir, rayDir);
    glm::vec3 s0_r0 = rayOrigin - center;
    float b = 2.0 * glm::dot(rayDir, s0_r0);
    float c = glm::dot(s0_r0, s0_r0) - (radius * radius);
    if (b*b - 4.0*a*c < 0.0){
        return -1.0;}
    return (-b - sqrt((b*b) - 4.0*a*c))/(2.0*a);
}
/*
 * Returns the ray essentials
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
/*
 * Created:03.06.2019
*/
Phy_Plane _Physics::constructPlaneFromPoints(glm::vec3 V0, glm::vec3 V1, glm::vec3 V2)
{
    glm::vec3 normal = glm::normalize(glm::cross(V1 - V0, V2 - V0));
    return constructPlaneFromPointNormal(V0, normal);
}
/*
 * Created:03.06.2019
*/
Phy_Plane _Physics::constructPlaneFromPointVectors(glm::vec3 Pt, glm::vec3 V1, const glm::vec3 V2)
{
    glm::vec3 normal = glm::cross(V1, V2);
    return constructPlaneFromPointNormal(Pt, normal);
}
/*
 * Created:03.06.2019
*/
Phy_Plane _Physics::constructPlaneFromPointNormal(glm::vec3 Pt, glm::vec3 normal)
{
    Phy_Plane Result;
    glm::vec3 normalizedNormal = glm::normalize(normal);
    Result.a = normalizedNormal.x;
    Result.b = normalizedNormal.y;
    Result.c = normalizedNormal.z;
    Result.d = - glm::dot(Pt, normalizedNormal);
    return Result;
}

//

/*
 * Function: 'Möller–Trumbore' ray triange intersection algorithm
 * checks for instrsection with the ray and traingle and returns a point
 * Created:03.06.2019
*/
bool _Physics::rayIntersectsTriangle(glm::vec3 rayOrigin,
                                     glm::vec3 rayVector,
                                     _Phy_Triangle inTriangle,
                                     glm::vec3& outIntersectionPoint)
{
    const float EPSILON = 0.0000001;
    glm::vec3 vertex0 = inTriangle.pointA;
    glm::vec3 vertex1 = inTriangle.pointB;
    glm::vec3 vertex2 = inTriangle.pointC;
    glm::vec3 edge1, edge2, h, s, q;
    float a,f,u,v;
    edge1 = vertex1 - vertex0;
    edge2 = vertex2 - vertex0;
    h = glm::cross(rayVector,edge2);
    a = glm::dot(edge1,h);
    if (a > -EPSILON && a < EPSILON)
        return false;    // This ray is parallel to this triangle.
    f = 1.0/a;
    s = rayOrigin - vertex0;
    u = f * (glm::dot(s,h));
    if (u < 0.0 || u > 1.0)
        return false;
    q = glm::cross(s,edge1);
    v = f * glm::dot(rayVector,q);
    if (v < 0.0 || u + v > 1.0)
        return false;
    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = f * glm::dot(edge2,q);
    if (t > EPSILON) // ray intersection
    {outIntersectionPoint = rayOrigin + rayVector * t;
        return true;}
    else {// This means that there is a line intersection but not a ray intersection.
        return false;}
}

void _Physics::transFormPhysicsTriangles(glm::mat4x4 modelMatrix)
{
    for(unsigned int tr = 0 ; tr < triVector.size() ; tr++)
    {
        triVector[tr].pointA =  modelMatrix * triVectorCopy[tr].pointA;
        triVector[tr].pointB =  modelMatrix * triVectorCopy[tr].pointB;
        triVector[tr].pointC =  modelMatrix * triVectorCopy[tr].pointC;
    }
}

/*
 * Update everything Internally goes in the _scene update loop
*/
void _Physics::updatePhysics(glm::vec2 mousePos, glm::vec3 camPos, glm::vec2 screenRes, _SceneEntity s)
{
    sceneEntity = s;
    setMousePointerRay(mousePos,s.getProjectionMatrix(),s.getViewMatrix(),screenRes);

    if(this->sceneEntity.getIsPhysicsObject() && this->sceneEntity.getPhysicsObjectType() == _Physics::Sphere){
        sp.center.x = sceneEntity.getPostion().x();sp.center.y = sceneEntity.getPostion().y();sp.center.z = sceneEntity.getPostion().z();
        sp.radius = sceneEntity.getScale();//should be replaced by max extents or user input
        if(hitSphere(sp.center,sp.radius,camPos)){
            sceneEntity.setIsHitByRay(true);
            sceneEntity.setColor(QVector4D(0.6,0.0,0.0,0.8));}
        else{
            sceneEntity.setIsHitByRay(false);
            sceneEntity.setColor(QVector4D(1.0,0.6,0.0,0.5));}}

    else if(this->sceneEntity.getIsPhysicsObject() && this->sceneEntity.getPhysicsObjectType() == _Physics::Box)
    {

    }
    else if(this->sceneEntity.getIsPhysicsObject() && this->sceneEntity.getPhysicsObjectType() == _Physics::Mesh){
        //sets the updated modelMatrix from the sceneEntity.
        transFormPhysicsTriangles(sceneEntity.getModelMatrix());
        //temporary optimisation : iterates every frame instead of iterating noOfElements*Everyframe.
        //need to do An AABB optimisation
        if(triItert > triVector.size()-1){
            triItert = 0;}
        if(triVector.size() > 0)
        { glm::vec3 outIntersectionPoint;
            if(rayIntersectsTriangle(camPos,ray_wor,triVector[triItert],outIntersectionPoint)) {
                qDebug() <<"outIntersectionPoint -"<< outIntersectionPoint.x << outIntersectionPoint.y << outIntersectionPoint.z;
                sceneEntity.setIsHitByRay(true);
                sceneEntity.setColor(QVector4D(0.0,1.0,0.0,0.8));
            }
            else if(!rayIntersectsTriangle(camPos,ray_wor,triVector[triItert],outIntersectionPoint)){
                sceneEntity.setIsHitByRay(false);
                sceneEntity.setColor(QVector4D(1.0,0.0,1.0,0.5));
            }
            triItert++;
        }
    }
}
