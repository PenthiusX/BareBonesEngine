#ifndef _ASSETLOADER_H
#define _ASSETLOADER_H
#include <vector>
#include <qstring.h>
//Assimp headers
#include <assimp/scene.h>;
#include <assimp/postprocess.h>;
#include <assimp/Importer.hpp>;

class _AssetLoader
{
public:
    _AssetLoader();
    ~_AssetLoader();

    std::vector<float> getAssetVertices();
    std::vector<unsigned int> getAssetIndices();
	bool modelLoader(QString pathToFile);

private:
std::vector<float> vertices;
std::vector<unsigned int> indices;
};

#endif // _ASSETLOADER_H
