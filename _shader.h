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

    void attachShaders();
    void attachShaders(QString v , QString f);
    void attachShaders(QString v , QString f,QString g);
    void useShaderProgram();
    GLint getUniformLocation(const char *nameOfUniform);

    void setChildShader(QString s,uint typ);
    void setChildShader(std::vector<QString>, uint typ);

private:
    std::string vShaderPath;//Pathname
    std::string fShaderPath;
    //Shader Program
    uint shaderProgram;
    std::map<uint,uint> child_shaders;

    uint compileShader(QString src_path,uint typ);
    void checkCompileErrors(GLuint shader, std::string type);

    _Tools tools;
    //Error checking and Test/Debug
    int success;
    char infoLog[512];
};

#endif // _SHADER_H
