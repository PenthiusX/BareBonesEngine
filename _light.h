#ifndef _LIGHT_H
#define _LIGHT_H

#include <glm/glm.hpp>


class _Light
{
public:

    _Light();
    _Light(glm::vec3 position,glm::vec4 color,float ambientStr,float specularStr);

    ~_Light();

    glm::vec3 getPosition();
    glm::vec4 getColor();
    float getAmbientStr();
    float getSpecularStr();

    void setPosition(glm::vec3 pos);
    void setColor(glm::vec4 color);
    void setAmbientStr(float as);
    void setSpecularStr(float ss);

private:
    glm::vec3 position;
    glm::vec4 color;
    float ambientStr;
    float specularStr;
};

#endif // _LIGHT_H
