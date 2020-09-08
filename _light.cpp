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
float _Light::getSpecular(){
    return this->specular;
}

float _Light::getDiffuse(){
    return this->diffuse;
}
void _Light::setPosition(glm::vec3 pos){
    this->position = pos;
}
void _Light::setSpecular(float ss){
    this->specular = ss;
}
void _Light::setDiffuse(float d){
    this->diffuse = d;
}


//_DirLight::_DirLight(){}
//_DirLight::~_DirLight(){}

std::string _DirLight::getSignature()
{
    return sig;
}

void _DirLight::setPosition(glm::vec3 pos){
    this->position = pos;
}

void _DirLight::setAmbDefSpec(glm::vec3 a, glm::vec3 b, glm::vec3 c){
    this->diffuse = a;
    this->ambient = b;
    this->specular = c;
}
std::vector<glm::vec3> _DirLight::getLightParams(){
    std::vector<glm::vec3> t;
    t.push_back(ambient);
    t.push_back(specular);
}
//_PointLight::_PointLight(){}
//_PointLight::~_PointLight(){}

std::string _PointLight::getSignature(){
    return sig;
}
void _PointLight::setPosition(glm::vec3 pos){
    position = pos;
}
void _PointLight::setAmbDefSpec(glm::vec3 a, glm::vec3 b, glm::vec3 c){
    this->diffuse = a;
    this->ambient = b;
    this->specular = c;
}
void _PointLight::setAdditonalParams3x3(glm::vec3 a, glm::vec3 b, glm::vec3 c){
    this->constant = (a.x + a.y + a.z)/3;
    this->linear = (b.x + b.y + b.z)/3;
    this->quadratic = (c.x + c.y + c.z)/3;
}
std::vector<glm::vec3> _PointLight::getLightParams(){
    std::vector<glm::vec3> t;
    t.push_back(glm::vec3(constant));
    t.push_back(glm::vec3(linear));
    t.push_back(glm::vec3(quadratic));
}
//_SpotLight::_SpotLight(){}
//_SpotLight::~_SpotLight(){}

std::string _SpotLight::getSignature(){
    return sig;
}
void _SpotLight::setPosition(glm::vec3 pos){
    position = pos;
}
void _SpotLight::setAmbDefSpec(glm::vec3 a, glm::vec3 b, glm::vec3 c){
    this->diffuse = a;
    this->ambient = b;
    this->specular = c;
}
void _SpotLight::setAdditonalParams2x3(glm::vec3 a, glm::vec3 b){
    direction = a;
    cutOff = b.x;
    outerCuttof = b.y;
}
std::vector<glm::vec3> _SpotLight::getLightParams(){
    std::vector<glm::vec3> t;
    t.push_back(direction);
    t.push_back(glm::vec3(cutOff,cutOff,cutOff));
}
I_Light::~I_Light(){}
