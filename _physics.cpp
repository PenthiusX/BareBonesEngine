/*
* Physics Class
*
* Note:
* Holds all the phhyscs based functional algorithms.
*/


#include "_physics.h"
#include <QDebug>
#include <algorithm>    // std::swap
#include "_tools.h"
#include <math.h>

//#include "_tools.h"

_Physics::_Physics(){
    //    sceneEntity = new _SceneEntity();
}
_Physics::~_Physics(){
    //    delete sceneEntity;
    delete r;
}

/*
   ▄▄ • ▄▄▄ .▄▄▄▄▄   .▄▄ · ▄▄▄ .▄▄▄▄▄
  ▐█ ▀ ▪▀▄.▀·•██     ▐█ ▀. ▀▄.▀·•██
  ▄█ ▀█▄▐▀▀▪▄ ▐█.▪   ▄▀▀▀█▄▐▀▀▪▄ ▐█.▪
  ▐█▄▪▐█▐█▄▄▌ ▐█▌· • ▐█▄▪▐█▐█▄▄▌ ▐█▌·
  ·▀▀▀▀  ▀▀▀  ▀▀▀  •  ▀▀▀▀  ▀▀▀  ▀▀▀
*/
void _Physics::setSceneEntity(_SceneEntity* s){
    this->sceneEntity = s;
}
_SceneEntity* _Physics::getSceneEntity(){
    return sceneEntity;
}
/*
 * returns the point of intersection varible value
 */
glm::vec3 _Physics::getRayTriIntersectionPoint() const{
    return outIntersectionPoint;
}
/*
*/
std::vector<_Phy_Triangle> _Physics::getPhysTries() const{
    return triVector;
}

