#include "_physics.h"
#include <QDebug>

_Physics::_Physics(){}
_Physics::~_Physics(){}

void _Physics::setSceneEntity(_SceneEntity s)
{
    this->sceneEntity = s;
    //Initialise based on SceneEntity;
    if(this->sceneEntity.getPhysicsObjectType() == _SceneEntity::Sphere)
    {
        sp.center.x = sceneEntity.getPostion().x();sp.center.y = sceneEntity.getPostion().y();sp.center.z = sceneEntity.getPostion().z();
        sp.radius = sceneEntity.getScale();//temporary//will be replaced by max extents
    }
    else if(this->sceneEntity.getPhysicsObjectType() == _SceneEntity::Box)
    {
       initialMax = this->sceneEntity.getModelInfo().getMaxExtent();
       initialMin = this->sceneEntity.getModelInfo().getMinExtent();
    }
    else if(this->sceneEntity.getPhysicsObjectType() == _SceneEntity::Mesh)
    {
        genTriesforCollision(this->sceneEntity.getModelInfo().getVertices(),this->sceneEntity.getModelInfo().getIndices());
        triVectorCopy = triVector;
    }
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
    if(vert.size() > 0)
    {
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
    else {
        qInfo() << "cant generate triangles for collision ModelData incomplete";
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
/*
 * Function: raySphereIntersect()
 * returns yes or no on intersection
 * Created:30_05_2019
 */
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
glm::vec3 _Physics::getRayWorld() const
{
    return this->ray_wor;
}
glm::vec4 _Physics::getrayEye() const
{
    return this->rayEye;
}
glm::vec3 _Physics::getrayNormalizedDeviceCoordinates() const
{
    return this->rayNormalizedDeviceCoordinates;
}
glm::vec4 _Physics::getrayClip() const
{
    return this->rayClip;
}
/*
 * returns the point of intersection varible value
 */
glm::vec3 _Physics::getRayTriIntersectionPoint() const
{
    return this->outIntersectionPoint;
}
/*
 * Created:03.06.2019 Not in use
*/
Phy_Plane _Physics::constructPlaneFromPoints(glm::vec3 V0, glm::vec3 V1, glm::vec3 V2)
{
    glm::vec3 normal = glm::normalize(glm::cross(V1 - V0, V2 - V0));
    return constructPlaneFromPointNormal(V0, normal);
}
/*
 * Created:03.06.2019 Not in use
*/
Phy_Plane _Physics::constructPlaneFromPointVectors(glm::vec3 Pt, glm::vec3 V1, const glm::vec3 V2)
{
    glm::vec3 normal = glm::cross(V1, V2);
    return constructPlaneFromPointNormal(Pt, normal);
}
/*
 * Created:03.06.2019 Not in use
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
/*
 * Function: 'Möller–Trumbore' ray triange intersection algorithm
 * checks for instrsection with the ray and traingle and returns a point
 * called in the Physics update for mesh
 * Created:03.06.2019
*/
bool _Physics::rayIntersectsTriangle(glm::vec3 rayOrigin,
                                     glm::vec3 rayVector,
                                     _Phy_Triangle inTriangle,
                                     glm::vec3& outIntersectionPoint)
{
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
/* Function: transFormPhysicsTriangles(glm::mat4x4 modelMatrix)
 * tranforms the physics bodies in sync with the actual object it
 * is bound on.
 * Created: 5_06_2019
*/
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
 * Created: 12_06_2019
*/
void _Physics::transFormBoxExtents(glm::mat4x4 modelMatrix)
{
    glm::vec4 max = modelMatrix * initialMax;
    glm::vec4 min = modelMatrix * initialMin;
    this->sceneEntity.getModelInfo().setMaxExtents(max);
    this->sceneEntity.getModelInfo().setMinExtents(min);
}
/*
 * Created: 5_06_2019
*/
bool _Physics::hitBoundingBox(Phy_Box b , glm::vec3 rayOrigin ,glm::vec3 rDirection)
{
    double minB[NUMDIM], maxB[NUMDIM];		//box
    double origin[NUMDIM], dir[NUMDIM];		//ray
    double coord[NUMDIM];				    //hit point
    minB[0] = b.min.x; maxB[0] = b.max.x; origin[0] = rayOrigin.x; dir[0] = rDirection.x;
    minB[1] = b.min.y; maxB[1] = b.max.y; origin[1] = rayOrigin.y; dir[1] = rDirection.y;
    minB[2] = b.min.z; maxB[2] = b.max.z; origin[2] = rayOrigin.z; dir[2] = rDirection.z;

    char inside = true;
    char quadrant[NUMDIM];
    register int i;
    int whichPlane;
    double maxT[NUMDIM];
    double candidatePlane[NUMDIM];

    //  Find candidate planes; this loop can be avoided if
    //  rays cast all from the eye(assume perpsective view)
    for (i=0; i<NUMDIM; i++)
        if(origin[i] < minB[i]){
            quadrant[i] = LEFT;
            candidatePlane[i] = minB[i];
            inside = false;
        }else if (origin[i] > maxB[i]){
            quadrant[i] = RIGHT;
            candidatePlane[i] = maxB[i];
            inside = false;
        }else
            quadrant[i] = MIDDLE;

    //  Ray origin inside bounding box
    if(inside){
        coord[0] = origin[0];
        coord[1] = origin[1];
        coord[2] = origin[2];
        return (true);
    }

    //  Calculate T distances to candidate planes
    for (i = 0; i < NUMDIM; i++)
        if (quadrant[i] != MIDDLE && dir[i] !=0.)
            maxT[i] = (candidatePlane[i]-origin[i]) / dir[i];
        else
            maxT[i] = -1.;

    //  Get largest of the maxT's for final choice of intersection
    whichPlane = 0;
    for (i = 1; i < NUMDIM; i++)
        if (maxT[whichPlane] < maxT[i])
            whichPlane = i;

    //  Check final candidate actually inside box
    if (maxT[whichPlane] < 0.) return (false);
    for (i = 0; i < NUMDIM; i++)
        if (whichPlane != i){
            coord[i] = origin[i] + maxT[whichPlane] *dir[i];
            if (coord[i] < minB[i] || coord[i] > maxB[i])
                return (false);
        } else {
            coord[i] = candidatePlane[i];
            qDebug() << coord[i];
        }
    return (true);				//ray hits box
}
/*
 * Created: 5_06_2019
*/
//faster but does not return a point
bool intersection(Phy_Box box, glm::vec3 raydir, glm::vec3 rayorigin)
{
    double tmin = -INFINITY, tmax = INFINITY;
    //    for (int i = 0; i < 3; ++i) {
    //        if (ray.dir[i] != 0.0) {
    //            double t1 = (box.min[i] - r.origin[i])/r.dir[i];
    //            double t2 = (box.max[i] - r.origin[i])/r.dir[i];

    //            tmin = max(tmin, min(t1, t2));
    //            tmax = min(tmax, max(t1, t2));
    //        } else if (ray.origin[i] <= box.min[i] || ray.origin[i] >= box.max[i]) {
    //            return false;
    //        }
    //    }
    return tmax > tmin && tmax > 0.0;
}

/*
 * Update everything Internally goes in the _scene update loop
*/
void _Physics::updatePhysics(glm::vec2 mousePos, glm::vec3 camPos, glm::vec2 screenRes, _SceneEntity s)
{
    sceneEntity = s;
    setMousePointerRay(mousePos,s.getProjectionMatrix(),s.getViewMatrix(),screenRes);

    //Sphere Intersection Test
    if(this->sceneEntity.getPhysicsObjectType() == _SceneEntity::Sphere){
        sp.center.x = sceneEntity.getPostion().x();sp.center.y = sceneEntity.getPostion().y();sp.center.z = sceneEntity.getPostion().z();
        sp.radius = sceneEntity.getScale();//should be replaced by max extents or user input
        hitSphere(sp.center,sp.radius,camPos)?sceneEntity.setIsHitByRay(true):sceneEntity.setIsHitByRay(false);
    }
    //Box Intersection Test
    else if(this->sceneEntity.getPhysicsObjectType() == _SceneEntity::Box){
//        transFormBoxExtents(sceneEntity.getModelMatrix());
        bx.max = this->sceneEntity.getModelInfo().getMaxExtent();
        bx.min = this->sceneEntity.getModelInfo().getMinExtent();
        hitBoundingBox(bx,camPos,ray_wor)?this->sceneEntity.setIsHitByRay(true):this->sceneEntity.setIsHitByRay(false);
    }
    //Mesh Intersection Test
    else if(this->sceneEntity.getPhysicsObjectType() == _SceneEntity::Mesh){
        //sets the updated modelMatrix from the sceneEntity.
        transFormPhysicsTriangles(sceneEntity.getModelMatrix());
        for(int it= 0 ; it < triVector.size() ; it++){
            if(triVector.size() != NULL){
             rayIntersectsTriangle(camPos,ray_wor,triVector[it],outIntersectionPoint)?sceneEntity.setIsHitByRay(true):sceneEntity.setIsHitByRay(false);
            }
        }
    }
}
