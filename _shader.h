#ifndef _SHADER_H
#define _SHADER_H
#include <QString>
#include <QOpenGLExtraFunctions>
#include <_tools.h>
/*
 * Shader class:
 * Holds functonality to bind shader to a shaderprogram
 * and use it the current opengl Context.
 * Created: 14_02_2019
 * Author: Aditya Mattoo
*/
class _Shader: protected QOpenGLExtraFunctions
{
public:
    _Shader();
    ~_Shader();

    uint getShaderProgram();
    void setFragmentShader(QString fshader);
    void setVertexShader(QString vShader);
    void attachShaders();
    void attachShaders(QString v , QString f);//should be deprecated
    void setChildShader(QString s,unsigned int typ);
    void setChildShader(std::vector<QString>, unsigned int typ);
    void useShaderProgram();
    uint getUniformLocation(const char *nameOfUniform);

private:
    //Shader Program
     uint shaderProgram = 0;

     // map(dictionary) of shader
     //key : shader typ enum eg. GL_VERTEX_SHADER
     //value : shader ID returned by glCreateShader
     std::map<unsigned int,unsigned int> child_shaders;

     unsigned int compileShader(QString src_path,unsigned int typ);

     _Tools tools;

     //Error checking
     int success;
     char infoLog[512];
};

#endif // _SHADER_H
