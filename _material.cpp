#include "_material.h"

_Material::_Material(){
}

_Material::~_Material(){

}

void _Material::setDiffuseTexture(QString path){
    this->diffusePath = path;
}

void _Material::setSpecularTexture(QString path)
{
    this->specularPath = path;
}

QString _Material::getDiffuseTexture()
{
    return this->diffusePath;
}
QString _Material::getSpecualrTexture()
{
    return this->specularPath;
}

void _Material::setAmbient(glm::vec3 amb)
{
   this->ambient = amb;
}

void _Material::setDiffuse(glm::vec3 diff)
{
    this->diffuse = diff;
}

void _Material::setSpecular(glm::vec3 spec)
{
    this->specular = spec;
}

void _Material::setShine(float sh)
{
    this->shininess = sh;
}

glm::vec3 _Material::getAmbient()
{
    return this->ambient;
}

glm::vec3 _Material::getDiffuse()
{
    return this->diffuse;
}

glm::vec3 _Material::getSpecular()
{
    return this->specular;
}

float _Material::getShine()
{
    return this->shininess;
}

void _Material::setShaders(QString v, QString f)
{
   shaderCont sc;
   sc.vshader = v;
   sc.fshader = f;
   shadersC.push_back(sc);
}

std::vector<_Material::shaderCont> _Material::getShaders()
{
    return shadersC;
}
