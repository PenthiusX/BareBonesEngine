#ifndef A_ANIMATION_H
#define A_ANIMATION_H

#include<glm/glm.hpp>
#include<glm/gtc/quaternion.hpp>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <assimp/Logger.hpp>
#include <assimp/DefaultLogger.hpp>
#include<assimp/quaternion.h>
#include<assimp/vector3.h>
#include<assimp/matrix4x4.h>

#include "a_bone.h"
#include "_modelinfo.h"

struct AssimpNodeData
{
    glm::mat4 transformation;
    std::string name;
    int childrenCount;
    std::vector<AssimpNodeData> children;
};

class _Animation
{
    public:
        _Animation();
        _Animation(const std::string& animationPath, _ModelInfo* model);
       ~_Animation();

        Bone* FindBone(const std::string& name);

        inline float GetTicksPerSecond();
        inline float GetDuration();
        inline const AssimpNodeData& GetRootNode();
        inline const std::map<std::string,BoneInfo>& GetBoneIDMap();

    private:
        void ReadMissingBones(const aiAnimation* animation, _ModelInfo& model);
        void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);

        float m_Duration;
        int m_TicksPerSecond;
        std::vector<Bone> m_Bones;
        AssimpNodeData m_RootNode;
        std::map<std::string, BoneInfo> m_BoneInfoMap;
};


class Animator
{
public:
    Animator();
    Animator(_Animation animation);
    ~Animator();

    void UpdateAnimation(float dt);

    void PlayAnimation(_Animation pAnimation);

    void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);



    //OGLdev
    uint FindPosition(float AnimationTimeTicks, const aiNodeAnim* pNodeAnim)
    {
        for (uint i = 0 ; i < pNodeAnim->mNumPositionKeys - 1 ; i++) {
            float t = (float)pNodeAnim->mPositionKeys[i + 1].mTime;
            if (AnimationTimeTicks < t) {
                return i;
            }
        }

        return 0;
    }
    void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTimeTicks, const aiNodeAnim* pNodeAnim)
    {
        // we need at least two values to interpolate...
        if (pNodeAnim->mNumPositionKeys == 1) {
            Out = pNodeAnim->mPositionKeys[0].mValue;
            return;
        }

        uint PositionIndex = FindPosition(AnimationTimeTicks, pNodeAnim);
        uint NextPositionIndex = PositionIndex + 1;
        assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
        float t1 = (float)pNodeAnim->mPositionKeys[PositionIndex].mTime;
        float t2 = (float)pNodeAnim->mPositionKeys[NextPositionIndex].mTime;
        float DeltaTime = t2 - t1;
        float Factor = (AnimationTimeTicks - t1) / DeltaTime;
        assert(Factor >= 0.0f && Factor <= 1.0f);
        const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
        const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
        aiVector3D Delta = End - Start;
        Out = Start + Factor * Delta;
    }
    //OGLdev

    std::vector<glm::mat4> GetFinalBoneMatrices();

private:
    std::vector<glm::mat4> m_FinalBoneMatrices;
    _Animation m_CurrentAnimation ;
    float m_CurrentTime = 0.0f;
    float m_DeltaTime = 0.0f;
};

#endif // A_ANIMATION_H
