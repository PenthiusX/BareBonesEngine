#include "_assetloader.h"

_AssetLoader::_AssetLoader(){
}
_AssetLoader::~_AssetLoader(){
}
std::vector<float> _AssetLoader::getAssetVertices()
{
    return this->vertices;
}

std::vector<unsigned int> _AssetLoader::getAssetIndices()
{
	return std::vector<unsigned int>();
}

bool _AssetLoader::modelLoader(QString pathToFile)
{
	return false;
}
