#pragma once

#include <glm/glm.hpp>
#include <qstring.h>


class _Material
{
public:
    _Material();
    ~_Material();

    void setDiffuseTexture(QString path);
    void setSpecularTexture(QString path);//pending
    void setBumpTexture(QString path);//pending
    QString getDiffuseTexture();
    QString getSpecualrTexture();
    //
    void setAmbient(glm::vec3 amb);
    void setDiffuse(glm::vec3 diff);
    void setSpecular(glm::vec3 spec);
    void setShine(float sh);

    glm::vec3 getAmbient();
    glm::vec3 getDiffuse();
    glm::vec3 getSpecular();
    float getShine();

private:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    QString diffusePath;
    QString specularPath;
    QString bumpTexture;
};
