#ifndef LIGHTTEST_H
#define LIGHTTEST_H


#include <vector>
#include "_shader.h"
//
#include <qopenglextrafunctions.h>
//GLM math Library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>


class LightTest : protected QOpenGLExtraFunctions {

public:
    LightTest();
    ~LightTest();

    std::vector<float>vertices;


    void setModelMatrix(glm::vec3 position,float scale,glm::vec3 rotation);

    void setCamViewMatrix(QVector3D eyePos,glm::vec3 focalPoint,QVector3D upVector);
    void setProjectionMatrix(int resW, int resH, float fov, float zNear, float zFar);
    void initLightBody();
    void draw();

   private:
    // lighting
    glm::vec3 lightPos;
    unsigned int VBO, cubeVAO;
    _Shader* s;

    glm::mat4 modelMatrix;
    glm::mat4 rotationMatrix,translationMatrix,scalingMatrix;
    glm::mat4 pivotTmat;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
};


#endif // LIGHTTEST_H
