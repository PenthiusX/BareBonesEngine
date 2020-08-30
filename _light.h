#ifndef _LIGHT_H
#define _LIGHT_H

#include <glm/glm.hpp>


class _Light
{
public:

    _Light();
    _Light(glm::vec3 position,glm::vec4 color,float ambient,float specular,float diffuse);

    ~_Light();

    glm::vec3 getPosition();
    glm::vec4 getColor();
    float getAmbientStr();
    float getSpecular();
    float getDiffuse();

    void setPosition(glm::vec3 pos);
    void setColor(glm::vec4 color);
    void setAmbientStr(float as);
    void setSpecular(float ss);
    void setDiffuse(float d);

private:
    glm::vec3 position;
    glm::vec4 color;
    float ambientStr;
    float diffuse;
    float specular;
};

#endif // _LIGHT_H
