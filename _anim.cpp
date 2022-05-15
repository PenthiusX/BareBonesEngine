#include "_anim.h"

_Anim::_Anim(){
    this->_hasAnimation = false;
    this->assimpScene = nullptr;
}
//-------------------------------------------------------------------------------------------
_Anim::_Anim(std::string pathToModelFile)
{
    Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE, aiDefaultLogStream_STDOUT);

    importer = new Assimp::Importer;
    assimpScene = importer->ReadFile(pathToModelFile,
                                     aiProcess_LimitBoneWeights
                                     );

    if(assimpScene) {
        loadAssimpScene(assimpScene,pathToModelFile);
    }
    Assimp::DefaultLogger::kill();
}
//-------------------------------------------------------------------------------------------
_Anim::~_Anim(){
    delete importer;
}
//-------------------------------------------------------------------------------------------
/*
 *
 */
void _Anim::loadAssimpScene(const aiScene *scene, std::string path) {
    for(uint i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[i];
        if(mesh->HasBones())
        {
            this->_hasAnimation = true;
            for (int b = 0; b < mesh->mNumBones; b++)
            {
                int boneIndex = 0;
                std::string boneName = mesh->mBones[b]->mName.C_Str();

                if (this->m_BoneNameToIndexMap.find(boneName) == this->m_BoneNameToIndexMap.end()) {
                    // Allocate an index for a new bone
                    boneIndex = (int)this->m_BoneNameToIndexMap.size();
                    this->m_BoneNameToIndexMap[boneName] = boneIndex;
                }
                else {
                    boneIndex = this->m_BoneNameToIndexMap[boneName];
                }
                if (boneIndex == this->m_BoneTranformMatrixInfo.size()){
                    BoneTranformMatrixInfo bi(mesh->mBones[b]->mOffsetMatrix);
                    this->m_BoneTranformMatrixInfo.push_back(bi);
                }
            }
        }
        m_GlobalInverseTransform =  scene->mRootNode->mTransformation.Inverse();
    }
}
//-------------------------------------------------------------------------------------------
/*
 *
 */
aiQuaternion _Anim::nlerp(aiQuaternion a, aiQuaternion b, float blend) {

    a.Normalize();
    b.Normalize();

    aiQuaternion result;
    float dot_product = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    float one_minus_blend = 1.0f - blend;

    if (dot_product < 0.0f) {
        result.x = a.x * one_minus_blend + blend * -b.x;
        result.y = a.y * one_minus_blend + blend * -b.y;
        result.z = a.z * one_minus_blend + blend * -b.z;
        result.w = a.w * one_minus_blend + blend * -b.w;
    }
    else {
        result.x = a.x * one_minus_blend + blend * b.x;
        result.y = a.y * one_minus_blend + blend * b.y;
        result.z = a.z * one_minus_blend + blend * b.z;
        result.w = a.w * one_minus_blend + blend * b.w;
    }
    return result.Normalize();
}
//-------------------------------------------------------------------------------------------
/*
 *
 */
uint _Anim::findPosition(float p_animation_time, const aiNodeAnim *p_node_anim) {
    for (uint i = 0; i < p_node_anim->mNumPositionKeys - 1; i++){
        if (p_animation_time < (float)p_node_anim->mPositionKeys[i + 1].mTime){
            return i;
        }
    }
    assert(0);
    return 0;
}
//-------------------------------------------------------------------------------------------
/*
 *
 */
uint _Anim::findRotation(float p_animation_time, const aiNodeAnim *p_node_anim) {
    for (uint i = 0; i < p_node_anim->mNumRotationKeys - 1; i++) {
        if (p_animation_time < (float)p_node_anim->mRotationKeys[i + 1].mTime) {
            return i;
        }
    }
    assert(0);
    return 0;
}
//-------------------------------------------------------------------------------------------
/*
 *
 */
uint _Anim::findScaling(float p_animation_time, const aiNodeAnim *p_node_anim)
{
    for (uint i = 0; i < p_node_anim->mNumScalingKeys - 1; i++) {
        if (p_animation_time < (float)p_node_anim->mScalingKeys[i + 1].mTime) {
            return i;
        }
    }
    assert(0);
    return 0;
}
//-------------------------------------------------------------------------------------------
/*
 *
 */
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
//-------------------------------------------------------------------------------------------
/*
 *
 */
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

    assert(factor >= 0.0f && factor <= 1.0f);
    aiQuaternion start_quat = p_node_anim->mRotationKeys[rotation_index].mValue;
    aiQuaternion end_quat = p_node_anim->mRotationKeys[next_rotation_index].mValue;

    return nlerp(start_quat, end_quat, factor);
}
//-------------------------------------------------------------------------------------------
/*
 *
 */
