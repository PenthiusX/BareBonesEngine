#include "_modelinfo.h"

_ModelInfo::_ModelInfo(){
}

void _ModelInfo::setName(QString name){
    this->name = name;
}
void _ModelInfo::setPath(QString path){
    this->path = path;
}
void _ModelInfo::setIsLoaded(bool isLoaded){
    this->isLoaded = isLoaded;
}
void _ModelInfo::setVertexArray(std::vector<float> vertexArray){
    this->vertexArray = vertexArray;
}
void _ModelInfo::setIndexArray(std::vector<unsigned int> indexAarray){
    this->indexAarray = indexAarray;
}
void _ModelInfo::setCentroid(glm::vec4 cent){
    this->centroid = cent;
}
void _ModelInfo::setMaxExtents(glm::vec4 max){
    this->max = max;
}
void _ModelInfo::setMinExtents(glm::vec4 min){
    this->min = min;
}
QString _ModelInfo::getName() const{
    return this->name;
}
QString _ModelInfo::getPath() const{
    return this->path;
}
std::vector<float> _ModelInfo::getVerticexArray() const{
    return this->vertexArray;
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
    return this->centroid;
}
bool _ModelInfo::getIsLoaded() const {
    return this->isLoaded;
}
