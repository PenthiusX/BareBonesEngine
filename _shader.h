#ifndef _SHADER_H
#define _SHADER_H
#include <QString>
#include <QOpenGLExtraFunctions>
#include "_tools.h"
/*
 * Shader class:
 * Holds functonality to bind shader to a shaderprogram
 * and use it the current opengl Context.
*/
class _Shader: protected QOpenGLExtraFunctions
{
public:
    _Shader();
    ~_Shader();

    uint getShaderProgram();

    void setFragmentShader(QString fshader);
    void setVertexShader(QString vShader);
    void setGeometryShader(QString geoS);
    void setComputeShader(QString compShader);

    void attachShaders();
    void attachShaders(QString v , QString f);
    void useShaderProgram();
    GLint getUniformLocation(const char *nameOfUniform);

    void setChildShader(QString s,uint typ);
    void setChildShader(std::vector<QString>, uint typ);

private:
    //Shader Program
    uint shaderProgram;

    // map(dictionary) of shader
    //key : shader typ enum eg. GL_VERTEX_SHADER
    //value : shader ID returned by glCreateShader
    std::map<uint,uint> child_shaders;

    uint compileShader(QString src_path,uint typ);


    _Tools tools;

    //Error checking and Test/Debug
    int success;
    char infoLog[512];
    void setUpParticles();
};

#endif // _SHADER_H
