#pragma once

#include <glm/glm.hpp>
#include <_texture.h>

class _Material
{
public:
    _Material();
    ~_Material();

    void setDiffuseTexture(_Texture* t);
    void setSpecularTexture();//pending
    void setBumpTexture();//pending

private:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    _Texture* diffuseTexture;
    _Texture* specularTexure;
};
