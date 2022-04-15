#ifndef _MODELINFO_H
#define _MODELINFO_H
#include <QString>
#include <vector>
#include <glm/glm.hpp>

struct VertexInfo {//added Aug.2020//for a secondary model loading system
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct BoneInfo
{
    glm::mat4x4 OffsetMatrix;
    glm::mat4x4 FinalTransformation;

    BoneInfo(const glm::mat4x4& Offset)
    {
        OffsetMatrix = Offset;
//        FinalTransformation.SetZero();
    }
};

class _ModelInfo
{
public:
    _ModelInfo();

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
