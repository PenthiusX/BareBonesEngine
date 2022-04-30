#include "_anim.h"

_Anim::_Anim()
{

}

_Anim::_Anim(std::string pathToModelFile)
{
    Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE, aiDefaultLogStream_STDOUT);

    importer = new Assimp::Importer;
    assimpScene = importer->ReadFile(pathToModelFile,
                                    aiProcess_GenSmoothNormals |
                                    aiProcess_CalcTangentSpace |
                                    aiProcess_Triangulate //|
                                    //aiProcess_JoinIdenticalVertices //|
                                    //aiProcess_SortByPType
                                    );

    if(assimpScene){
        loadAssimpScene(assimpScene,pathToModelFile);
    }
    Assimp::DefaultLogger::kill();
}


int _Anim::getBoneId(const aiBone* pBone)
{
    int BoneIndex = 0;
    std::string BoneName(pBone->mName.C_Str());

    if (this->m_BoneNameToIndexMap.find(BoneName) == this->m_BoneNameToIndexMap.end()) {
        // Allocate an index for a new bone
        BoneIndex = (int)this->m_BoneNameToIndexMap.size();
        this->m_BoneNameToIndexMap[BoneName] = BoneIndex;
    }
    else {
        BoneIndex = this->m_BoneNameToIndexMap[BoneName];
    }

    return BoneIndex;
}

void _Anim::loadAssimpScene(const aiScene *scene, std::string path)
{
    for(uint i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[i];
        //--------------------Animation-------------------------------------
        if(mesh->HasBones())
        {
            this->m_Bones.resize(mesh->mNumVertices);//cleanup this structure~!!!!
            for (int boneIndex = 0; boneIndex < mesh->mNumBones; boneIndex++)
            {
                int BoneId = getBoneId(mesh->mBones[boneIndex]);

                if (BoneId == this->m_BoneTranformMatrixInfo.size()) {
                    BoneTranformMatrixInfo bi(mat4_cast(mesh->mBones[boneIndex]->mOffsetMatrix));
                    this->m_BoneTranformMatrixInfo.push_back(bi);
                }

                uint weightNumber = mesh->mBones[boneIndex]->mNumWeights;
                for (uint i = 0 ; i < weightNumber ; i++) {
                    const aiVertexWeight& vw = mesh->mBones[boneIndex]->mWeights[i];
                    uint GlobalVertexID = /*m_Meshes[MeshIndex].BaseVertex +*/mesh->mBones[boneIndex]->mWeights[i].mVertexId;
                    this->m_Bones[GlobalVertexID].AddBoneData(BoneId, vw.mWeight);
                }
            }
        }
    }
}

//Animation//Aditya WIP-----------------------------------------------------

uint _Anim::FindPosition(float AnimationTimeTicks, const aiNodeAnim *pNodeAnim)
{
    for (uint i = 0 ; i < pNodeAnim->mNumPositionKeys - 1 ; i++) {
        float t = (float)pNodeAnim->mPositionKeys[i + 1].mTime;
        if (AnimationTimeTicks < t) {
            return i;
        }
    }

    return 0;
}

void _Anim::CalcInterpolatedPosition(aiVector3D &Out, float AnimationTimeTicks, const aiNodeAnim *pNodeAnim)
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

uint _Anim::FindRotation(float AnimationTimeTicks, const aiNodeAnim *pNodeAnim)
{
    assert(pNodeAnim->mNumRotationKeys > 0);

    for (uint i = 0 ; i < pNodeAnim->mNumRotationKeys - 1 ; i++) {
        float t = (float)pNodeAnim->mRotationKeys[i + 1].mTime;
        if (AnimationTimeTicks < t) {
            return i;
        }
    }

    return 0;
}

void _Anim::CalcInterpolatedRotation(aiQuaternion &Out, float AnimationTimeTicks, const aiNodeAnim *pNodeAnim)
{
    // we need at least two values to interpolate...
    if (pNodeAnim->mNumRotationKeys == 1) {
        Out = pNodeAnim->mRotationKeys[0].mValue;
        return;
    }

    uint RotationIndex = FindRotation(AnimationTimeTicks, pNodeAnim);
    uint NextRotationIndex = RotationIndex + 1;
    assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
    float t1 = (float)pNodeAnim->mRotationKeys[RotationIndex].mTime;
    float t2 = (float)pNodeAnim->mRotationKeys[NextRotationIndex].mTime;
    float DeltaTime = t2 - t1;
    float Factor = (AnimationTimeTicks - t1) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
    const aiQuaternion& EndRotationQ   = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
    aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
    Out = StartRotationQ;
    Out.Normalize();
}

