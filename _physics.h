#ifndef _PHYSICS_H
#define _PHYSICS_H

#include <vector>
#include <QVector3D>
#include <QOpenGLExtraFunctions>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


typedef struct Phy_Triangle{
   QVector3D pointA;
   QVector3D pointB;
   QVector3D pointC;
}_Phy_Triangle;

typedef struct Phy_Sphere{
    QVector3D center;
    float radius;
}_Phy_Sphere;

class _Physics : protected QOpenGLExtraFunctions
{
public:
    _Physics();
    ~_Physics();

    std::vector<_Phy_Triangle> filltrianglesfromVerIndexArrays(std::vector<float>vert,std::vector<unsigned int> index);

private:
    std::vector<_Phy_Triangle> triVector;
    std::vector<_Phy_Triangle> sphVector;
};

#endif // _PHYSICS_H
