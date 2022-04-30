#include "_modelinfo.h"

_ModelInfo::_ModelInfo(){
}

_ModelInfo::~_ModelInfo(){

}

//Setters
void _ModelInfo::setName(QString name){
    this->name = name;
}
void _ModelInfo::setPath(QString path) {
    this->path = path;
}

void _ModelInfo::setIsLoaded(bool isLoaded){
    this->isLoaded = isLoaded;
}
void _ModelInfo::setVertexArray(std::vector<float> vertexArray){
    this->vertexArray = vertexArray;
}
void _ModelInfo::setVertexInfoArray(std::vector<VertexInfo> vertexInfoArray){
    this->vertexInfoArray = vertexInfoArray;
}
void _ModelInfo::setNormalArray(std::vector<float> norArray){
    this->normalArray = norArray;
}
void _ModelInfo::setIndexArray(std::vector<unsigned int> indexAarray){
    this->indexAarray = indexAarray;
}
void _ModelInfo::setCentroid(glm::vec4 cent){
    this->cent = cent;
}
void _ModelInfo::setMaxExtents(glm::vec4 max){
    this->max = max;
}
void _ModelInfo::setMinExtents(glm::vec4 min){
    this->min = min;
}

//Getters
QString _ModelInfo::getName() const{
    return this->name;
}
QString _ModelInfo::getPath() const{
    return this->path;
}
std::vector<float> _ModelInfo::getVertexArray() const{
    return this->vertexArray;
}
std::vector<VertexInfo> _ModelInfo::getVertexInfoArray()const{
    return this->vertexInfoArray;
}
std::vector<float> _ModelInfo::getNormalArray() const{
    return this->normalArray;
}
std::vector<unsigned int> _ModelInfo::getIndexArray() const{
    return this->indexAarray;
}
glm::vec4 _ModelInfo::getMaxExtent() const{
    return this->max;
}
glm::vec4 _ModelInfo::getMinExtent() const{
    return this->min;
}
glm::vec4 _ModelInfo::getCentroid() const{
    return this->cent;
}
bool _ModelInfo::getIsLoaded() const{
    return this->isLoaded;
}

/*
*/
void _ModelInfo::calcMinMaxExtents(){
  if(vertexInfoArray.size() != 0){
    for(unsigned int i = 0 ; i < vertexInfoArray.size(); i++)
    {
        glm::vec3 p = vertexInfoArray[i].Position;
        //maxEntent
        if(p.x >= max.x)
            max.x=(p.x);
        if(p.y >= max.y)
            max.y=(p.y);
        if(p.z >= max.z)
            max.z=(p.z);
        max.w = 0.0f;

        //min extent
        if(p.x <= min.x)
            min.x=(p.x);
        if(p.y <= min.y)
            min.y=(p.y);
        if(p.z <= min.z)
            min.z=(p.z);
        min.w = 0.0f;
    }
  }
  if(vertexArray.size() != 0)
  {
      std::vector<float> v = vertexArray;
      for(unsigned int i = 0 ; i < v.size() ; i += 3)
      {
          //maxEntent
          if(v[i] >= max.x)
              max.x=(v[i]);
          if(v[i + 1] >= max.y)
              max.y=(v[i + 1]);
          if(v[i + 2] >= max.z)
              max.z=(v[i + 2]);
          max.w = 0.0f;
          //min extent
          if(v[i] <= min.x)
              min.x=(v[i]);
          if(v[i + 1] <= min.y)
              min.y=(v[i + 1]);
          if(v[i + 2] <= min.z)
              min.z=(v[i + 2]);
          min.w = 0.0f;
      }
  }
    cent =  calcCentroidFromMinMax();
}
/*
*/
glm::vec4 _ModelInfo::calcCentroidFromMinMax(){
    glm::vec4 centroid;
    centroid.x = (max.x + min.x)*0.5f;
    centroid.y = (max.y + min.y)*0.5f;
    centroid.z = (max.z + min.z)*0.5f;
    centroid.w = 0.0f;
    cent = centroid;
    return centroid;
}
glm::vec4 _ModelInfo::calcCentroidFromMinMax(glm::vec3 min, glm::vec3 max){
    glm::vec4 centroid;
    centroid.x = (max.x + min.x)*0.5f;
    centroid.y = (max.y + min.y)*0.5f;
    centroid.z = (max.z + min.z)*0.5f;
    centroid.w = 0.0f;
    cent = centroid;
    return centroid;
}


//Animation//Aditya WIP-----------------------------------------------------

