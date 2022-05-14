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

#include "_modelinfo.h"

class _Anim
{
public:
    _Anim();
    _Anim(std::string pathToModelFile);//Load model and anim data from model file
    ~_Anim();

    const aiScene* assimpScene = nullptr;
    Assimp::Importer *importer;
    void loadAssimpScene(const aiScene* scene, std::string path);


    std::map<std::string,uint> m_BoneNameToIndexMap;
    std::vector<BoneTranformMatrixInfo> m_BoneTranformMatrixInfo;

    glm::quat rotate_head_xz = glm::quat(cos(glm::radians(0.0f)), sin(glm::radians(0.0f)) * glm::vec3(1.0f, 0.0f, 0.0f));
    aiMatrix4x4 m_GlobalInverseTransform;

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
