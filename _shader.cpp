#include "_shader.h"
#include "tools.h"
#include <iostream>

_Shader::_Shader() : QOpenGLExtraFunctions(QOpenGLContext::currentContext())
{
    vertexShader = 0;
    fragmentShader = 0;
}
_Shader::~_Shader(){}
/*
 * The getShaderProgram() retrun the shaderprogram 
 * unsigned int variable.
 * created 14_02_2019
*/
uint _Shader::getShaderProgram()
{
    return this->shaderProgram;
}
/*
 * The setFragmentShader(QString f) copiles and
 * binds the fragment shader passed in form a Qstring
 * and returns an unsigned int;
 * created 14_02_2019
*/
void _Shader::setFragmentShader(QString f)
{
    QByteArray v_source_utf = ReadStringFromQrc(f).toLocal8Bit(); // get shader source from qrc file
    const char *fshader = v_source_utf.data(); //convert to const char*

    //Vertex Shader
    fragmentShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(fragmentShader, 1, &fshader, NULL);
    glCompileShader(fragmentShader);

    //check for compile success
    int success;
    char infoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}
/*
 * The setVertexShader(QString v) copiles and 
 * binds the vertex shader passed in from a Qstring parameter,
 * and returns an unsigned int;
 * created 14_02_2019
*/
void _Shader::setVertexShader(QString v)
{
    QByteArray v_source_utf = ReadStringFromQrc(v).toLocal8Bit(); // get shader source from qrc file
    const char *vshader = v_source_utf.data(); //convert to const char*

    //Vertex Shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vshader, NULL);
    glCompileShader(vertexShader);

    //check for compile success
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}
/*
 * attachShaders(), attaches the shaders to the GLProgram
 * this will only work if the fragment and vertex shader
 * have been compiled before this function.\
 * created 14_02_2019
*/
void _Shader::attachShaders()
{
    if(vertexShader > 0 && fragmentShader > 0)
    {
        //shader program is a uint in the header
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADERPROGRAM::COMPILATION_FAILED." << infoLog << std::endl;
        }
    }
    else
    {
       std::cerr << "ERROR::SHADERCLASS::NEED_VERTEX_FRAGMENT_SHADERS_FIRST." << std::endl;
       exit(0);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
/*
 * attachShaders(QString v,QString f), 
 * this then binds the 
 * created 14_02_2019
*/
void _Shader::attachShaders(QString v,QString f)
{
    //shader string literals
    QByteArray v_source_utf = ReadStringFromQrc(v).toLocal8Bit(); // get shader source from qrc file
    QByteArray f_source_utf = ReadStringFromQrc(f).toLocal8Bit(); // get shader source from qrc file

    const char *vshader = v_source_utf.data(); //convert to const char*
    const char *fShader = f_source_utf.data(); //convert to const char*

    //Vertex Shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vshader, NULL);
    glCompileShader(vertexShader);
    //check for compile success
    int success;
    char infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    //Fragment Shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShader, NULL);
    glCompileShader(fragmentShader);\
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!fragmentShader)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //shader program is a uint in the header
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADERPROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //delete shaders after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
/*
 * Needs to be called before draw
 * everyloop for multiple
 * sets which shader needs to be used in the
 * current context
 * created 14_02_2019
 */
void _Shader::useShaderProgram()
{
    glUseProgram(this->shaderProgram);
}
