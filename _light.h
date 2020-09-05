#ifndef _LIGHT_H
#define _LIGHT_H

#include <glm/glm.hpp>
#include <vector>


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
   virtual char* getSignature() = 0;
   virtual void setPosition(glm::vec3 pos) = 0;
   virtual void setDiffuse(glm::vec3 col) = 0;

   //Pending: implement exception handeling  due to abiguity of data
   virtual void setLightParams2V3(glm::vec3 a, glm::vec3 b) = 0;
   virtual void setLightParams3V3(glm::vec3 a, glm::vec3 b, glm::vec3 c) = 0;
   virtual std::vector<glm::vec3> getLightParams() = 0;
};

class _DirLight : public I_Light
{
public:
    _DirLight(char* sig){};
    virtual ~_DirLight(){};

    char* getSignature();
    void setPosition(glm::vec3 pos);
    void setDiffuse(glm::vec3 diff);
    void setLightParams2V3(glm::vec3 a, glm::vec3 b);
    void setLightParams3V3(glm::vec3 a, glm::vec3 b, glm::vec3 c){};//no def
    std::vector<glm::vec3> getLightParams();

private:
    char* sig;
    glm::vec3 position;
    glm::vec3 diffuse;
    glm::vec3 ambient;
    glm::vec3 specular;
};

class _PointLight : public I_Light{

public:

    _PointLight(char* sig){};
    virtual ~_PointLight();

    char* getSignature();
    void setPosition(glm::vec3 pos);
    void setDiffuse(glm::vec3 diff);
    void setLightParams2V3(glm::vec3 a, glm::vec3 b);//nodef
    void setLightParams3V3(glm::vec3 a, glm::vec3 b, glm::vec3 c);
    std::vector<glm::vec3> getLightParams();

private:
    char* sig;
    glm::vec3 position;
    glm::vec3 diffuse;
    float constant;
    float linear;
    float quadratic;
};

class _SpotLight : public I_Light{

public:
    _SpotLight(char* sig){};
    virtual ~_SpotLight(){};

    char* getSignature();
    void setPosition(glm::vec3 pos);
    void setDiffuse(glm::vec3 diff);
    void setLightParams2V3(glm::vec3 a, glm::vec3 b);
    void setLightParams3V3(glm::vec3 a, glm::vec3 b, glm::vec3 c){};//no def
    std::vector<glm::vec3> getLightParams();

private:
    char* sig;
    glm::vec3 position;
    glm::vec3 diffuse;
    glm::vec3 direction;
    float cutOff;
};
#endif // _LIGHT_H
