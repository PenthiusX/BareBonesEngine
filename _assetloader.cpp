#include "_assetloader.h"

_AssetLoader::_AssetLoader(){

}

_AssetLoader::~_AssetLoader(){
}

void _AssetLoader::setAssetVertices(std::vector<float> vert)
{
    this->vertices = vert;
}

std::vector<float> _AssetLoader::getAssetVertices()
{
    return this->vertices;
}

void _AssetLoader::setAssetIndices(std::vector<unsigned int> ind)
{
}

std::vector<unsigned int> _AssetLoader::getAssetIndices()
{
	return std::vector<unsigned int>();
}
