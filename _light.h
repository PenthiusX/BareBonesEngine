#ifndef _LIGHT_H
#define _LIGHT_H

#include <glm/glm.hpp>
#include <vector>
#include <string>


class _Light
{
public:

    _Light();
    _Light(glm::vec3 position,glm::vec4 color,float ambient,float specular,float diffuse);

    ~_Light();

    glm::vec3 getPosition();
    glm::vec4 getColor();
    float getSpecular();
    float getDiffuse();

    void setPosition(glm::vec3 pos);
    void setColor(glm::vec4 color);
    void setSpecular(float ss);
    void setDiffuse(float d);

private:
    glm::vec3 position;
    glm::vec4 color;
    float ambientStr;
    float diffuse;
    float specular;
};


class I_Light
{
public:
   virtual ~I_Light();
   virtual std::string getSignature() = 0;
   virtual std::string getLightType() = 0;
   virtual void setPosition(glm::vec3 pos) = 0;
   virtual void setAmbDefSpec(glm::vec3 a, glm::vec3 b, glm::vec3 c) = 0;
   virtual void setAdditonalParams2x3(glm::vec3 a, glm::vec3 b) = 0;
   virtual void setAdditonalParams3x3(glm::vec3 a, glm::vec3 b, glm::vec3 c) = 0;
   virtual std::vector<glm::vec3> getLightParams() = 0;
};

class _DirLight : public I_Light
{
public:
    _DirLight(std::string sig){this->sig = sig;};
    virtual ~_DirLight(){};

    std::string getSignature();
    std::string getLightType();
    void setPosition(glm::vec3 pos);
    void setAmbDefSpec(glm::vec3 a, glm::vec3 b, glm::vec3 c);
    void setAdditonalParams2x3(glm::vec3 a, glm::vec3 b){}//nodef
    void setAdditonalParams3x3(glm::vec3 a, glm::vec3 b, glm::vec3 c){}//no def
    std::vector<glm::vec3> getLightParams();

private:
    std::string sig;
    glm::vec3 position;
    glm::vec3 diffuse;
    glm::vec3 ambient;
    glm::vec3 specular;
    const std::string name = "DirLight";
};

class _PointLight : public I_Light
{

public:

    _PointLight(std::string sig){this->sig = sig;};
    virtual ~_PointLight(){};

    std::string getSignature();
    std::string getLightType();
    void setPosition(glm::vec3 pos);
    void setAmbDefSpec(glm::vec3 a, glm::vec3 b, glm::vec3 c);
    void setAdditonalParams2x3(glm::vec3 a, glm::vec3 b){};//nodef
    void setAdditonalParams3x3(glm::vec3 a, glm::vec3 b, glm::vec3 c);
    std::vector<glm::vec3> getLightParams();

private:
    std::string sig;
    glm::vec3 position;

    glm::vec3 diffuse;
    glm::vec3 ambient;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;
    const std::string name = "PointLight";
};

class _SpotLight : public I_Light{

public:
    _SpotLight(std::string sig){this->sig = sig;};
    virtual ~_SpotLight(){};

    std::string getSignature();
    std::string getLightType();
    void setPosition(glm::vec3 pos);
    void setAmbDefSpec(glm::vec3 a, glm::vec3 b, glm::vec3 c);
    void setAdditonalParams2x3(glm::vec3 a, glm::vec3 b);
    void setAdditonalParams3x3(glm::vec3 a, glm::vec3 b, glm::vec3 c){};//no def
    std::vector<glm::vec3> getLightParams();

private:
    std::string sig;
    glm::vec3 position;

    glm::vec3 diffuse;
    glm::vec3 ambient;
    glm::vec3 specular;

    glm::vec3 direction;
    float cutOff;
    float outerCuttof;
    const std::string name = "SpotLight";
};
#endif // _LIGHT_H