//uint _ModelInfo::FindPosition(float AnimationTimeTicks, const aiNodeAnim *pNodeAnim)
//{
//    for (uint i = 0 ; i < pNodeAnim->mNumPositionKeys - 1 ; i++) {
//        float t = (float)pNodeAnim->mPositionKeys[i + 1].mTime;
//        if (AnimationTimeTicks < t) {
//            return i;
//        }
//    }

//    return 0;
//}

//void _ModelInfo::CalcInterpolatedPosition(aiVector3D &Out, float AnimationTimeTicks, const aiNodeAnim *pNodeAnim)
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

//uint _ModelInfo::FindRotation(float AnimationTimeTicks, const aiNodeAnim *pNodeAnim)
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

//void _ModelInfo::CalcInterpolatedRotation(aiQuaternion &Out, float AnimationTimeTicks, const aiNodeAnim *pNodeAnim)
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

//uint _ModelInfo::FindScaling(float AnimationTimeTicks, const aiNodeAnim *pNodeAnim)
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

//void _ModelInfo::CalcInterpolatedScaling(aiVector3D &Out, float AnimationTimeTicks, const aiNodeAnim *pNodeAnim)
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

//void _ModelInfo::ReadNodeHierarchy(float AnimationTimeTicks, const aiNode *pNode, const glm::mat4x4 &ParentTransform,)
//{
//    std::string NodeName(pNode->mName.data);

//    const aiAnimation* pAnimation = assimpScene->mAnimations[0];

//    glm::mat4x4 NodeTransformation = mat4_cast(pNode->mTransformation);

//    const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

//    if (pNodeAnim) {
//        // Interpolate scaling and generate scaling transformation matrix
//        aiVector3D Scaling;
//        CalcInterpolatedScaling(Scaling, AnimationTimeTicks, pNodeAnim);
//         glm::mat4x4 ScalingM = glm::scale(ScalingM, glm::vec3(Scaling.x, Scaling.y, Scaling.z));//scale equally on all sides

//        // Interpolate rotation and generate rotation transformation matrix
//        aiQuaternion RotationQ;
//        CalcInterpolatedRotation(RotationQ, AnimationTimeTicks, pNodeAnim);
//         glm::mat4x4 RotationM = mat4_cast(RotationQ.GetMatrix());

//        // Interpolate translation and generate translation transformation matrix
//        aiVector3D Translation;
//        CalcInterpolatedPosition(Translation, AnimationTimeTicks, pNodeAnim);
//         glm::mat4x4 TranslationM;
//        TranslationM = glm::translate(TranslationM,glm::vec3(Translation.x, Translation.y, Translation.z));

//        // Combine the above transformations
//        NodeTransformation = TranslationM * RotationM * ScalingM;
//    }

//    glm::mat4x4 GlobalTransformation = ParentTransform * NodeTransformation;

//    if (m_BoneNameToIndexMap.find(NodeName) != m_BoneNameToIndexMap.end()) {
//        uint BoneIndex = m_BoneNameToIndexMap[NodeName];
//        m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].OffsetMatrix;
//    }

//    for (uint i = 0 ; i < pNode->mNumChildren ; i++) {
//        ReadNodeHierarchy(AnimationTimeTicks, pNode->mChildren[i], GlobalTransformation);
//    }
//}

//void _ModelInfo::GetBoneTransforms(float TimeInSeconds, std::vector<glm::mat4x4> &Transforms)
//{
//    glm::mat4x4 Identity;
//    Identity = glm::mat4(1.f);

//    float TicksPerSecond = (float)(assimpScene->mAnimations[0]->mTicksPerSecond != 0 ? assimpScene->mAnimations[0]->mTicksPerSecond : 25.0f);
//    float TimeInTicks = TimeInSeconds * TicksPerSecond;
//    float AnimationTimeTicks = fmod(TimeInTicks, (float)assimpScene->mAnimations[0]->mDuration);

//    ReadNodeHierarchy(AnimationTimeTicks, assimpScene->mRootNode, Identity);
//    Transforms.resize(m_BoneInfo.size());

//    for (uint i = 0 ; i < m_BoneInfo.size() ; i++) {
//        Transforms[i] = m_BoneInfo[i].FinalTransformation;
//    }
//}

//const aiNodeAnim *_ModelInfo::FindNodeAnim(const aiAnimation *pAnimation, const std::string &NodeName)
//{
//    for (uint i = 0 ; i < pAnimation->mNumChannels ; i++) {
//        const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

//        if (std::string(pNodeAnim->mNodeName.data) == NodeName) {
//            return pNodeAnim;
//        }
//    }

//    return NULL;
//}
