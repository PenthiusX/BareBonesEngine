#include "_physics.h"
#include <QDebug>
#include <algorithm>    // std::swap

//#include "_tools.h"

_Physics::_Physics(){
}
_Physics::~_Physics(){}

void _Physics::setSceneEntity(_SceneEntity s){
    sceneEntity = s;
    //Initialise based on SceneEntity;
    if(sceneEntity.getPhysicsObjectType() == _SceneEntity::Sphere){
        sp.center = glm::vec3(sceneEntity.getPostion().x(),sceneEntity.getPostion().y(),sceneEntity.getPostion().z());
        sp.radius = glm::distance(glm::vec3(sceneEntity.getModelInfo().getCentroid()), glm::vec3(sceneEntity.getModelInfo().getMaxExtent())) ;
        //used for maxextent update
        initialMax = sceneEntity.getModelInfo().getMaxExtent();
        initialMin = sceneEntity.getModelInfo().getMinExtent();
    }
    else if(sceneEntity.getPhysicsObjectType() == _SceneEntity::Box){
        //used for maxextent update
        initialMax = sceneEntity.getModelInfo().getMaxExtent();
        initialMin = sceneEntity.getModelInfo().getMinExtent();
    }
    else if(sceneEntity.getPhysicsObjectType() == _SceneEntity::Mesh){
        genTriesforCollision(sceneEntity.getModelInfo().getVerticexArray(),sceneEntity.getModelInfo().getIndexArray());
        triVectorCopy = triVector;
        //used for maxextent update
        initialMax = sceneEntity.getModelInfo().getMaxExtent();
        initialMin = sceneEntity.getModelInfo().getMinExtent();
    }
}
/*
 *
*/
_SceneEntity _Physics::getSceneEntity(){
    return sceneEntity;
}
/*
 *
*/
void _Physics::genTriesforCollision(std::vector<float> vert, std::vector<unsigned int> index){
    // this point will not change on translation
    glm::vec4 vpoint;
    std::vector< glm::vec4> pv;
    if(vert.size() > 0){
        for(int i = 0 ; i < vert.size() ; i += 3){
            vpoint.x = vert[i];
            vpoint.y = vert[i+1];
            vpoint.z = vert[i+2];
            vpoint.w = 1.0f;
            pv.push_back(vpoint);
        }
    }
    if(index.size() > 0){
        //Exception handeling: Forces improper models with extra
        //vertices that are not forming tries to invoke and out of
        //bounds error in this collection algorith.
        unsigned int setOfTries = index.size()/3;
        for(int i = 0 ; i < setOfTries * 3 ; i += 3){
            _Phy_Triangle tri;
            tri.pointA = pv[index[i]];
            tri.pointB = pv[index[i+1]];
            tri.pointC = pv[index[i+2]];
            triVector.push_back(tri);
        }
    }
    else{
        qInfo() << "cant generate triangles for collision ModelData incomplete";
    }
}
/* Function: setMousePointerRay()
 * takes the mouse position and casts a ray in world space from it.
 * Created:30_05_2019
 */