/*
  ▪   ▐ ▄ ▪  ▄▄▄▄▄▪   ▄▄▄· ▄▄▌  ▪  ·▄▄▄▄•▄▄▄ .
  ██ •█▌▐███ •██  ██ ▐█ ▀█ ██•  ██ ▪▀·.█▌▀▄.▀·
  ▐█·▐█▐▐▌▐█· ▐█.▪▐█·▄█▀▀█ ██▪  ▐█·▄█▀▀▀•▐▀▀▪▄
  ▐█▌██▐█▌▐█▌ ▐█▌·▐█▌▐█ ▪▐▌▐█▌▐▌▐█▌█▌▪▄█▀▐█▄▄▌
  ▀▀▀▀▀ █▪▀▀▀ ▀▀▀ ▀▀▀ ▀  ▀ .▀▀▀ ▀▀▀·▀▀▀ • ▀▀▀
*/
void _Physics::initialiseSceneEntity(_SceneEntity* s){
    r = new _Renderer();//for visual helprs
    sceneEntity = s;
    //Initialise based on SceneEntity;
    if(sceneEntity->getPhysicsObjectType() == _SceneEntity::Sphere){
        sp.center = sceneEntity->getPostion();
        sp.radius = glm::distance(glm::vec3(sceneEntity->getModelInfo().getCentroid()), glm::vec3(sceneEntity->getModelInfo().getMaxExtent())) ;
        //used for maxextent update
        initialMax = sceneEntity->getModelInfo().getMaxExtent();
        initialMin = sceneEntity->getModelInfo().getMinExtent();
        createVisualHelper();
    }
    if(sceneEntity->getPhysicsObjectType() == _SceneEntity::Box){
        //used for maxextent update
        initialMax = sceneEntity->getModelInfo().getMaxExtent();
        initialMin = sceneEntity->getModelInfo().getMinExtent();
        createVisualHelper();
    }
    if(sceneEntity->getPhysicsObjectType() == _SceneEntity::Mesh){
        //inline if condition--
        sceneEntity->getModelInfo().getVertexArray().size() == 0 ?
                    genTriesforCollision(sceneEntity->getModelInfo().getVertexInfoArray(),sceneEntity->getModelInfo().getIndexArray()):
                    genTriesforCollision(sceneEntity->getModelInfo().getVertexArray(),sceneEntity->getModelInfo().getIndexArray());
        //---------------
        triVectorCopy = triVector;
        //used for maxextent update
        initialMax = sceneEntity->getModelInfo().getMaxExtent();
        initialMin = sceneEntity->getModelInfo().getMinExtent();
        createVisualHelper();
    }
}
/*
 *
*/
void _Physics::genTriesforCollision(std::vector<VertexInfo> vert, std::vector<unsigned int> index){
    std::vector< glm::vec4> pv;
    if(vert.size() > 0){
        for(uint i = 0 ; i < vert.size() ; i++){
            pv.push_back(glm::vec4(vert[i].Position.x,vert[i].Position.y,vert[i].Position.z,0.0));
        }

    }
    if(index.size() > 0){
        //Exception handeling: for improper models with extra
        //vertices that are not forming tries.
        //Improper models invoke an out of bounds error.
        uint setOfTries = index.size()/3;
        //
        for(uint i = 0 ; i < setOfTries * 3 ; i+= 3){
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
/*
 *
*/
void _Physics::genTriesforCollision(std::vector<float> vert, std::vector<unsigned int> index){
    // this point will not change on translation
    glm::vec4 vpoint;
    std::vector< glm::vec4> pv;
    if(vert.size() > 0){
        //Exception handeling: for improper models with extra
        //setof coordinate values that are not forming tries.
        //Improper models invoke an out of bounds error.
        unsigned int setOfVerts = vert.size()/3;
        for(int i = 0 ; i < setOfVerts * 3 ; i += 3){
            vpoint.x = vert[i];
            vpoint.y = vert[i+1];
            vpoint.z = vert[i+2];
            vpoint.w = 1.0f;
            pv.push_back(vpoint);
        }
    }
    if(index.size() > 0){
        //Exception handeling: for improper models with extra
        //vertices that are not forming tries.
        //Improper models invoke an out of bounds error.
        uint setOfTries = index.size()/3;
        //
        for(uint i = 0 ; i < setOfTries * 3 ; i+= 3){
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
/*
 * takes the mouse position and casts a ray in world space from it.
 */
void _Physics::setMousePointerRay(glm::vec2 mousePressPosition, glm::mat4x4 glm_projection4x4, glm::mat4x4 glm_view4x4, glm::vec2 res){
    resW = (int)res.x;
    resH = (int)res.y;

    // viewport coordinate system
    // normalized device coordinates
    auto x = (2.0f * mousePressPosition.x) / resW - 1.0f;
    auto y = 1.f - (2.0f * mousePressPosition.y) / resH;
    auto z = 1.f;
    glm::vec3 rayNormalizedDeviceCoordinates = glm::vec3(x, y, z);

    // 4D homogeneous clip coordinates
    rayClip = glm::vec4(rayNormalizedDeviceCoordinates.x, rayNormalizedDeviceCoordinates.y, -1.f, 1.f);
    // 4D eye (camera) coordinates
    rayEye = glm::inverse(glm_projection4x4) * rayClip;
    rayEye = glm::vec4(rayEye.x,rayEye.y, -1.0, 0.0);
    ray_wor = glm::inverse(glm_view4x4) * rayEye;
    // don't forget to normalise the vector at some point
    ray_wor = glm::normalize(ray_wor);
}
/*Not in use
*/
_Phy_Plane _Physics::constructPlaneFromPoints(glm::vec3 V0, glm::vec3 V1, glm::vec3 V2){
    glm::vec3 normal = glm::normalize(glm::cross(V1 - V0, V2 - V0));
    return constructPlaneFromPointNormal(V0, normal);
}
/* Not in use
*/
_Phy_Plane _Physics::constructPlaneFromPointVectors(glm::vec3 Pt, glm::vec3 V1, const glm::vec3 V2){
    glm::vec3 normal = glm::cross(V1, V2);
    return constructPlaneFromPointNormal(Pt, normal);
}
/*Not in use
*/
_Phy_Plane _Physics::constructPlaneFromPointNormal(glm::vec3 Pt, glm::vec3 normal){
    _Phy_Plane Result;
    glm::vec3  normalizedNormal = glm::normalize(normal);
    Result.a = normalizedNormal.x;
    Result.b = normalizedNormal.y;
    Result.c = normalizedNormal.z;
    Result.d = - glm::dot(Pt, normalizedNormal);
    return Result;
}
/*
  ▄▄▄▄▄▄▄▄ ..▄▄ · ▄▄▄▄▄.▄▄ ·
  •██  ▀▄.▀·▐█ ▀. •██  ▐█ ▀.
   ▐█.▪▐▀▀▪▄▄▀▀▀█▄ ▐█.▪▄▀▀▀█▄
   ▐█▌·▐█▄▄▌▐█▄▪▐█ ▐█▌·▐█▄▪▐█
   ▀▀▀  ▀▀▀  ▀▀▀▀  ▀▀▀  ▀▀▀▀
*/
/*
 *
 * returns yes or no on intersection
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
 * Returns distance from r0 to first intersecion with sphere,
 * or -1.0 if no intersection.
*/
float _Physics::raySphereIntersectDistance(glm::vec3 rayOrigin, glm::vec3 center, float radius){
    glm::vec3 rayDir = ray_wor;
    float a = dot(rayDir, rayDir);
    glm::vec3 s0_r0 = rayOrigin - center;
    float b = 2.0 * glm::dot(rayDir, s0_r0);
    float c = glm::dot(s0_r0, s0_r0) - (radius * radius);
    if (b*b - 4.0*a*c < 0.0){return -1.0;}
    return (-b - sqrt((b*b) - 4.0*a*c))/(2.0*a);
}
/*
 * 'Möller–Trumbore' ray triange intersection algorithm
 * checks for instrsection with the ray and traingle and returns a point
 * called in the Physics update for mesh
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
        return false;// This ray is parallel to this triangle.
    f = 1.0/a;
    s = rayOrigin - vertex0;
    u = f * (glm::dot(s,h));
    if (u < 0.0 || u > 1.0)return false;
    q = glm::cross(s,edge1);
    v = f * glm::dot(rayVector,q);
    if (v < 0.0 || u + v > 1.0) return false;
    // At this stage we can compute to find out where the intersection point is on the line.
    double t = f * glm::dot(edge2,q);
    if (t > EPSILON){outIntersectionPoint = rayOrigin + rayVector * (float)t;return true;}// ray intersection
    else {// This means that there is a line intersection but not a ray intersection.
        return false;}
}
/*
 * Checks for intersection with all traingles and returns tru or false
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
 *
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
/*
  ▄▄·       ▄▄▌  ▄▄▌  ▪  ·▄▄▄▄  ▄▄▄ .▄▄▄      ▄▄▄▄▄▄▄▄   ▄▄▄·  ▐ ▄ .▄▄ · ·▄▄▄      ▄▄▄  • ▌ ▄ ·.
 ▐█ ▌▪▪     ██•  ██•  ██ ██▪ ██ ▀▄.▀·▀▄ █·    •██  ▀▄ █·▐█ ▀█ •█▌▐█▐█ ▀. ▐▄▄·▪     ▀▄ █··██ ▐███▪
 ██ ▄▄ ▄█▀▄ ██▪  ██▪  ▐█·▐█· ▐█▌▐▀▀▪▄▐▀▀▄      ▐█.▪▐▀▀▄ ▄█▀▀█ ▐█▐▐▌▄▀▀▀█▄██▪  ▄█▀▄ ▐▀▀▄ ▐█ ▌▐▌▐█·
 ▐███▌▐█▌.▐▌▐█▌▐▌▐█▌▐▌▐█▌██. ██ ▐█▄▄▌▐█•█▌     ▐█▌·▐█•█▌▐█ ▪▐▌██▐█▌▐█▄▪▐███▌.▐█▌.▐▌▐█•█▌██ ██▌▐█▌
 ·▀▀▀  ▀█▄▀▪.▀▀▀ .▀▀▀ ▀▀▀▀▀▀▀▀•  ▀▀▀ .▀  ▀     ▀▀▀ .▀  ▀ ▀  ▀ ▀▀ █▪ ▀▀▀▀ ▀▀▀  ▀█▄▀▪.▀  ▀▀▀  █▪▀▀▀
*/
/*
 * tranforms the physics bodies in sync with the actual object it
 * is bound on.

*/
void _Physics::transFormPhysicsTriangles(glm::mat4x4 modelMatrix){
    for(unsigned int tr = 0 ; tr < triVector.size() ; tr++){
        triVector[tr].pointA =  modelMatrix * triVectorCopy[tr].pointA;
        triVector[tr].pointB =  modelMatrix * triVectorCopy[tr].pointB;
        triVector[tr].pointC =  modelMatrix * triVectorCopy[tr].pointC;
    }
}
/*
*/
void _Physics::transFormBoxExtents(glm::mat4x4 rotScaleMatrix){
    glm::vec4 max = glm::vec4(sceneEntity->getPostion().x,sceneEntity->getPostion().y,sceneEntity->getPostion().z,0.0f) + rotScaleMatrix * initialMax;
    glm::vec4 min = glm::vec4(sceneEntity->getPostion().x,sceneEntity->getPostion().y,sceneEntity->getPostion().z,0.0f) + rotScaleMatrix * initialMin;
    _ModelInfo m = sceneEntity->getModelInfo();
    m.setMaxExtents(max);
    m.setMinExtents(min);
    m.calcCentroidFromMinMax();
    sceneEntity->setModelInfo(m);
}
/*
*/
void _Physics::updateSphereExtents(){
    sp.center = sceneEntity->getPostion();
    sp.radius = glm::distance(glm::vec3(sceneEntity->getModelInfo().getCentroid()), glm::vec3(sceneEntity->getModelInfo().getMaxExtent())) * 0.7;
}

/*
  ▄• ▄▌ ▄▄▄· ·▄▄▄▄   ▄▄▄· ▄▄▄▄▄▄▄▄ .
  █▪██▌▐█ ▄█ ██▪ ██ ▐█ ▀█ •██  ▀▄.▀·
  █▌▐█▌ ██▀· ▐█· ▐█▌▄█▀▀█  ▐█.▪▐▀▀▪▄
  ▐█▄█▌▐█▪·• ██. ██ ▐█ ▪▐▌ ▐█▌·▐█▄▄▌
   ▀▀▀ .▀    ▀▀▀▀▀•  ▀  ▀  ▀▀▀  ▀▀▀
*/
/*
 * Update everything Internally goes in the _scene update loop
*/
void _Physics::updateMousePhysics(glm::vec2 mousePos, glm::vec3 camPos, glm::vec2 screenRes)
{
    setMousePointerRay(mousePos,sceneEntity->getProjectionMatrix(),sceneEntity->getViewMatrix(),screenRes);
    switch(sceneEntity->getPhysicsObjectType())
    {
    case _SceneEntity::Sphere :
        //updates the maxExtents
        transFormBoxExtents(sceneEntity->getRotationmatrix() * sceneEntity->getScaleingMatrix());
        //set sphere collider dimensions
        updateSphereExtents();
        //intersection check
        hitSphere(sp.center,sp.radius,camPos)?sceneEntity->setIsHitByRay(true):sceneEntity->setIsHitByRay(false);
        if(hitSphere(sp.center,sp.radius,camPos)){
            qDebug() <<"HitSphere"<<"Hit Id-"<<sceneEntity->getId()<<"Hit Ipos-"<<sceneEntity->getIndexPosInScene();
        }
        break;
    case _SceneEntity::Box :
        //updates the maxExtents
        transFormBoxExtents(sceneEntity->getRotationmatrix() * sceneEntity->getScaleingMatrix());
        //set box collider extents
        bx.max = sceneEntity->getModelInfo().getMaxExtent();
        bx.min = sceneEntity->getModelInfo().getMinExtent();
        //intersection check.
        if(hitBoundingBoxF(bx,camPos,ray_wor)){
            sceneEntity->setIsHitByRay(true);
            qDebug() <<"HitBox"<<"Hit Id-"<<sceneEntity->getId()<<"Hit Ipos-"<<sceneEntity->getIndexPosInScene();
        }
        if(!hitBoundingBoxF(bx,camPos,ray_wor)){sceneEntity->setIsHitByRay(false);}
        break;
    case _SceneEntity::Mesh :
        //updates the maxExtents
        transFormBoxExtents(sceneEntity->getRotationmatrix() * sceneEntity->getScaleingMatrix());
        //sets the updated modelMatrix from the sceneEntity->
        transFormPhysicsTriangles(sceneEntity->getModelMatrix());
        //Intersection check.
        if(rayIntersectsTriangles(triVector,camPos,ray_wor)){
            sceneEntity->setIsHitByRay(true);
            qDebug() <<"HitMesh"<<"Hit Id-"<<sceneEntity->getId()<<"Hit Ipos-"<<sceneEntity->getIndexPosInScene();
        }else if(!rayIntersectsTriangles(triVector,camPos,ray_wor)){
            sceneEntity->setIsHitByRay(false);}
        break;
    }
    updateVisualHelper();
}

/*
*/
bool _Physics::updateObjObjPhysics(std::vector<_Physics *> _physicsObjArray){
    //Scan this Physics object and its triangles with
    //the arry of all other physics object triangles
    //selected for collision in scene.
    for(uint i = 0 ; i < _physicsObjArray.size() ;i++){
        if(this->sceneEntity->getPhysicsObjectType() == _SceneEntity::Mesh &&
                _physicsObjArray[i]->getSceneEntity()->getPhysicsObjectType() == _SceneEntity::Mesh &&
                this->sceneEntity->getId() != _physicsObjArray[i]->getSceneEntity()->getId()){//make sure its not the same object
            //updates the maxExtents
            transFormBoxExtents(sceneEntity->getRotationmatrix() * sceneEntity->getScaleingMatrix());
            //sets the updated modelMatrix from the sceneEntity->
            transFormPhysicsTriangles(sceneEntity->getModelMatrix());
            return triangleTriangleIntersectionTest(*this,*_physicsObjArray[i]);
        }
        else if(this->sceneEntity->getPhysicsObjectType() == _SceneEntity::Sphere &&
                _physicsObjArray[i]->getSceneEntity()->getPhysicsObjectType() == _SceneEntity::Sphere &&
                this->sceneEntity->getId() != _physicsObjArray[i]->getSceneEntity()->getId()){
            //implement sphere on sphere here;
        }
    }
    updateVisualHelper();
}



/* Triangle/triangle intersection test routine,
 * by Tomas Moller, 1997.
 * See article "A Fast Triangle-Triangle Intersection Test",
 * Journal of Graphics Tools, 2(2), 1997
 *
 * Updated June 1999: removed the divisions -- a little faster now!
 * Updated October 1999: added {} to cross and sub macros
 *
 * int NoDivTriTriIsect(float V0[3],float V1[3],float V2[3],
 *                      float U0[3],float U1[3],float U2[3])
 *
 * parameters: vertices of triangle 1: V0,V1,V2
 *             vertices of triangle 2: U0,U1,U2
 * result    : returns 1 if the triangles intersect, otherwise 0
 *
 */

#define fabs(x)(float(fabs(x)))        /* implement as is fastest on your machine */

/* if USEEPSILONTEST is true then we do a check:
   if |dv|<EPSILON then dv=0.0;
   else no check is done (which is less robust)

*/
#define USEEPSILONTEST TRUE
#define EPSILON 0.000001
/*
 * c type macros for faster than glm computations
*/
#define cross(dest,v1,v2){                     \
    dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
    dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
    dest[2]=v1[0]*v2[1]-v1[1]*v2[0];\
}

#define dot(v1,v2)(v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])

#define sub(dest,v1,v2){         \
    dest[0]=v1[0]-v2[0]; \
    dest[1]=v1[1]-v2[1]; \
    dest[2]=v1[2]-v2[2]; \
}

/*
 * sort so that a<=b
*/
#define sort(a,b)       \
    if(a>b)    \
{          \
    float c; \
    c=a;     \
    a=b;     \
    b=c;     \
}


/*
 * this edge to edge test is based on Franlin Antonio's gem:
   "Faster Line Segment Intersection", in Graphics Gems III,
   pp. 199-202
*/
#define edgeEdgeTest(V0,U0,U1)                      \
    Bx=U0[i0]-U1[i0];                                   \
    By=U0[i1]-U1[i1];                                   \
    Cx=V0[i0]-U0[i0];                                   \
    Cy=V0[i1]-U0[i1];                                   \
    f=Ay*Bx-Ax*By;                                      \
    d=By*Cx-Bx*Cy;                                      \
    if((f>0 && d>=0 && d<=f) || (f<0 && d<=0 && d>=f))  \
    {                                                   \
        e=Ax*Cy-Ay*Cx;                                    \
        if(f>0)                                           \
        {                                                 \
            if(e>=0 && e<=f) return 1;                      \
        }                                                 \
        else                                              \
        {                                                 \
            if(e<=0 && e>=f) return 1;                      \
        }                                                 \
    }

#define edgeAgainstTriEdges(V0,V1,U0,U1,U2) \
{                                              \
    float Ax,Ay,Bx,By,Cx,Cy,e,d,f;               \
    Ax=V1[i0]-V0[i0];                            \
    Ay=V1[i1]-V0[i1];                            \
    /* test edge U0,U1 against V0,V1 */          \
    edgeEdgeTest(V0,U0,U1);                    \
    /* test edge U1,U2 against V0,V1 */          \
    edgeEdgeTest(V0,U1,U2);                    \
    /* test edge U2,U1 against V0,V1 */          \
    edgeEdgeTest(V0,U2,U0);                    \
}

#define pointInTri(V0,U0,U1,U2)           \
{                                           \
    float a,b,c,d0,d1,d2;                     \
    /* is T1 completly inside T2? */          \
    /* check if V0 is inside tri(U0,U1,U2) */ \
    a=U1[i1]-U0[i1];                          \
    b=-(U1[i0]-U0[i0]);                       \
    c=-a*U0[i0]-b*U0[i1];                     \
    d0=a*V0[i0]+b*V0[i1]+c;                   \
    \
    a=U2[i1]-U1[i1];                          \
    b=-(U2[i0]-U1[i0]);                       \
    c=-a*U1[i0]-b*U1[i1];                     \
    d1=a*V0[i0]+b*V0[i1]+c;                   \
    \
    a=U0[i1]-U2[i1];                          \
    b=-(U0[i0]-U2[i0]);                       \
    c=-a*U2[i0]-b*U2[i1];                     \
    d2=a*V0[i0]+b*V0[i1]+c;                   \
    if(d0*d1>0.0)                             \
    {                                         \
        if(d0*d2>0.0) return 1;                 \
    }                                         \
}
/*
 */
int coPlanarTriTri(float N[3],float V0[3],float V1[3],float V2[3],float U0[3],float U1[3],float U2[3])
{
    float A[3];
    short i0,i1;
    /* first project onto an axis-aligned plane, that maximizes the area */
    /* of the triangles, compute indices: i0,i1. */
    A[0]=fabs(N[0]);
    A[1]=fabs(N[1]);
    A[2]=fabs(N[2]);
    if(A[0]>A[1]){
        if(A[0]>A[2]){
            i0=1;      /* A[0] is greatest */
            i1=2;}
        else{
            i0=0;      /* A[2] is greatest */
            i1=1;}}
    else{ /* A[0]<=A[1] */
        if(A[2]>A[1]){
            i0=0;      /* A[2] is greatest */
            i1=1;}
        else{
            i0=0;      /* A[1] is greatest */
            i1=2;}}

    /* test all edges of triangle 1 against the edges of triangle 2 */
    edgeAgainstTriEdges(V0,V1,U0,U1,U2);
    edgeAgainstTriEdges(V1,V2,U0,U1,U2);
    edgeAgainstTriEdges(V2,V0,U0,U1,U2);

    /* finally, test if tri1 is totally contained in tri2 or vice versa */
    pointInTri(V0,U0,U1,U2);
    pointInTri(U0,V0,V1,V2);
    return 0;
}

/*
*/
#define newComputeIntervals(VV0,VV1,VV2,D0,D1,D2,D0D1,D0D2,A,B,C,X0,X1) \
{ \
    if(D0D1>0.0f) \
{ \
    /* here we know that D0D2<=0.0 */ \
    /* that is D0, D1 are on the same side, D2 on the other or on the plane */ \
    A=VV2; B=(VV0-VV2)*D2; C=(VV1-VV2)*D2; X0=D2-D0; X1=D2-D1; \
    } \
    else if(D0D2>0.0f)\
{ \
    /* here we know that d0d1<=0.0 */ \
    A=VV1; B=(VV0-VV1)*D1; C=(VV2-VV1)*D1; X0=D1-D0; X1=D1-D2; \
    } \
    else if(D1*D2>0.0f || D0!=0.0f) \
{ \
    /* here we know that d0d1<=0.0 or that D0!=0.0 */ \
    A=VV0; B=(VV1-VV0)*D0; C=(VV2-VV0)*D0; X0=D0-D1; X1=D0-D2; \
    } \
    else if(D1!=0.0f) \
{ \
    A=VV1; B=(VV0-VV1)*D1; C=(VV2-VV1)*D1; X0=D1-D0; X1=D1-D2; \
    } \
    else if(D2!=0.0f) \
{ \
    A=VV2; B=(VV0-VV2)*D2; C=(VV1-VV2)*D2; X0=D2-D0; X1=D2-D1; \
    } \
    else \
{ \
    /* triangles are coplanar */ \
    return coPlanarTriTri(N1,V0,V1,V2,U0,U1,U2); \
    } \
    }

//Most updated No division based intersection test by Moller
int NoDivTriTriIsect(float V0[3],float V1[3],float V2[3],float U0[3],float U1[3],float U2[3])
{
    float E1[3],E2[3];
    float N1[3],N2[3],d1,d2;
    float du0,du1,du2,dv0,dv1,dv2;
    float D[3];
    float isect1[2], isect2[2];
    float du0du1,du0du2,dv0dv1,dv0dv2;
    short index;
    float vp0,vp1,vp2;
    float up0,up1,up2;
    float bb,cc,max;

    /* compute plane equation of triangle(V0,V1,V2) */
    sub(E1,V1,V0);
    sub(E2,V2,V0);
    cross(N1,E1,E2);
    d1=-dot(N1,V0);
    /* plane equation 1: N1.X+d1=0 */

    /* put U0,U1,U2 into plane equation 1 to compute signed distances to the plane*/
    du0=dot(N1,U0)+d1;
    du1=dot(N1,U1)+d1;
    du2=dot(N1,U2)+d1;

    /* coplanarity robustness check */
#if USEEPSILONTEST==TRUE
    if(fabs(du0)<EPSILON) du0=0.0;
    if(fabs(du1)<EPSILON) du1=0.0;
    if(fabs(du2)<EPSILON) du2=0.0;
#endif
    du0du1=du0*du1;
    du0du2=du0*du2;

    if(du0du1>0.0f && du0du2>0.0f) /* same sign on all of them + not equal 0 ? */
        return 0;                    /* no intersection occurs */

    /* compute plane of triangle (U0,U1,U2) */
    sub(E1,U1,U0);
    sub(E2,U2,U0);
    cross(N2,E1,E2);
    d2=-dot(N2,U0);
    /* plane equation 2: N2.X+d2=0 */

    /* put V0,V1,V2 into plane equation 2 */
    dv0=dot(N2,V0)+d2;
    dv1=dot(N2,V1)+d2;
    dv2=dot(N2,V2)+d2;

#if USEEPSILONTEST==TRUE
    if(fabs(dv0)<EPSILON) dv0=0.0;
    if(fabs(dv1)<EPSILON) dv1=0.0;
    if(fabs(dv2)<EPSILON) dv2=0.0;
#endif

    dv0dv1=dv0*dv1;
    dv0dv2=dv0*dv2;

    if(dv0dv1>0.0f && dv0dv2>0.0f) /* same sign on all of them + not equal 0 ? */
        return 0;                    /* no intersection occurs */

    /* compute direction of intersection line */
    cross(D,N1,N2);

    /* compute and index to the largest component of D */
    max=(float)fabs(D[0]);
    index=0;
    bb=(float)fabs(D[1]);
    cc=(float)fabs(D[2]);
    if(bb>max) max=bb,index=1;
    if(cc>max) max=cc,index=2;

    /* this is the simplified projection onto L*/
    vp0=V0[index];
    vp1=V1[index];
    vp2=V2[index];

    up0=U0[index];
    up1=U1[index];
    up2=U2[index];

    /* compute interval for triangle 1 */
    float a,b,c,x0,x1;
    newComputeIntervals(vp0,vp1,vp2,dv0,dv1,dv2,dv0dv1,dv0dv2,a,b,c,x0,x1);

    /* compute interval for triangle 2 */
    float d,e,f,y0,y1;
    newComputeIntervals(up0,up1,up2,du0,du1,du2,du0du1,du0du2,d,e,f,y0,y1);

    float xx,yy,xxyy,tmp;
    xx=x0*x1;
    yy=y0*y1;
    xxyy=xx*yy;

    tmp=a*xxyy;
    isect1[0]=tmp+b*x1*yy;
    isect1[1]=tmp+c*x0*yy;

    tmp=d*xxyy;
    isect2[0]=tmp+e*xx*y1;
    isect2[1]=tmp+f*xx*y0;

    sort(isect1[0],isect1[1]);
    sort(isect2[0],isect2[1]);

    if(isect1[1]<isect2[0] || isect2[1]<isect1[0]) return 0;
    return 1;
}
/*
*/
bool _Physics::triangleTriangleIntersectionTest(_Physics objA, _Physics objB){
    for(uint i = 0; i < objA.getPhysTries().size(); i++)
    {
        for(uint j = 0; j < objB.getPhysTries().size(); j++)
        {
            _Phy_Triangle a = objA.getPhysTries()[i];_Phy_Triangle b = objB.getPhysTries()[j];
            float V0[3];V0[0] = a.pointA.x;V0[1] = a.pointA.y;V0[2] = a.pointA.z;
            float V1[3];V1[0] = a.pointB.x;V1[1] = a.pointB.y;V1[2] = a.pointB.z;
            float V2[3];V2[0] = a.pointC.x;V2[1] = a.pointC.y;V2[2] = a.pointC.z;
            float U0[3];U0[0] = b.pointA.x;U0[1] = b.pointA.y;U0[2] = b.pointA.z;
            float U1[3];U1[0] = b.pointB.x;U1[1] = b.pointB.y;U1[2] = b.pointB.z;
            float U2[3];U2[0] = b.pointC.x;U2[1] = b.pointC.y;U2[2] = b.pointC.z;

            int intr = NoDivTriTriIsect(V0,V1,V2,U0,U1,U2);
            if(intr == 1){
                return true;
            }
        }
    }
    return false;
}
/*
▌ ▐·▪  . ▄▄ · ▄• ▄▌ ▄▄▄· ▄▄▌  ▪  .▄▄ · ▄▄▄ .
▪█·█▌██ ▐█ ▀. █▪██▌▐█ ▀█ ██•  ██ ▐█ ▀. ▀▄.▀·
▐█▐█•▐█·▄▀▀▀█▄█▌▐█▌▄█▀▀█ ██▪  ▐█·▄▀▀▀█▄▐▀▀▪▄
 ███ ▐█▌▐█▄▪▐█▐█▄█▌▐█ ▪▐▌▐█▌▐▌▐█▌▐█▄▪▐█▐█▄▄▌
. ▀  ▀▀▀ ▀▀▀▀  ▀▀▀  ▀  ▀ .▀▀▀ ▀▀▀ ▀▀▀▀  ▀▀▀
*/
/*
 * this is local meshRenderObject to create a custom visual object
 * that acts as a visual rep of the physics object
*/
void _Physics::createVisualHelper()
{
    r->setShader(":/shaders/geoTestV.glsl",":/shaders/geoTestF.glsl",":/shaders/geoShaderExplode.glsl");//1

    if(sceneEntity->getPhysicsObjectType() == _SceneEntity::Mesh)
    {
        //Sets the matrices init info
        r->setModelMatrix(sceneEntity->getPostion(), sceneEntity->getScale(), sceneEntity->getRotation());
        std::vector<float> vertexArray;
        for(uint t = 0 ; t > triVector.size() ; t++){
            vertexArray.push_back(triVector[t].pointA.x);
            vertexArray.push_back(triVector[t].pointA.y);
            vertexArray.push_back(triVector[t].pointA.z);

            vertexArray.push_back(triVector[t].pointB.x);
            vertexArray.push_back(triVector[t].pointB.y);
            vertexArray.push_back(triVector[t].pointB.z);

            vertexArray.push_back(triVector[t].pointC.x);
            vertexArray.push_back(triVector[t].pointC.y);
            vertexArray.push_back(triVector[t].pointC.z);
        }
        r->setModelDataInBuffers(vertexArray);
    }
    if(sceneEntity->getPhysicsObjectType() == _SceneEntity::Box)
    {
        //Sets the matrices init info
        r->setModelMatrix(sceneEntity->getPostion(), sceneEntity->getScale(), sceneEntity->getRotation());
        r->setModelDataInBuffers(this->sceneEntity->getModelInfo().getVertexArray());
    }
    if(sceneEntity->getPhysicsObjectType() == _SceneEntity::Sphere)
    {
        //Sets the matrices init info
        r->setModelMatrix(sceneEntity->getPostion(), sceneEntity->getScale(), sceneEntity->getRotation());
        r->setModelDataInBuffers(this->sceneEntity->getModelInfo().getVertexArray());
    }
}

void _Physics::updateVisualHelper()
{
    r->setPosition(sceneEntity->getPostion());
    r->setRotation(sceneEntity->getRotation());
    r->setscale(sceneEntity->getScale());
}

void _Physics::drawVisualHelper()
{
    r->draw(1);
}
