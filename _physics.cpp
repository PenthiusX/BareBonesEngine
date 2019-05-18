#include "_physics.h"

_Physics::_Physics() : QOpenGLExtraFunctions(QOpenGLContext::currentContext()){}
_Physics::~_Physics(){}

std::vector<_Phy_Triangle> _Physics::filltrianglesfromVerIndexArrays(std::vector<float> vert, std::vector<unsigned int> index)
{
      QVector3D vpoint;
      std::vector<QVector3D> pv;

    for(int i = 0 ; i < vert.size() ; i += 3)
    {
        vpoint.setX(vert[i]);
        vpoint.setY(vert[i+1]);
        vpoint.setZ(vert[i+2]);
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
