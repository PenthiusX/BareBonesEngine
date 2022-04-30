#ifndef _ANIM_H
#define _ANIM_H
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

#include "_modelinfo.h"//Wip Aditya

#define MAX_NUM_BONES_PER_VERTEX 4
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
//struct VertexBoneData
//{
////    uint BoneIDs[MAX_NUM_BONES_PER_VERTEX];// = { 0 };
////    float Weights[MAX_NUM_BONES_PER_VERTEX];// = { 0.0f };

//    std::vector<uint>BoneIDs;
//    std::vector<float>Weights;

//    VertexBoneData()
//    {
//        BoneIDs.reserve(MAX_NUM_BONES_PER_VERTEX);
//        Weights.reserve(MAX_NUM_BONES_PER_VERTEX);

//        for(uint p = 0; p < MAX_NUM_BONES_PER_VERTEX ; p++){
//                    BoneIDs.push_back(0);
//                    Weights.push_back(0);
//        }
//    }

//    void AddBoneData(uint BoneID, float Weight)
//    {
//        for (uint i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(BoneIDs) ; i++) {
//            auto w = Weights[i];
//            if (w == 0.0) {
//                BoneIDs[i] = BoneID;
//                Weights[i] = Weight;
//                //printf("Adding bone %d weight %f at index %i\n", BoneID, Weight, i);
//                return;
//            }
//        }
//        // should never get here - more bones than we have space for
////        assert(0);
//    }
//};

//struct BoneInfo
//{
//    glm::mat4x4 OffsetMatrix;
//    glm::mat4x4 FinalTransformation;

//    BoneInfo(const glm::mat4x4& Offset)
//    {
//        OffsetMatrix = Offset;
//        FinalTransformation = glm::mat4();
//    }
//};

class _Anim//Wip aditya
{
public:
    _Anim();
    _Anim(std::string pathToModelFile);//Load model and anim data from model file

    ~_Anim(){
        delete importer;
    };
    //Animation//Aditya WIP-----------------------------------------------------
    const aiScene* assimpScene = nullptr;
    Assimp::Importer *importer;
    void loadAssimpScene(const aiScene* scene, std::string path);
    int getBoneId(const aiBone* pBone);

    uint FindPosition(float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);
    uint FindRotation(float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);
    uint FindScaling(float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);
    void ReadNodeHierarchy(float AnimationTimeTicks, const aiNode* pNode, const glm::mat4& ParentTransform);
    void GetBoneTransforms(float TimeInSeconds, std::vector<glm::mat4>& Transforms);
    const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string& NodeName);

    glm::mat4 m_GlobalInverseTransform;
    std::map<std::string,uint> m_BoneNameToIndexMap;
    std::vector<BoneTranformMatrixInfo> m_BoneTranformMatrixInfo;
    std::vector<VertexBoneData> m_Bones;

    // For converting between ASSIMP and glm
    static inline glm::vec3 vec3_cast(const aiVector3D &v) { return glm::vec3(v.x, v.y, v.z); }
    static inline glm::vec2 vec2_cast(const aiVector3D &v) { return glm::vec2(v.x, v.y); }
    static inline glm::quat quat_cast(const aiQuaternion &q) { return glm::quat(q.w, q.x, q.y, q.z); }
    static inline glm::mat4 mat4_cast(const aiMatrix4x4 &m) { return glm::transpose(glm::make_mat4(&m.a1)); }
    static inline glm::mat4 mat4_cast(const aiMatrix3x3 &m) { return glm::transpose(glm::make_mat3(&m.a1)); }
    //Animation//Aditya WIP-----------------------------------------------------
};

#endif // _ANIM_H
