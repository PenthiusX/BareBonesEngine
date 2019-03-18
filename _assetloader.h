#ifndef _ASSETLOADER_H
#define _ASSETLOADER_H
#include <vector>
#include <qstring.h>
//
//#include <Qt3DCore>
//#include <Qt3DRender>
//#include <Qt3DInput>
//#include <Qt3DLogic>
//#include <Qt3DExtras>
//#include <Qt3DAnimation>

class _AssetLoader
{
public:
    _AssetLoader();
    ~_AssetLoader();

    std::vector<float> getAssetVertices();
    std::vector<unsigned int> getAssetIndices();
	bool objLoader(QString pathToFile);

private:
std::vector<float> vertices;
std::vector<unsigned int> indices;
std::vector<float> uvs;


    //Qt3DRender::QMesh* qmesh;

};

#endif // _ASSETLOADER_H
