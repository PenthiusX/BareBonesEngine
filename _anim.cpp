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

//int _Anim::getBoneId(const aiBone* pBone)
//{
//    int BoneIndex = 0;
//    std::string BoneName(pBone->mName.C_Str());

//    if (this->m_BoneNameToIndexMap.find(BoneName) == this->m_BoneNameToIndexMap.end()) {
//        // Allocate an index for a new bone
//        BoneIndex = (int)this->m_BoneNameToIndexMap.size();
//        this->m_BoneNameToIndexMap[BoneName] = BoneIndex;
//    }
//    else {
//        BoneIndex = this->m_BoneNameToIndexMap[BoneName];
//    }

//    return BoneIndex;
//}

//void _Anim::loadAssimpScene(const aiScene *scene, std::string path)
//{
//    for(uint i = 0; i < scene->mNumMeshes; i++)
//    {
//        aiMesh* mesh = scene->mMeshes[i];
//        //--------------------Animation-------------------------------------
//        if(mesh->HasBones())
//        {
//            this->m_Bones.resize(mesh->mNumVertices);//cleanup this structure~!!!!
//            for (int boneIndex = 0; boneIndex < mesh->mNumBones; boneIndex++)
//            {
//                int BoneId = getBoneId(mesh->mBones[boneIndex]);

//                if (BoneId == this->m_BoneTranformMatrixInfo.size()) {
//                    BoneTranformMatrixInfo bi(mat4_cast(mesh->mBones[boneIndex]->mOffsetMatrix));
//                    this->m_BoneTranformMatrixInfo.push_back(bi);
//                }

//                uint weightNumber = mesh->mBones[boneIndex]->mNumWeights;
//                for (uint i = 0 ; i < weightNumber ; i++) {
//                    const aiVertexWeight& vw = mesh->mBones[boneIndex]->mWeights[i];
//                    uint GlobalVertexID = /*m_Meshes[MeshIndex].BaseVertex +*/mesh->mBones[boneIndex]->mWeights[i].mVertexId;
//                    this->m_Bones[GlobalVertexID].AddBoneData(BoneId, vw.mWeight);
//                }
//            }
//        }
//    }
//    m_GlobalInverseTransform =  mat4_cast(scene->mRootNode->mTransformation);
//    m_GlobalInverseTransform = glm::inverse(m_GlobalInverseTransform);
////    m_GlobalInverseTransform =  scene->mRootNode->mTransformation.Inverse();

//}

////Animation//Aditya WIP-----------------------------------------------------

//uint _Anim::FindPosition(float AnimationTimeTicks, const aiNodeAnim *pNodeAnim)
//{
//    for (uint i = 0 ; i < pNodeAnim->mNumPositionKeys - 1 ; i++) {
//        float t = (float)pNodeAnim->mPositionKeys[i + 1].mTime;
//        if (AnimationTimeTicks < t) {
//            return i;
//        }
//    }

//    return 0;
//}

//void _Anim::CalcInterpolatedPosition(aiVector3D &Out, float AnimationTimeTicks, const aiNodeAnim *pNodeAnim)
//{
//    // we need at least two values to interpolate...
//    if (pNodeAnim->mNumPositionKeys == 1) {
//        Out = pNodeAnim->mPositionKeys[0].mValue;
//        return;
//    }

//    uint PositionIndex = FindPosition(AnimationTimeTicks, pNodeAnim);
//    uint NextPositionIndex = PositionIndex + 1;
//    assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
//    float t1 = (float)pNodeAnim->mPositionKeys[PositionIndex].mTime;
//    float t2 = (float)pNodeAnim->mPositionKeys[NextPositionIndex].mTime;
//    float DeltaTime = t2 - t1;
//    float Factor = (AnimationTimeTicks - t1) / DeltaTime;
//    assert(Factor >= 0.0f && Factor <= 1.0f);
//    const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
//    const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
//    aiVector3D Delta = End - Start;
//    Out = Start + Factor * Delta;

//}

//uint _Anim::FindRotation(float AnimationTimeTicks, const aiNodeAnim *pNodeAnim)
//{
//    assert(pNodeAnim->mNumRotationKeys > 0);

//    for (uint i = 0 ; i < pNodeAnim->mNumRotationKeys - 1 ; i++) {
//        float t = (float)pNodeAnim->mRotationKeys[i + 1].mTime;
//        if (AnimationTimeTicks < t) {
//            return i;
//        }
//    }

//    return 0;
//}

