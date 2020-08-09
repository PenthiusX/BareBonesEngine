#ifndef _RENDERER_H
#define _RENDERER_H
#include <vector>
#include "_shader.h"
#include "_sceneentity.h"
#include "_texture.h"
//
#include <qopenglextrafunctions.h>
#include <qelapsedtimer.h>//for timer
//GLM math Library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#if defined(Q_OS_WIN)
     //#include "_ObjLoader.h"//use external objLoader.
#endif

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

    void setShader();//default shatder to load
    void setShader(QString vertexShader, QString fragmentShader);//takes a string literal and passes
    void setModelDataInBuffers(std::vector<float>vertexArray,std::vector<uint> indexArray);//take vertex and index data and binds it to object buffer
    void setModelDataInBuffers(std::vector<VertexInfo>vertexArray,std::vector<uint> indexArra);
    void setModelDataInBuffers(std::vector<float>vertexArray,std::vector<float> normalsArray ,std::vector<uint> indexArray);//take vertex and index data and binds it to object buffer
#if defined(Q_OS_WIN)
    //void setModelDataInBuffers(objl::Loader LoaderObject);
#endif
    //
    void setTexture(QString pathtoTexture);
    void setTexture(char* texBitmap);//takes am image and binds it to object
    void setTexture(char* texBitmap,uint iwidth,uint iheight);//takes am image and binds it to object
    //
    void setupTexture();//takes a hardcoded imagedata and binds it to object
    void setupTexture(QString texfile); //initializes texture from file
    //
    void setModelMatrix(glm::vec3 position, float scale, glm::vec3 rotation);//set the model matrix
    void setCamViewMatrix(QVector3D eyePos, glm::vec3 focalPoint, QVector3D upVector);//sets the Camera matrix
    void setProjectionMatrix(int resW, int resH, float fov, float zFar, float zNear);//sets the projection matrix
    //
    void setPosition(glm::vec3 pos);//resets the positon to the value that is required
    void translate(glm::vec3 pos);//update the current positon with the value that is set
    void setRotation(glm::vec3 rot);
    void setRotationAroundPivot(glm::vec3 rot, glm::vec3 pivot);
    void setscale(float scale);
    void lookAt(QVector3D ptl);
    //
    void initSceneEntityInRenderer(_SceneEntity s_);
    void setSceneEntityInRenderer(_SceneEntity s);
    //
    _SceneEntity getSceneEntity() const;
    void draw();//Draws/paints everything bound in the scene

    void RotationBetweenVectors(glm::vec3 dest);
private:
    QVector4D actualColor;
    uint VBO;//vertex buffer object
    uint VAO;//attribute buffer object
    uint EBO;//index buffer object
    //Shader class object sets the shaders and passes the program to the current context
    _Shader* shdr;
    GLint colorUniform;
    GLint mvpUniform;
    GLint modelUnifrom;
    GLint viewUniform;
    GLint projectionUniform;
    GLint mousePosUniform;
    //Frambuffer variables
    GLuint frameBuffer1;
    uint fbtexture,textureColorbuffer,rbo;
    uint quadVAO,quadVBO;
    _Shader *fboShader;
    //Matrices for Translation and view will be multiplied with the position to set translation rotaion ,scaling witrespect to view.
    glm::mat4 modelMatrix;
    glm::mat4 rotationMatrix,translationMatrix,scalingMatrix;
    glm::mat4 pivotTmat;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    //Holds the vertex and index data
    std::vector<float> vertices;//not allocated yet
    std::vector<uint> indices;//not allocated yet
    //
    std::vector<_Texture> textures;//Texture array for tetures in use for the respective renderer object
    //
    _SceneEntity sceneEntity;//the local sceneEntity object for use in the renderer
    //
    QElapsedTimer timer;//timer object to maintain a timer based events and trasformations
    //Stores the uniform location allocated in the shader
    void setuniformLocations();
    void keepSceneEntityUpdated();
    void setColors();

    glm::vec3 camposForLight;
    glm::vec3 lightPos;
    void setLights();

//    //test
//    void DRAWPAINTERTEXT();
};
#endif // _RENDERER_H
