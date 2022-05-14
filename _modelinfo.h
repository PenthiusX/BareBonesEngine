#ifndef _MODELINFO_H
#define _MODELINFO_H
#include <QString>
#include <vector>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <sstream>

#include<glm/glm.hpp>
#include<glm/gtc/quaternion.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <assimp/Logger.hpp>
#include <assimp/DefaultLogger.hpp>

#include<assimp/quaternion.h>
#include<assimp/vector3.h>
#include<assimp/matrix4x4.h>

#include <map>

#define MAX_NUM_BONES_PER_VERTEX 4

struct VertexInfo {//added Aug.2020//for a secondary model loading system
    //Model data
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    //Animation data
    int m_BoneIDs[MAX_NUM_BONES_PER_VERTEX];//bone indexes which will influence this vertex
    float m_Weights[MAX_NUM_BONES_PER_VERTEX];//weights from each bone
};

struct BoneTranformMatrixInfo
{
    aiMatrix4x4 OffsetMatrix;
    aiMatrix4x4 FinalTransformation;

    BoneTranformMatrixInfo(const aiMatrix4x4& Offset)
    {
        OffsetMatrix = Offset;
        FinalTransformation.IsIdentity();
    }
};

class _ModelInfo
{
public:
    _ModelInfo();
    ~_ModelInfo();

    void setName(QString name);
    void setPath(QString path);
    void setIsLoaded(bool isLoaded);
    void setVertexArray(std::vector<float> vertexArray);
    void setVertexInfoArray(std::vector<VertexInfo> vertexInfoArray);
    void setIndexArray(std::vector<unsigned int> indexAarray);
    void setNormalArray(std::vector<float>norArray);
    void setMaxExtents(glm::vec4 max);
    void setMinExtents(glm::vec4 min);
    void setCentroid(glm::vec4 cent);
    //
    QString getName() const;
    QString getPath() const;
    std::vector<float> getVertexArray()const;
    std::vector<VertexInfo> getVertexInfoArray()const;
    std::vector<uint> getIndexArray()const;
    std::vector<float> getNormalArray()const;
    glm::vec4 getMaxExtent() const;
    glm::vec4 getMinExtent() const;
    glm::vec4 getCentroid() const;
    bool getIsLoaded() const;

    void calcMinMaxExtents();
    glm::vec4 calcCentroidFromMinMax();
    glm::vec4 calcCentroidFromMinMax(glm::vec3 min, glm::vec3 max);

    //Animation//Aditya WIP-----------------------------------------------------
    std::map<std::string,uint> m_BoneNameToIndexMap;
    //Animation//Aditya WIP-----------------------------------------------------

private:
    QString name;//the tag
    QString path;//the relative path of the model data file
    std::vector<VertexInfo> vertexInfoArray;//Holds vertex,normal and texcordData//will replace existingsss
    std::vector<float> vertexArray;// vertices read from modelFile into memory
    std::vector<uint> indexAarray;// indeices read from modelFile into memory
    std::vector<float> normalArray;// normal array
    //
    glm::vec4 max;//max extent.
    glm::vec4 min;//min extent.
    glm::vec4 cent;//centroid from min & max.
    bool isLoaded;
};

#endif // _MODELINFO_H
