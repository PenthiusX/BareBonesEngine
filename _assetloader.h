#ifndef _ASSETLOADER_H
#define _ASSETLOADER_H
#include <vector>
#include <qstring.h>
/*
 * Class: _AssetLoader
 * class sets values needed by the viewMatrix
 * for setting of values on Init or realtime
 * Created: 15_03_2019
 * Author: Aditya
*/

class _AssetLoader
{
public:
    _AssetLoader();
    ~_AssetLoader();

    std::vector<float> getAssetVertices();
    std::vector<unsigned int> getAssetIndices();
    void objLoader(QString pathToFile);

private:
std::vector<float> vertices;
std::vector<unsigned int> indices;
std::vector<float> uvs;
};

#endif // _ASSETLOADER_H
