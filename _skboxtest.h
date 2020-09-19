#pragma once
#include <QOpenGLExtraFunctions>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <_shader.h>
#include <QImage>
#include <QString>
#include <QImage>

#include "_camera.h"



class TestSkyBox : public QOpenGLExtraFunctions
{
private:
    _Shader* shader;
    GLuint v,p;
    GLuint skyboxVAO, skyboxVBO;
    unsigned int cubemapTexture;
    std::vector<QString> faces;
    glm::mat4x4 viewMatrix = glm::mat4(1.0f);
    glm::mat4x4 perspective = glm::mat4(1.0f);

public :

    TestSkyBox() : QOpenGLExtraFunctions(QOpenGLContext::currentContext()){}
    ~TestSkyBox(){
        delete shader;
    }

    void initialise()
    {
        shader = new _Shader();
        shader->attachShaders(":/shaders/skyboxV.glsl", ":/shaders/skyboxF.glsl");

        float skyboxVertices[] = {
            // positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };

        //Test tex
        faces.push_back(":/textures/right.jpg");
        faces.push_back(":/textures/left.jpg");
        faces.push_back(":/textures/top.jpg");
        faces.push_back(":/textures/bottom.jpg");
        faces.push_back(":/textures/front.jpg");
        faces.push_back(":/textures/back.jpg");

//        faces.push_back(":/textures/nightsky_east.bmp");
//        faces.push_back(":/textures/nightsky_west.bmp");
//        faces.push_back(":/textures/nightsky_up.bmp");
//        faces.push_back(":/textures/nightsky_down.bmp");
//        faces.push_back(":/textures/nightsky_north.bmp");
//        faces.push_back(":/textures/nightsky_south.bmp");



        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);
        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

       cubemapTexture  = loadCubemap(faces);

       shader->useShaderProgram();
       glUniform1i(shader->getUniformLocation("skybox"), 10);
       v = shader->getUniformLocation("view");
       p = shader->getUniformLocation("projection");
    }

    //
    unsigned int loadCubemap(std::vector<QString> faces)
    {
        unsigned int textureID;
        glBindTexture(GL_TEXTURE_2D,0);
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            QImage img = QImage(faces[i]).convertToFormat(QImage::Format_RGBA8888);;
            width = img.width();
            height = img.height();
            if (img.bits())
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.bits());
            }
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureID;
    }



    void draw(_Camera cam, int resH,int resW)
    {
        viewMatrix = glm::lookAt(glm::vec3(cam.getEyePosition().x, cam.getEyePosition().y,cam.getEyePosition().z),
                                 glm::vec3(cam.getFocalPoint().x, cam.getFocalPoint().y, cam.getFocalPoint().z),
                                 glm::vec3(cam.getUpVector().x, cam.getUpVector().y, cam.getUpVector().z));

       float aspect = float(resW) / float(resH ? resH : 1);
        perspective =  glm::perspective(glm::radians((float)cam.getFOV()), aspect, cam.getNearClipDistance(),cam.getFarClipDistance());

        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        shader->useShaderProgram();

        glUniformMatrix4fv(p, 1, GL_FALSE, glm::value_ptr(perspective));//skyboxShader.setMat4("projection", projection);
        glUniformMatrix4fv(v, 1, GL_FALSE, glm::value_ptr(glm::mat4(glm::mat3(viewMatrix))));// remove translation from the view matrix

        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE10);//10 texture slot
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default
     }
};
