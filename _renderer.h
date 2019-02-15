#ifndef _RENDERER_H
#define _RENDERER_H
#include <vector>
#include "_shader.h"
#include <qopenglextrafunctions.h>

/*
 * The Renderer class
 * To create an abstraction for randering data
 * Extends the _glwidget class, will be included in the _glwidget class
 *  * Created: 8_02_2019
 * Author: Aditya,Saurabh
*/
class _Renderer : protected QOpenGLExtraFunctions
{
public:
    _Renderer();
    ~_Renderer();

    void setShader();//default shatder to load
    void setShader(QString vertexShader, QString fragmentShader);//takes a string literal and passes
    void setBuffers(std::vector<float>vertexArray,std::vector<int> indexArray);//take vertex and index data and binds it to object buffer
    void setTexture(char* texBitmap);//takes am image and binds it to object
    void draw();//Draws everything bound in the scene

private:
unsigned int VBO;//vertex buffer object
unsigned int VAO;//attribute buffer object 
unsigned int EBO;//index buffer object
//Shader class object sets the shaders and passes
//the program to the current context
_Shader* shdr;

std::vector<float> vertices;
std::vector<int> indices;

};

#endif // _RENDERER_H
