#ifndef _MODELINFO_H
#define _MODELINFO_H
#include <Qstring>
#include <vector>
#include <glm/glm.hpp>

class _ModelInfo
{
public:
    _ModelInfo();

    void setName(QString name);
    void setPath(QString path);
    void setIsLoaded(bool isLoaded);
    void setVertexArray(std::vector<float> vertexArray);
    void setIndexArray(std::vector<unsigned int> indexAarray);
    void setMaxExtents(glm::vec4 max);
    void setMinExtents(glm::vec4 min);
    void setCentroid(glm::vec4 cent);
    //
    QString getName() const;
    QString getPath() const;
    std::vector<float> getVerticexArray()const;
    std::vector<unsigned int> getIndexArray()const;
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
    std::vector<float> vertexArray;//the actual vertices read from modelFile into memory
    std::vector<unsigned int> indexAarray;//the actual indeices read from modelFile into memory
    //
    glm::vec4 max;//max extent.
    glm::vec4 min;//min extent.
    glm::vec4 cent;//centroid from min & max.
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
};

#endif // _MODELINFO_H
