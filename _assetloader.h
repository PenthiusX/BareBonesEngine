#ifndef _ASSETLOADER_H
#define _ASSETLOADER_H
#include <vector>
#include <qstring.h>
#include <qvector3d.h>
#include <glm/glm.hpp>

#include "_modelinfo.h"
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
    void setModelInfo(_ModelInfo minfo);
    _ModelInfo getModelInfo();
    void objLoader(QString pathToFile);
    void loadAllModelsInfoFromFolder(QString folderName);//not in use
    _ModelInfo generateQuad();

private:
    _ModelInfo modelInfo;
    //
    std::vector<float> vertices;
    std::vector<uint> indices;
    std::vector<float> uvs;
    //
    glm::vec4 vertMin;
    glm::vec4 vertMax;
    int posCounter;
    int arrayCounter;
};

#endif // _ASSETLOADER_H
