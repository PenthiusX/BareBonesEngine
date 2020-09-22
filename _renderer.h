#pragma once

#include <vector>
#include "_shader.h"
#include "_sceneentity.h"
#include "_texture.h"
#include "_light.h"
#include "_material.h"
//
#include <qopenglextrafunctions.h>
#include <qelapsedtimer.h>//for timer
//GLM math Library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <qopengl.h>
/*
 * Class: _Renderer
 * To create an abstraction for randering data
 * Extends the _glwidget class, will be included in the _glwidget class
*/

class _Renderer : protected QOpenGLExtraFunctions
{
public:
    _Renderer();
    ~_Renderer();
    //
    void setCamViewMatrix(glm::vec3 eyePos, glm::vec3 focalPoint, glm::vec3 upVector);// Sets the Camera matrix
    void setProjectionMatrix(int resW, int resH, float fov, float zFar, float zNear);// Sets the projection matrix
    void setOrthoProjectionMatrix(float left, float right, float bottom, float top, float zNear, float zFar);
    void setProjectionMatrix(int type ,glm::mat4x4 m);
    //Transformantions
    void setPosition(glm::vec3 pos);// Resets the positon to the value that is required
    void translate(glm::vec3 pos);// Update the current positon with the value that is set
    void setRotation(glm::vec3 rot);
    void setRotationAroundPivot(glm::vec3 rot, glm::vec3 pivot);
    void setscale(float scale);
    void lookAt(QVector3D ptl);
    void RotationBetweenVectors(glm::vec3 dest);
    //Scene Entity
    void initSceneEntityInRenderer(_SceneEntity s_);
    void setSceneEntityInRenderer(_SceneEntity s);
    _SceneEntity getSceneEntity() const;
    //
    void draw();// Draws paints everything bound in the scene
    //Lights
    void updateLightUniforms(std::vector<I_Light*> il);
    //Frame Enablements
    void setGLEnablements();

private:
    QVector4D actualColor;
    uint VBO;//vertex buffer objec
    uint VAO;//attribute buffer object
    uint EBO;//index buffer object
    //Frambuffer variables
    GLuint frameBuffer1;
    uint fbtexture,textureColorbuffer,rbo;
    uint quadVAO,quadVBO;
    _Shader *fboShader;
    //Matrices for Translation and view will be multiplied with the position to set translation rotaion ,scaling witrespect to view.
    glm::mat4 modelMatrix;
    glm::mat4 rotationMatrix,translationMatrix,scalingMatrix;
    glm::mat4 pivotTmat;
    glm::mat4 projectionMatrix,orthoProjMatrix;
    glm::mat4 viewMatrix;
    //
    _SceneEntity sceneEntity;//the local sceneEntity object for use in the renderer
    QElapsedTimer timer;//timer object to maintain a timer based events and trasformations
    glm::vec3 camposForLight;
    glm::vec3 focalPoint;
    QElapsedTimer qtimer;
    //
    void setModelMatrix(glm::vec3 position, float scale, glm::vec3 rotation);//set the model matrix
    void setModelDataInBuffers(std::vector<float>vertexArray,std::vector<uint> indexArray);//take vertex and index data and binds it to object buffer
    void setModelDataInBuffers(std::vector<VertexInfo>vertexInfoArray,std::vector<uint> indexArray);
    //void setModelDataInBuffers(std::vector<float>vertexArray,std::vector<float> normalsArray ,std::vector<uint> indexArray);//take vertex and index data and binds it to object buffer
    //Holds the vertex and index data
    std::vector<uint> indices;
    //
    std::vector<_Texture> textures;//Texture array for tetures in use for the respective renderer object
    void setTexture(QString pathtoTexture);
    void setTexture(char* texBitmap);//takes am image and binds it to object
    void setTexture(char* texBitmap,uint iwidth,uint iheight);//takes am image and binds it to object
    void setupTexture();//takes a hardcoded imagedata and binds it to object
    void setupTexture(QString texfile,_Texture::Type t); //initializes texture from file
    //
    void keepSceneEntityUpdated();
    void updateColorUniforms();
    void updateMaterial(_Material m);
    //
    //Shader class object sets the shaders and passes the program to the current context
    _Shader* shdr,shdr2;//shader2 is for utility only
    void setShader();//default shatder to load
    void setShader(QString vertexShader, QString fragmentShader);//takes a string literal and passes
};
