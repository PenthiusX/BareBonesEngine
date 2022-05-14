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
