#ifndef _ASSETLOADER_H
#define _ASSETLOADER_H
#include <vector>
#include <qstring.h>
#include <qvector3d.h>
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

    typedef struct Model_Info{
        QString name;
        QString path;
        std::vector<float> vertexArray;
        std::vector<unsigned int> indexAarray;
    }_Model_Info;

    //Not optimal
    std::vector<float> getAssetVertices();
    std::vector<unsigned int> getAssetIndices();

    std::vector<_AssetLoader::_Model_Info> getModelInfoArray();
    void objLoader(QString pathToFile);
    void loadAllModelsInfoFromFolder(QString folderName);

private:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<float> uvs;

    std::vector<_AssetLoader::_Model_Info> modelInfoArray;
    _AssetLoader::_Model_Info minfo;

    QVector3D vertMin;
    QVector3D vertMax;
};

#endif // _ASSETLOADER_H
