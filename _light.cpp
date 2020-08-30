#include "_light.h"

_Light::_Light(){}

_Light::_Light(glm::vec3 position,glm::vec4 color,float ambient,float specular,float diffuse){
    this->color = color;
    this->position = position;
    this->ambientStr = ambient;
    this->specular = specular;
    this->diffuse = diffuse;
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
float _Light::getSpecular(){
    return this->specular;
}

float _Light::getDiffuse(){
    return this->diffuse;
}
void _Light::setPosition(glm::vec3 pos){
    this->position = pos;
}
void _Light::setAmbientStr(float as){
    this->ambientStr = as;
}
void _Light::setSpecular(float ss){
    this->specular = ss;
}
void _Light::setDiffuse(float d){
    this->diffuse = d;
}
