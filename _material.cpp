#include "_material.h"

_Material::_Material()
{

}

_Material::~_Material()
{

}

void _Material::setDiffuseTexture(_Texture* t)
{
    this->diffuseTexture = t;
}
