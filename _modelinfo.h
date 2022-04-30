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

//struct VertexBoneData
//{
//    //The size of both arrys need to be the same
//    //value of joiintINdex[0] correcpoinds to the weight at weights[0]
//    //every vertex can have more bones(id) effecting it, depected in jointIndicexId,
//    //and its correspointding weight.

//    //One can obtmise this by culling the most non influential id+weight vals and
//    //minmising it to 3 or 4 values, to improve perf.
//    std::vector<uint> jointIndicexId; // this is going to be passed to the shader using a gl3f or 4fv
//    std::vector<float> weights; // this is going to be passed to the shader using a gl3f or 4fv
//};
#define MAX_BONE_INFLUENCE 4
struct VertexInfo {//added Aug.2020//for a secondary model loading system
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;

    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

#define MAX_NUM_BONES_PER_VERTEX 4
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
struct VertexBoneData
{
    std::vector<uint>BoneIDs;
    std::vector<float>Weights;

    VertexBoneData()
    {
        BoneIDs.reserve(MAX_NUM_BONES_PER_VERTEX);
        Weights.reserve(MAX_NUM_BONES_PER_VERTEX);

        for(uint p = 0; p < MAX_NUM_BONES_PER_VERTEX ; p++){
                    BoneIDs.push_back(0);
                    Weights.push_back(0);
        }
    }

    void AddBoneData(uint BoneID, float Weight)
    {
        for (uint i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(BoneIDs) ; i++) {
            auto w = Weights[i];
            if (w == 0.0) {
                BoneIDs[i] = BoneID;
                Weights[i] = Weight;
                //printf("Adding bone %d weight %f at index %i\n", BoneID, Weight, i);
                return;
            }
        }
        // should never get here - more bones than we have space for
//        assert(0);
    }
};

struct BoneTranformMatrixInfo
{
    glm::mat4 OffsetMatrix;
    glm::mat4 FinalTransformation;

    BoneTranformMatrixInfo(const glm::mat4& Offset)
    {
        OffsetMatrix = Offset;
        FinalTransformation = glm::mat4();
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
//    uint FindPosition(float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);
//    void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);
//    uint FindRotation(float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);
//    void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);
//    uint FindScaling(float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);
//    void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);
//    void ReadNodeHierarchy(float AnimationTimeTicks, const aiNode* pNode, const glm::mat4x4& ParentTransform);
//    void GetBoneTransforms(float TimeInSeconds, std::vector<glm::mat4x4>& Transforms);
//    const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string& NodeName);

    glm::mat4 m_GlobalInverseTransform;
    std::map<std::string,uint> m_BoneNameToIndexMap;
    std::vector<BoneTranformMatrixInfo> m_BoneTranformMatrixInfo;
    std::vector<VertexBoneData> m_Bones;
//    // For converting between ASSIMP and glm
//    static inline glm::vec3 vec3_cast(const aiVector3D &v) { return glm::vec3(v.x, v.y, v.z); }
//    static inline glm::vec2 vec2_cast(const aiVector3D &v) { return glm::vec2(v.x, v.y); }
//    static inline glm::quat quat_cast(const aiQuaternion &q) { return glm::quat(q.w, q.x, q.y, q.z); }
//    static inline glm::mat4 mat4_cast(const aiMatrix4x4 &m) { return glm::transpose(glm::make_mat4(&m.a1)); }
//    static inline glm::mat4 mat4_cast(const aiMatrix3x3 &m) { return glm::transpose(glm::make_mat3(&m.a1)); }
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
