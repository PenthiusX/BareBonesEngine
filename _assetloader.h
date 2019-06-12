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
    private:
        QString name;//the tag
        QString path;//the relative path of the model data file
        std::vector<float> vertexArray;//the actual vertices read from modelFile into memory
        std::vector<unsigned int> indexAarray;//the actual indeices read from modelFile into memory
        //
        QVector3D max;//max extent.
        QVector3D min;//min extent.
        QVector3D centroid;//centroid from min & max.
        bool isLoaded;
        //
        //!!buffer stuff should be done here and should hold these bindings.!!
        //uint VAOlocation
        //uint VBOlocation
        //uint EBOlocation
        //!!comes later
        // UVs
        // Normals
        // PBR

    public:
        void setName(QString name);
        void setPath(QString path);
        void setIsLoaded(bool isLoaded);
        void setVertexArray(std::vector<float> vertexArray);
        void setIndexArray(std::vector<unsigned int> indexAarray);
        void setMaxExtents(QVector3D max);
        void setMinExtents(QVector3D min);
        void setTranslationMatrix(glm::mat4x4 t);
        void setRotationMatrix(glm::mat4x4 r);
        void setScalingMatrix(glm::mat4x4 s);
        QString getName() const;
        QString getPath() const;
        std::vector<float> getVertices()const;
        std::vector<unsigned int> getIndices()const;
        QVector3D getMaxExtent() const;
        QVector3D getMinExtent() const;
        QVector3D getCentroid() const;
        bool getIsLoaded() const;
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
    QVector3D vertMin;
    QVector3D vertMax;
    int posCounter;
    int arrayCounter;
};

#endif // _ASSETLOADER_H
