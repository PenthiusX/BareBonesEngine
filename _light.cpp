#include "_light.h"

_Light::_Light(){}

_Light::_Light(glm::vec3 position,glm::vec4 color,float ambientStr,float specularStr){
    this->color = color;
    this->position = position;
    this->ambientStr = ambientStr;
    this->specularStr = specularStr;
}
_Light::~_Light(){}
glm::vec3 _Light::getPosition(){
   return this->position;
}
glm::vec4 _Light::getColor(){
    return this->color;
}
float _Light::getAmbientStr(){
    return this->ambientStr;
}
float _Light::getSpecularStr(){
    return this->specularStr;
}
void _Light::setPosition(glm::vec3 pos){
    this->position = pos;
}
void _Light::setAmbientStr(float as){
    this->ambientStr = as;
}
void _Light::setSpecularStr(float ss){
    this->specularStr = ss;
}