void _Physics::setMousePointerRay(glm::vec2 mousePressPosition, glm::mat4x4 glm_projection4x4, glm::mat4x4 glm_view4x4, glm::vec2 res){
    resW = (int)res.x;
    resH = (int)res.y;

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
/*
 * Function: raySphereIntersect()
 * returns yes or no on intersection
 * Created:30_05_2019
 */
bool _Physics::hitSphere(glm::vec3 center, float radius , glm::vec3 rayOrigin){
    glm::vec3 rayDir = ray_wor;
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
float _Physics::raySphereIntersect(glm::vec3 rayOrigin, glm::vec3 center, float radius){
    glm::vec3 rayDir = ray_wor;
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
glm::vec3 _Physics::getRayWorld() const{
    return ray_wor;
}
glm::vec4 _Physics::getrayEye() const{
    return rayEye;
}
glm::vec3 _Physics::getrayNormalizedDeviceCoordinates() const{
    return rayNormalizedDeviceCoordinates;
}
glm::vec4 _Physics::getrayClip() const{
    return rayClip;
}
/*
 * returns the point of intersection varible value
 */
glm::vec3 _Physics::getRayTriIntersectionPoint() const{
    return outIntersectionPoint;
}
/*
 * Created:03.06.2019 Not in use
*/
Phy_Plane _Physics::constructPlaneFromPoints(glm::vec3 V0, glm::vec3 V1, glm::vec3 V2){
    glm::vec3 normal = glm::normalize(glm::cross(V1 - V0, V2 - V0));
    return constructPlaneFromPointNormal(V0, normal);
}
/*
 * Created:03.06.2019 Not in use
*/
Phy_Plane _Physics::constructPlaneFromPointVectors(glm::vec3 Pt, glm::vec3 V1, const glm::vec3 V2){
    glm::vec3 normal = glm::cross(V1, V2);
    return constructPlaneFromPointNormal(Pt, normal);
}
/*
 * Created:03.06.2019 Not in use
*/
Phy_Plane _Physics::constructPlaneFromPointNormal(glm::vec3 Pt, glm::vec3 normal){
    Phy_Plane Result;
    glm::vec3 normalizedNormal = glm::normalize(normal);
    Result.a = normalizedNormal.x;
    Result.b = normalizedNormal.y;
    Result.c = normalizedNormal.z;
    Result.d = - glm::dot(Pt, normalizedNormal);
    return Result;
}
/*
 * Function: 'Möller–Trumbore' ray triange intersection algorithm
 * checks for instrsection with the ray and traingle and returns a point
 * called in the Physics update for mesh
 * Created:03.06.2019
*/
bool _Physics::rayIntersectsTriangle(glm::vec3 rayOrigin,
                                     glm::vec3 rayVector,
                                     _Phy_Triangle inTriangle,
                                     glm::vec3& outIntersectionPoint){
    const double EPSILON = 0.0000001;
    glm::vec3 vertex0 = inTriangle.pointA;
    glm::vec3 vertex1 = inTriangle.pointB;
    glm::vec3 vertex2 = inTriangle.pointC;
    glm::vec3 edge1, edge2, h, s, q;
    double a,f,u,v;
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
    double t = f * glm::dot(edge2,q);
    if (t > EPSILON) // ray intersection
    {outIntersectionPoint = rayOrigin + rayVector * (float)t;
        return true;}
    else {// This means that there is a line intersection but not a ray intersection.
        return false;}
}
/*
 * Created: 15_06_2019
 */
bool _Physics::rayIntersectsTriangles(std::vector<_Phy_Triangle> tries,glm::vec3 rayOrigin,glm::vec3 rayVector){
    if(tries.size() != NULL){
        for(int it= 0 ; it < tries.size() ; it++){
            if(rayIntersectsTriangle(rayOrigin,rayVector,triVector[it],outIntersectionPoint)){
                return true;
            }
        }
    }
    return false;
}
/*
 * Created: 5_06_2019
*/
bool hitBoundingBoxF(_Phy_Box b,glm::vec3 orig, glm::vec3 r){
    float tmin = (b.min.x - orig.x) / r.x;
    float tmax = (b.max.x - orig.x) / r.x;

    if(tmin > tmax) std::swap(tmin, tmax);

    float tymin = (b.min.y - orig.y) / r.y;
    float tymax = (b.max.y - orig.y) / r.y;

    if(tymin > tymax) std::swap(tymin, tymax);
    if((tmin > tymax) || (tymin > tmax))
        return false;

    if(tymin > tmin)
        tmin = tymin;
    if(tymax < tmax)
        tmax = tymax;

    float tzmin = (b.min.z - orig.z) / r.z;
    float tzmax = (b.max.z - orig.z) / r.z;

    if(tzmin > tzmax) std::swap(tzmin, tzmax);
    if((tmin > tzmax) || (tzmin > tmax))
        return false;

    if(tzmin > tmin)
        tmin = tzmin;
    if(tzmax < tmax)
        tmax = tzmax;
    return true;
}

/* Function: transFormPhysicsTriangles(glm::mat4x4 modelMatrix)
 * tranforms the physics bodies in sync with the actual object it
 * is bound on.
 * Created: 5_06_2019
*/
void _Physics::transFormPhysicsTriangles(glm::mat4x4 modelMatrix){
    for(unsigned int tr = 0 ; tr < triVector.size() ; tr++){
        triVector[tr].pointA =  modelMatrix * triVectorCopy[tr].pointA;
        triVector[tr].pointB =  modelMatrix * triVectorCopy[tr].pointB;
        triVector[tr].pointC =  modelMatrix * triVectorCopy[tr].pointC;
    }
}
/*
 * Created: 12_06_2019
*/
void _Physics::transFormBoxExtents(glm::mat4x4 rotScaleMatrix){
    glm::vec4 max = glm::vec4(sceneEntity.getPostion().x(),sceneEntity.getPostion().y(),sceneEntity.getPostion().z(),0.0f) + rotScaleMatrix * initialMax;
    glm::vec4 min = glm::vec4(sceneEntity.getPostion().x(),sceneEntity.getPostion().y(),sceneEntity.getPostion().z(),0.0f) + rotScaleMatrix * initialMin;
    _ModelInfo m = sceneEntity.getModelInfo();
    m.setMaxExtents(max);
    m.setMinExtents(min);
    m.calcCentroidFromMinMax();
    sceneEntity.setModelInfo(m);
}
/*
 * Update everything Internally goes in the _scene update loop
*/
void _Physics::updatePhysics(glm::vec2 mousePos, glm::vec3 camPos, glm::vec2 screenRes, _SceneEntity se){
    sceneEntity = se;
    setMousePointerRay(mousePos,sceneEntity.getProjectionMatrix(),sceneEntity.getViewMatrix(),screenRes);
    //Sphere Intersection Test
    if(sceneEntity.getPhysicsObjectType() == _SceneEntity::Sphere){
        //updates the maxExtents
        //In this case just to keep the extent helpers Updated
        transFormBoxExtents(sceneEntity.getRotationmatrix() * sceneEntity.getScaleingMatrix());
        //set sphere collider dimensions
        sp.center = glm::vec3(sceneEntity.getPostion().x(),sceneEntity.getPostion().y(),sceneEntity.getPostion().z());
        sp.radius = glm::distance(glm::vec3(sceneEntity.getModelInfo().getCentroid()), glm::vec3(sceneEntity.getModelInfo().getMaxExtent()));
        //intersection check
        hitSphere(sp.center,sp.radius,camPos)?sceneEntity.setIsHitByRay(true):sceneEntity.setIsHitByRay(false);
        if(hitSphere(sp.center,sp.radius,camPos))qDebug() <<"HitSphere"<<"Hit Id-"<<sceneEntity.getId();
    }
    //Box Intersection Test
    else if(sceneEntity.getPhysicsObjectType() == _SceneEntity::Box){
        //updates the maxExtents
        transFormBoxExtents(sceneEntity.getRotationmatrix() * sceneEntity.getScaleingMatrix());
        //set box collider extents
        bx.max = sceneEntity.getModelInfo().getMaxExtent();
        bx.min = sceneEntity.getModelInfo().getMinExtent();
        //intersection check.
        if(hitBoundingBoxF(bx,camPos,ray_wor)){
            sceneEntity.setIsHitByRay(true);
            qDebug() <<"HitBox"<<"Hit Id-"<<sceneEntity.getId();
        }
        else if(!hitBoundingBoxF(bx,camPos,ray_wor))
            sceneEntity.setIsHitByRay(false);
    }
    //Mesh Intersection Test
    else if(sceneEntity.getPhysicsObjectType() == _SceneEntity::Mesh){
        //updates the maxExtents
        transFormBoxExtents(sceneEntity.getRotationmatrix() * sceneEntity.getScaleingMatrix());
        //sets the updated modelMatrix from the sceneEntity.
        transFormPhysicsTriangles(sceneEntity.getModelMatrix());
        //Intersection check.
        if(rayIntersectsTriangles(triVector,camPos,ray_wor)){
            sceneEntity.setIsHitByRay(true);
            qDebug() <<"HitMesh"<<"Hit Id-"<<sceneEntity.getId();
        }else if(!rayIntersectsTriangles(triVector,camPos,ray_wor))
            sceneEntity.setIsHitByRay(false);
    }
}