//void _Anim::CalcInterpolatedRotation(aiQuaternion &Out, float AnimationTimeTicks, const aiNodeAnim *pNodeAnim)
//{
//    // we need at least two values to interpolate...
//    if (pNodeAnim->mNumRotationKeys == 1) {
//        Out = pNodeAnim->mRotationKeys[0].mValue;
//        return;
//    }

//    uint RotationIndex = FindRotation(AnimationTimeTicks, pNodeAnim);
//    uint NextRotationIndex = RotationIndex + 1;
//    assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
//    float t1 = (float)pNodeAnim->mRotationKeys[RotationIndex].mTime;
//    float t2 = (float)pNodeAnim->mRotationKeys[NextRotationIndex].mTime;
//    float DeltaTime = t2 - t1;
//    float Factor = (AnimationTimeTicks - t1) / DeltaTime;
//    assert(Factor >= 0.0f && Factor <= 1.0f);
//    const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
//    const aiQuaternion& EndRotationQ   = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
//    aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
//    Out = StartRotationQ;
//    Out.Normalize();
//}

//uint _Anim::FindScaling(float AnimationTimeTicks, const aiNodeAnim *pNodeAnim)
//{
//    assert(pNodeAnim->mNumScalingKeys > 0);

//    for (uint i = 0 ; i < pNodeAnim->mNumScalingKeys - 1 ; i++) {
//        float t = (float)pNodeAnim->mScalingKeys[i + 1].mTime;
//        if (AnimationTimeTicks < t) {
//            return i;
//        }
//    }

//    return 0;
//}

//void _Anim::CalcInterpolatedScaling(aiVector3D &Out, float AnimationTimeTicks, const aiNodeAnim *pNodeAnim)
//{
//    // we need at least two values to interpolate...
//    if (pNodeAnim->mNumScalingKeys == 1) {
//        Out = pNodeAnim->mScalingKeys[0].mValue;
//        return;
//    }

//    uint ScalingIndex = FindScaling(AnimationTimeTicks, pNodeAnim);
//    uint NextScalingIndex = ScalingIndex + 1;
//    assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
//    float t1 = (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime;
//    float t2 = (float)pNodeAnim->mScalingKeys[NextScalingIndex].mTime;
//    float DeltaTime = t2 - t1;
//    float Factor = (AnimationTimeTicks - (float)t1) / DeltaTime;
//    assert(Factor >= 0.0f && Factor <= 1.0f);
//    const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
//    const aiVector3D& End   = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
//    aiVector3D Delta = End - Start;
//    Out = Start + Factor * Delta;
//}

//void _Anim::ReadNodeHierarchy(float AnimationTimeTicks, const aiNode *pNode, const glm::mat4 &ParentTransform)
//{
//    std::string NodeName(pNode->mName.data);

//    const aiAnimation* pAnimation = assimpScene->mAnimations[0];
//    glm::mat4 NodeTransformation = mat4_cast(pNode->mTransformation);
//    const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

//    if (pNodeAnim) {
//        // Interpolate scaling and generate scaling transformation matrix
//        aiVector3D Scaling;
//        CalcInterpolatedScaling(Scaling, AnimationTimeTicks, pNodeAnim);
//        glm::vec3 scale = glm::vec3(Scaling.x, Scaling.y, Scaling.z);
//        glm::mat4 ScalingM = glm::scale(glm::mat4(1.0f), scale);

//        // Interpolate rotation and generate rotation transformation matrix
//        aiQuaternion RotationQ;
//           CalcInterpolatedRotation(RotationQ, AnimationTimeTicks, pNodeAnim);
//           glm::quat rotation = quat_cast(RotationQ);
//           glm::mat4 RotationM = glm::mat4_cast(rotation);

//        // Interpolate translation and generate translation transformation matrix
//           aiVector3D Translation;
//          CalcInterpolatedPosition(Translation, AnimationTimeTicks, pNodeAnim);
//          glm::vec3 translation = glm::vec3(Translation.x, Translation.y, Translation.z);
//          glm::mat4 TranslationM = glm::translate(glm::mat4(1.0f), translation);

//        // Combine the above transformations
//        NodeTransformation = TranslationM * RotationM * ScalingM;
//    }

//    glm::mat4 GlobalTransformation = ParentTransform * NodeTransformation;

//    if (m_BoneNameToIndexMap.find(NodeName) != m_BoneNameToIndexMap.end()) {
//        uint BoneIndex = m_BoneNameToIndexMap[NodeName];
//        m_BoneTranformMatrixInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneTranformMatrixInfo[BoneIndex].OffsetMatrix;
//    }

