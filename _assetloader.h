#ifndef _ASSETLOADER_H
#define _ASSETLOADER_H
#include <vector>


class _AssetLoader
{
public:
    _AssetLoader();
    ~_AssetLoader();
	//
    void setAssetVertices(std::vector<float> vert);
    std::vector<float> getAssetVertices();
	//
    void setAssetIndices(std::vector<unsigned int> ind);
    std::vector<unsigned int> getAssetIndices();

private:
std::vector<float> vertices;
std::vector<unsigned int> indices;

};

#endif // _ASSETLOADER_H
