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
#include <math.h>

#include "_modelinfo.h"//Wip Aditya

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
//    uint FindPosition(float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);
//    void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);
//    uint FindRotation(float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);
//    void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);
//    uint FindScaling(float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);
//    void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);
//    void ReadNodeHierarchy(float AnimationTimeTicks, const aiNode* pNode, const glm::mat4& ParentTransform);
//    void GetBoneTransforms(float TimeInSeconds, std::vector<glm::mat4>& Transforms);
//    const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string& NodeName);

//    glm::mat4 m_GlobalInverseTransform;
//    std::map<std::string,uint> m_BoneNameToIndexMap;
//    std::vector<BoneTranformMatrixInfo> m_BoneTranformMatrixInfo;
//    std::vector<VertexBoneData> m_Bones;


    // For converting between ASSIMP and glm
    static inline glm::vec3 vec3_cast(const aiVector3D &v) { return glm::vec3(v.x, v.y, v.z); }
    static inline glm::vec2 vec2_cast(const aiVector3D &v) { return glm::vec2(v.x, v.y); }
    static inline glm::quat quat_cast(const aiQuaternion &q) { return glm::quat(q.w, q.x, q.y, q.z); }
    static inline glm::mat4 mat4_cast(const aiMatrix4x4 &m) { return glm::transpose(glm::make_mat4(&m.a1)); }
    static inline glm::mat4 mat4_cast(const aiMatrix3x3 &m) { return glm::transpose(glm::make_mat3(&m.a1)); }
    //Animation//Aditya WIP-----------------------------------------------------




    aiMatrix4x4 m_GlobalInverseTransform;
    std::map<std::string,uint> m_BoneNameToIndexMap;
    std::vector<BoneTranformMatrixInfoAi> m_BoneTranformMatrixInfo;
    std::vector<VertexBoneData> m_Bones;
    glm::quat rotate_head_xz = glm::quat(cos(glm::radians(0.0f)), sin(glm::radians(0.0f)) * glm::vec3(1.0f, 0.0f, 0.0f)); // this quad do nothingggggg!!!!!

    aiQuaternion nlerp(aiQuaternion a, aiQuaternion b, float blend);
    uint findPosition(float p_animation_time, const aiNodeAnim* p_node_anim);
    uint findRotation(float p_animation_time, const aiNodeAnim* p_node_anim);
    uint findScaling(float p_animation_time, const aiNodeAnim* p_node_anim);
    aiVector3D calcInterpolatedPosition(float p_animation_time, const aiNodeAnim* p_node_anim);
    aiQuaternion calcInterpolatedRotation(float p_animation_time, const aiNodeAnim* p_node_anim);
    aiVector3D calcInterpolatedScaling(float p_animation_time, const aiNodeAnim* p_node_anim);
    const aiNodeAnim * findNodeAnim(const aiAnimation * p_animation, const std::string p_node_name);
    void readNodeHierarchy(float p_animation_time, const aiNode* p_node, const aiMatrix4x4 parent_transform);
    void boneTransform(double time_in_sec, std::vector<aiMatrix4x4>& transforms);
private:
};

#endif // _ANIM_H