//    for (uint i = 0 ; i < pNode->mNumChildren ; i++) {
//        ReadNodeHierarchy(AnimationTimeTicks, pNode->mChildren[i], GlobalTransformation);
//    }
//}

//void _Anim::GetBoneTransforms(float TimeInSeconds, std::vector<glm::mat4> &Transforms)
//{
//    glm::mat4 Identity;
//    Identity = glm::mat4(1.f);

//    float TicksPerSecond = (float)(assimpScene->mAnimations[0]->mTicksPerSecond != 0 ? assimpScene->mAnimations[0]->mTicksPerSecond : 25.0f);
//    float TimeInTicks = TimeInSeconds * TicksPerSecond;
//    float AnimationTimeTicks = fmod(TimeInTicks, (float)assimpScene->mAnimations[0]->mDuration);

//    ReadNodeHierarchy(AnimationTimeTicks, assimpScene->mRootNode, Identity);
//    Transforms.resize(m_BoneTranformMatrixInfo.size());

//    for (uint i = 0 ; i < m_BoneTranformMatrixInfo.size() ; i++) {
//        Transforms[i] = m_BoneTranformMatrixInfo[i].FinalTransformation;
//    }
//}

//const aiNodeAnim *_Anim::FindNodeAnim(const aiAnimation *pAnimation, const std::string &NodeName)
//{
//    for (uint i = 0 ; i < pAnimation->mNumChannels ; i++) {
//        const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

//        if (std::string(pNodeAnim->mNodeName.data) == NodeName) {
//            return pNodeAnim;
//        }
//    }

//    return NULL;
//}





void _Anim::loadAssimpScene(const aiScene *scene, std::string path)
{
    for(uint i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[i];
        //--------------------Animation-------------------------------------
        if(mesh->HasBones())
        {
            this->m_Bones.resize(mesh->mNumVertices);//cleanup this structure~!!!!
            for (int b = 0; b < mesh->mNumBones; b++)
            {

                int BoneIndex = 0;
                std::string BoneName(mesh->mBones[b]->mName.C_Str());

                if (this->m_BoneNameToIndexMap.find(BoneName) == this->m_BoneNameToIndexMap.end()) {
                    // Allocate an index for a new bone
                    BoneIndex = (int)this->m_BoneNameToIndexMap.size();
                    this->m_BoneNameToIndexMap[BoneName] = BoneIndex;
                }
                else {
                    BoneIndex = this->m_BoneNameToIndexMap[BoneName];
                }

                //                int bonindex = GetBoneId(mesh->mBones[b]);

                if (BoneIndex == this->m_BoneTranformMatrixInfo.size())
                {
                    BoneTranformMatrixInfoAi bi(mesh->mBones[b]->mOffsetMatrix);
                    this->m_BoneTranformMatrixInfo.push_back(bi);
                }

                uint weightNumber = mesh->mBones[b]->mNumWeights;
                for (uint i = 0 ; i < weightNumber ; i++) {

                    const aiVertexWeight& vw = mesh->mBones[b]->mWeights[i];
                    uint GlobalVertexID = mesh->mBones[b]->mWeights[i].mVertexId;
                    this->m_Bones[GlobalVertexID].AddBoneData(BoneIndex, vw.mWeight);
                }
            }
        }

        m_GlobalInverseTransform =  scene->mRootNode->mTransformation.Inverse();
    }
}


aiQuaternion _Anim::nlerp(aiQuaternion a, aiQuaternion b, float blend)
{
    //cout << a.w + a.x + a.y + a.z << endl;
    a.Normalize();
    b.Normalize();

    aiQuaternion result;
    float dot_product = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    float one_minus_blend = 1.0f - blend;

    if (dot_product < 0.0f)
    {
        result.x = a.x * one_minus_blend + blend * -b.x;
        result.y = a.y * one_minus_blend + blend * -b.y;
        result.z = a.z * one_minus_blend + blend * -b.z;
        result.w = a.w * one_minus_blend + blend * -b.w;
    }
    else
    {
        result.x = a.x * one_minus_blend + blend * b.x;
        result.y = a.y * one_minus_blend + blend * b.y;
        result.z = a.z * one_minus_blend + blend * b.z;
        result.w = a.w * one_minus_blend + blend * b.w;
    }

    return result.Normalize();
}