aiVector3D _Anim::calcInterpolatedScaling(float p_animation_time, const aiNodeAnim *p_node_anim)
{
    if (p_node_anim->mNumScalingKeys == 1)
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
//-------------------------------------------------------------------------------------------
/*
 *
 */
const aiNodeAnim *_Anim::findNodeAnim(const aiAnimation *p_animation, const std::string p_node_name)
{
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
//-------------------------------------------------------------------------------------------
/*
 *
 */
void _Anim::readNodeHierarchy(float p_animation_time, const aiNode *p_node, const aiMatrix4x4 parent_transform)
{
    std::string node_name = p_node->mName.data;

    const aiAnimation* animation = assimpScene->mAnimations[0];
    aiMatrix4x4 node_transform = p_node->mTransformation;

    const aiNodeAnim* node_anim = findNodeAnim(animation, node_name);

    if (node_anim) {
        //scaling
        aiVector3D scaling_vector = calcInterpolatedScaling(p_animation_time, node_anim);
        aiMatrix4x4 scaling_matr;
        aiMatrix4x4::Scaling(scaling_vector, scaling_matr);

        //rotation
        aiQuaternion rotate_quat = calcInterpolatedRotation(p_animation_time, node_anim);
        aiMatrix4x4 rotate_matr = aiMatrix4x4(rotate_quat.GetMatrix());

        //translation
        aiVector3D translate_vector = calcInterpolatedPosition(p_animation_time, node_anim);
        aiMatrix4x4 translate_matr;
        aiMatrix4x4::Translation(translate_vector, translate_matr);

        if ( std::string(node_anim->mNodeName.data) == "Head") {
            //this rotation will additively add rotational transformation on the bone that is named head.
            rotate_head_xz = glm::quat(cos(glm::radians(100.0f)), sin(glm::radians(0.0f)) * glm::vec3(1.0f, 0.0f, 0.0f));
            aiQuaternion rotate_head = aiQuaternion(rotate_head_xz.w, rotate_head_xz.x, rotate_head_xz.y, rotate_head_xz.z);
            node_transform = translate_matr * (rotate_matr * aiMatrix4x4(rotate_head.GetMatrix())) * scaling_matr;
        }
        else {
            node_transform = translate_matr * rotate_matr * scaling_matr;
        }
    }

    aiMatrix4x4 global_transform = parent_transform * node_transform;

    if (m_BoneNameToIndexMap.find(node_name) != m_BoneNameToIndexMap.end()) {
        uint bone_index = m_BoneNameToIndexMap[node_name];
        m_BoneTranformMatrixInfo[bone_index].FinalTransformation = m_GlobalInverseTransform * global_transform * m_BoneTranformMatrixInfo[bone_index].OffsetMatrix;
    }
    for (uint i = 0; i < p_node->mNumChildren; i++) {
        readNodeHierarchy(p_animation_time, p_node->mChildren[i], global_transform);//Traverse the tree.
    }
}
//-------------------------------------------------------------------------------------------
/*
 *
 */
void _Anim::boneTransform(double time_in_sec, std::vector<aiMatrix4x4> &transforms)
{
    aiMatrix4x4 identity_matrix; // = mat4(1.0f);

    float ticks_per_second = assimpScene->mAnimations[0]->mTicksPerSecond;;//(float)(assimpScene->mAnimations[0]->mTicksPerSecond != 0 ? assimpScene->mAnimations[0]->mTicksPerSecond : 25.0f);
    double time_in_ticks = time_in_sec * ticks_per_second;
    float animation_time = fmod(time_in_ticks, assimpScene->mAnimations[0]->mDuration);

    this->readNodeHierarchy(animation_time, assimpScene->mRootNode, identity_matrix);

    transforms.resize(m_BoneTranformMatrixInfo.size());

    for (uint i = 0; i < m_BoneTranformMatrixInfo.size(); i++){
        transforms[i] = m_BoneTranformMatrixInfo[i].FinalTransformation;
    }
}

bool _Anim::hasAnimations() const{
    return this->_hasAnimation;
}
//-------------------------------------------------------------------------------------------
