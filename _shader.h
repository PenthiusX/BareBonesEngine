#ifndef _SHADER_H
#define _SHADER_H
#include <QString>
#include <QOpenGLExtraFunctions>
/*
 * Class: _Shader
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
    void setFragmentShader(QString fragmentshaderPath);
    void setVertexShader(QString vertexShaderPath);
    void attachShaders();
    void attachShaders(QString vertexInfo , QString fragmentInfo);
    void setChildShader(QString s,unsigned int typ);
    void setChildShader(std::vector<QString>, unsigned int typ);
	uint getUniformLocation(const char* nameOfUniform);
    void useShaderProgram();
	void resetShader();
	
private:
    //Shader Program
     uint shaderProgram;
     unsigned int vertexShader;
     unsigned int fragmentShader;
     //Error checking
     int success;
     char infoLog[512];

       std::map<unsigned int,unsigned int> child_shaders;
      unsigned int compile_shader(QString src_path,unsigned int typ);
};

#endif // _SHADER_H