uint _Anim::findPosition(float p_animation_time, const aiNodeAnim *p_node_anim)
{
    for (uint i = 0; i < p_node_anim->mNumPositionKeys - 1; i++)
    {
        if (p_animation_time < (float)p_node_anim->mPositionKeys[i + 1].mTime)
        {
            return i;
        }
    }
    assert(0);
    return 0;
}

uint _Anim::findRotation(float p_animation_time, const aiNodeAnim *p_node_anim)
{

    for (uint i = 0; i < p_node_anim->mNumRotationKeys - 1; i++)
    {
        if (p_animation_time < (float)p_node_anim->mRotationKeys[i + 1].mTime)
        {
            return i;
        }
    }

    assert(0);
    return 0;
}

uint _Anim::findScaling(float p_animation_time, const aiNodeAnim *p_node_anim)
{

    for (uint i = 0; i < p_node_anim->mNumScalingKeys - 1; i++)
    {
        if (p_animation_time < (float)p_node_anim->mScalingKeys[i + 1].mTime)
        {
            return i;
        }
    }

    assert(0);
    return 0;
}

aiVector3D _Anim::calcInterpolatedPosition(float p_animation_time, const aiNodeAnim *p_node_anim)
{
    if (p_node_anim->mNumPositionKeys == 1)
    {
        return p_node_anim->mPositionKeys[0].mValue;
    }

    uint position_index = findPosition(p_animation_time, p_node_anim);
    uint next_position_index = position_index + 1;
    assert(next_position_index < p_node_anim->mNumPositionKeys);

    float delta_time = (float)(p_node_anim->mPositionKeys[next_position_index].mTime - p_node_anim->mPositionKeys[position_index].mTime);

    float factor = (p_animation_time - (float)p_node_anim->mPositionKeys[position_index].mTime) / delta_time;
    assert(factor >= 0.0f && factor <= 1.0f);
    aiVector3D start = p_node_anim->mPositionKeys[position_index].mValue;
    aiVector3D end = p_node_anim->mPositionKeys[next_position_index].mValue;
    aiVector3D delta = end - start;

    return start + factor * delta;
}

aiQuaternion _Anim::calcInterpolatedRotation(float p_animation_time, const aiNodeAnim *p_node_anim)
{
    if (p_node_anim->mNumRotationKeys == 1)
    {
        return p_node_anim->mRotationKeys[0].mValue;
    }

    uint rotation_index = findRotation(p_animation_time, p_node_anim);
    uint next_rotation_index = rotation_index + 1;
    assert(next_rotation_index < p_node_anim->mNumRotationKeys);

    float delta_time = (float)(p_node_anim->mRotationKeys[next_rotation_index].mTime - p_node_anim->mRotationKeys[rotation_index].mTime);

    float factor = (p_animation_time - (float)p_node_anim->mRotationKeys[rotation_index].mTime) / delta_time;

    //cout << "p_node_anim->mRotationKeys[rotation_index].mTime: " << p_node_anim->mRotationKeys[rotation_index].mTime << endl;
    //cout << "p_node_anim->mRotationKeys[next_rotaion_index].mTime: " << p_node_anim->mRotationKeys[next_rotation_index].mTime << endl;
    //cout << "delta_time: " << delta_time << endl;
    //cout << "animation_time: " << p_animation_time << endl;
    //cout << "animation_time - mRotationKeys[rotation_index].mTime: " << (p_animation_time - (float)p_node_anim->mRotationKeys[rotation_index].mTime) << endl;
    //cout << "factor: " << factor << endl << endl << endl;

    assert(factor >= 0.0f && factor <= 1.0f);
    aiQuaternion start_quat = p_node_anim->mRotationKeys[rotation_index].mValue;
    aiQuaternion end_quat = p_node_anim->mRotationKeys[next_rotation_index].mValue;

    return nlerp(start_quat, end_quat, factor);
}

aiVector3D _Anim::calcInterpolatedScaling(float p_animation_time, const aiNodeAnim *p_node_anim)
{
    if (p_node_anim->mNumScalingKeys == 1) // Keys ��� ������� �����
    {
        return p_node_anim->mScalingKeys[0].mValue;
    }

    uint scaling_index = findScaling(p_animation_time, p_node_anim);
    uint next_scaling_index = scaling_index + 1;
    assert(next_scaling_index < p_node_anim->mNumScalingKeys);

    float delta_time = (float)(p_node_anim->mScalingKeys[next_scaling_index].mTime - p_node_anim->mScalingKeys[scaling_index].mTime);

    float  factor = (p_animation_time - (float)p_node_anim->mScalingKeys[scaling_index].mTime) / delta_time;
    assert(factor >= 0.0f && factor <= 1.0f);
    aiVector3D start = p_node_anim->mScalingKeys[scaling_index].mValue;
    aiVector3D end = p_node_anim->mScalingKeys[next_scaling_index].mValue;
    aiVector3D delta = end - start;

    return start + factor * delta;
}