uint _Anim::FindScaling(float AnimationTimeTicks, const aiNodeAnim *pNodeAnim)
{
    assert(pNodeAnim->mNumScalingKeys > 0);

    for (uint i = 0 ; i < pNodeAnim->mNumScalingKeys - 1 ; i++) {
        float t = (float)pNodeAnim->mScalingKeys[i + 1].mTime;
        if (AnimationTimeTicks < t) {
            return i;
        }
    }

    return 0;
}

void _Anim::CalcInterpolatedScaling(aiVector3D &Out, float AnimationTimeTicks, const aiNodeAnim *pNodeAnim)
{
    // we need at least two values to interpolate...
    if (pNodeAnim->mNumScalingKeys == 1) {
        Out = pNodeAnim->mScalingKeys[0].mValue;
        return;
    }

    uint ScalingIndex = FindScaling(AnimationTimeTicks, pNodeAnim);
    uint NextScalingIndex = ScalingIndex + 1;
    assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
    float t1 = (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime;
    float t2 = (float)pNodeAnim->mScalingKeys[NextScalingIndex].mTime;
    float DeltaTime = t2 - t1;
    float Factor = (AnimationTimeTicks - (float)t1) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
    const aiVector3D& End   = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;
}

void _Anim::ReadNodeHierarchy(float AnimationTimeTicks, const aiNode *pNode, const glm::mat4 &ParentTransform)
{
    std::string NodeName(pNode->mName.data);

    const aiAnimation* pAnimation = assimpScene->mAnimations[0];
    glm::mat4 NodeTransformation = mat4_cast(pNode->mTransformation);
    const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

    if (pNodeAnim) {
        // Interpolate scaling and generate scaling transformation matrix
        aiVector3D Scaling;
        CalcInterpolatedScaling(Scaling, AnimationTimeTicks, pNodeAnim);
        glm::vec3 scale = glm::vec3(Scaling.x, Scaling.y, Scaling.z);
        glm::mat4 ScalingM = glm::scale(glm::mat4(1.0f), scale);

        // Interpolate rotation and generate rotation transformation matrix
        aiQuaternion RotationQ;
           CalcInterpolatedRotation(RotationQ, AnimationTimeTicks, pNodeAnim);
           glm::quat rotation = quat_cast(RotationQ);
           glm::mat4 RotationM = glm::mat4_cast(rotation);

        // Interpolate translation and generate translation transformation matrix
           aiVector3D Translation;
          CalcInterpolatedPosition(Translation, AnimationTimeTicks, pNodeAnim);
          glm::vec3 translation = glm::vec3(Translation.x, Translation.y, Translation.z);
          glm::mat4 TranslationM = glm::translate(glm::mat4(1.0f), translation);

        // Combine the above transformations
        NodeTransformation = TranslationM * RotationM * ScalingM;
    }

    glm::mat4 GlobalTransformation = ParentTransform * NodeTransformation;

    if (m_BoneNameToIndexMap.find(NodeName) != m_BoneNameToIndexMap.end()) {
        uint BoneIndex = m_BoneNameToIndexMap[NodeName];
        m_BoneTranformMatrixInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneTranformMatrixInfo[BoneIndex].OffsetMatrix;
    }

    for (uint i = 0 ; i < pNode->mNumChildren ; i++) {
        ReadNodeHierarchy(AnimationTimeTicks, pNode->mChildren[i], GlobalTransformation);
    }
}

void _Anim::GetBoneTransforms(float TimeInSeconds, std::vector<glm::mat4> &Transforms)
{
    glm::mat4 Identity;
    Identity = glm::mat4(1.f);

    float TicksPerSecond = (float)(assimpScene->mAnimations[0]->mTicksPerSecond != 0 ? assimpScene->mAnimations[0]->mTicksPerSecond : 25.0f);
    float TimeInTicks = TimeInSeconds * TicksPerSecond;
    float AnimationTimeTicks = fmod(TimeInTicks, (float)assimpScene->mAnimations[0]->mDuration);

    ReadNodeHierarchy(AnimationTimeTicks, assimpScene->mRootNode, Identity);
    Transforms.resize(m_BoneTranformMatrixInfo.size());

    for (uint i = 0 ; i < m_BoneTranformMatrixInfo.size() ; i++) {
        Transforms[i] = m_BoneTranformMatrixInfo[i].FinalTransformation;
    }
}

const aiNodeAnim *_Anim::FindNodeAnim(const aiAnimation *pAnimation, const std::string &NodeName)
{
    for (uint i = 0 ; i < pAnimation->mNumChannels ; i++) {
        const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

        if (std::string(pNodeAnim->mNodeName.data) == NodeName) {
            return pNodeAnim;
        }
    }

    return NULL;
}
