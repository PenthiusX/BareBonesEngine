#ifndef _ASSETLOADER_H
#define _ASSETLOADER_H
#include <vector>
#include <qstring.h>
#include <qvector3d.h>

#include <glm/glm.hpp>
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

    //----
    typedef struct Model_Info{//Stores the data of the model loded into scene as a SceneEntity.


    public:
        void setName(QString name);
        void setPath(QString path);
        void setIsLoaded(bool isLoaded);
        void setVertexArray(std::vector<float> vertexArray);
        void setIndexArray(std::vector<unsigned int> indexAarray);
        void setMaxExtents(glm::vec4 max);
        void setMinExtents(glm::vec4 min);
        void setCentroid(glm::vec4 cent);

        QString getName() const;
        QString getPath() const;
        std::vector<float> getVertices()const;
        std::vector<unsigned int> getIndices()const;
        glm::vec4 getMaxExtent() const;
        glm::vec4 getMinExtent() const;
        glm::vec4 getCentroid() const;
        bool getIsLoaded() const;

    private:
        QString name;//the tag
        QString path;//the relative path of the model data file
        std::vector<float> vertexArray;//the actual vertices read from modelFile into memory
        std::vector<unsigned int> indexAarray;//the actual indeices read from modelFile into memory
        //
        glm::vec4 max;//max extent.
        glm::vec4 min;//min extent.
        glm::vec4 centroid;//centroid from min & max.
        bool isLoaded;
        //
        //!!buffer stuff should be done here and should hold these bindings.!!
        //uint VAOlocation
        //uint VBOlocation
        //uint EBOlocation

        //!!comes later
        // UVs
        // Normals
        // PBR?

    }_Model_Info;

    //----
    void setModelInfo(_AssetLoader::Model_Info minfo);
    _AssetLoader::Model_Info getModelInfo();
    std::vector<float> getAssetVertices();
    std::vector<unsigned int> getAssetIndices();
    void objLoader(QString pathToFile);
    void loadAllModelsInfoFromFolder(QString folderName);
    void calcMinMaxExtents();

private:
    _AssetLoader::Model_Info modelInfo;
    //
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<float> uvs;
    //
    glm::vec4 vertMin;
    glm::vec4 vertMax;
    int posCounter;
    int arrayCounter;

    glm::vec4 calcCentroidFromMinMax();

};

#endif // _ASSETLOADER_H