const aiNodeAnim *_Anim::findNodeAnim(const aiAnimation *p_animation, const std::string p_node_name)
{
    // channel in animation contains aiNodeAnim (aiNodeAnim its transformation for bones)
    // numChannels == numBones
    for (uint i = 0; i < p_animation->mNumChannels; i++)
    {
        const aiNodeAnim* node_anim = p_animation->mChannels[i];
        if (std::string(node_anim->mNodeName.data) == p_node_name)
        {
            return node_anim;
        }
    }

    return nullptr;
}

void _Anim::readNodeHierarchy(float p_animation_time, const aiNode *p_node, const aiMatrix4x4 parent_transform)
{

    std::string node_name(p_node->mName.data);


    const aiAnimation* animation = assimpScene->mAnimations[0];
    aiMatrix4x4 node_transform = p_node->mTransformation;

    const aiNodeAnim* node_anim = findNodeAnim(animation, node_name);

    if (node_anim)
    {

        //scaling
        //aiVector3D scaling_vector = node_anim->mScalingKeys[2].mValue;
        aiVector3D scaling_vector = calcInterpolatedScaling(p_animation_time, node_anim);
        aiMatrix4x4 scaling_matr;
        aiMatrix4x4::Scaling(scaling_vector, scaling_matr);

        //rotation
        //aiQuaternion rotate_quat = node_anim->mRotationKeys[2].mValue;
        aiQuaternion rotate_quat = calcInterpolatedRotation(p_animation_time, node_anim);
        aiMatrix4x4 rotate_matr = aiMatrix4x4(rotate_quat.GetMatrix());

        //translation
        //aiVector3D translate_vector = node_anim->mPositionKeys[2].mValue;
        aiVector3D translate_vector = calcInterpolatedPosition(p_animation_time, node_anim);
        aiMatrix4x4 translate_matr;
        aiMatrix4x4::Translation(translate_vector, translate_matr);

        if ( std::string(node_anim->mNodeName.data) == std::string("Head"))
        {
            aiQuaternion rotate_head = aiQuaternion(rotate_head_xz.w, rotate_head_xz.x, rotate_head_xz.y, rotate_head_xz.z);

            node_transform = translate_matr * (rotate_matr * aiMatrix4x4(rotate_head.GetMatrix())) * scaling_matr;
        }
        else
        {
            node_transform = translate_matr * rotate_matr * scaling_matr;
        }

    }

    aiMatrix4x4 global_transform = parent_transform * node_transform;


    if (m_BoneNameToIndexMap.find(node_name) != m_BoneNameToIndexMap.end()) // true if node_name exist in bone_mapping
    {
        uint bone_index = m_BoneNameToIndexMap[node_name];
        m_BoneTranformMatrixInfo[bone_index].FinalTransformation = m_GlobalInverseTransform * global_transform * m_BoneTranformMatrixInfo[bone_index].OffsetMatrix;
    }

    for (uint i = 0; i < p_node->mNumChildren; i++)
    {
        readNodeHierarchy(p_animation_time, p_node->mChildren[i], global_transform);
    }

}

void _Anim::boneTransform(double time_in_sec, std::vector<aiMatrix4x4> &transforms)
{
    aiMatrix4x4 identity_matrix; // = mat4(1.0f);

    float ticks_per_second = (float)(assimpScene->mAnimations[0]->mTicksPerSecond != 0 ? assimpScene->mAnimations[0]->mTicksPerSecond : 25.0f);
    double time_in_ticks = time_in_sec * ticks_per_second;
    float animation_time = fmod(time_in_ticks, assimpScene->mAnimations[0]->mDuration);


    readNodeHierarchy(animation_time, assimpScene->mRootNode, identity_matrix);

    transforms.resize(m_BoneTranformMatrixInfo.size());

    for (uint i = 0; i < m_BoneTranformMatrixInfo.size(); i++)
    {
        transforms[i] = m_BoneTranformMatrixInfo[i].FinalTransformation;
    }
}
