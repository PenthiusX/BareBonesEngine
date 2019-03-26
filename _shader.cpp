#include "_shader.h"
#include "tools.h"
#include <iostream>
/*
 * The _Shader class
 * Created: 14_02_2019
 * Author: Aditya
*/
/*
* Constructor 
* Object initialised in _renderer class
* Created: 14_02_2019
*/
_Shader::_Shader() : QOpenGLExtraFunctions(QOpenGLContext::currentContext())
{
    vertexShader = 0;
    fragmentShader = 0;
}
/*Distructor*/
_Shader::~_Shader(){}
/*
 * Function: getShaderProgram() retrun the shaderprogram 
 * unsigned int variable.Is being used by the _renderer class
 * Created: 14_02_2019
*/
uint _Shader::getShaderProgram()
{
    return this->shaderProgram;
}
/*
 * Function: setFragmentShader(QString f) copiles and,
 * binds the fragment shader passed in form a Qstring,
 * and returns an unsigned int.
 * Is being used by the _renderer class
 * Created: 14_02_2019
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
 * Function: setVertexShader(QString v) 
 * copiles and binds the vertex shader passed in from  
 * a Qstring parameter, and returns an unsigned int;
 * Is being used by the _renderer class
 * Created: 14_02_2019
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
 * Function: attachShaders(), attaches the shaders to the GLProgram
 * this will only work if the fragment and vertex shader
 * have been compiled before this function.
 * Created: 14_02_2019
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
 * Function: attachShaders(QString v,QString f), 
 * takes the qrc aided path to the vertex and fragment 
 * shader files and compiles them for linking
 * Created: 14_02_2019
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
    glCompileShader(fragmentShader);
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
* Function: getUniformLocation(char* nameOfUniform)
* returns a uint representing the loaction index of 
* the uniform in the shader takes the name of the uniform
* as the parameter
* Created: 18_02_2019
*/
uint _Shader::getUniformLocation(const char* nameOfUniform)
{
	return  glGetUniformLocation(this->shaderProgram, nameOfUniform);
}
/*
 * Function: useShaderProgram()
 * Needs to be called before draw everyloop for multiple
 * sets which shader needs to be used in the current context
 * Created: 14_02_2019
*/
void _Shader::useShaderProgram()
{
	glUseProgram(this->shaderProgram);
}
/*
* Function: resetShader()
* resets the shader pointer value to 0;
* Created: 08_03_2019
*/
void _Shader::resetShader()
{
	glUseProgram(0);
}
/*
 * Sau feature , needs checking
*/
void _Shader::setChildShader(QString s, unsigned int typ)
{
    unsigned int shader = compile_shader(ReadStringFromQrc(s),typ);
    child_shaders[typ]=shader;//setting dictionary value shader ID at key typ
}
/*
 * Sau feature , needs checking
*/
void _Shader::setChildShader(std::vector<QString> shader_parts, unsigned int typ)
{
    QString combined_src;

    for (auto const& shader_part : shader_parts)
    {
        combined_src = combined_src + ReadStringFromQrc(shader_part);//second specifies value at key in map(dictionary)
    }

    //tools.ReadStringFromQrc(s);

    unsigned int shader = compile_shader(combined_src,typ);
    child_shaders[typ]=shader;//setting dictionary value shader ID at key typ
}
/*
 * Sau feature , needs checking
 */
unsigned int _Shader::compile_shader(QString src, unsigned int typ)
{
    unsigned int shader;
    QByteArray source_utf = src.toLocal8Bit(); // get shader source from qrc file
    const char *shader_src = source_utf.data(); //convert to const char*
    //shader
    shader = glCreateShader(typ);
    glShaderSource(shader, 1, &shader_src, NULL);
    glCompileShader(shader);
    //check for compile success
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED::TYPE_ENUM: " << typ  << infoLog << std::endl;
    }

    return shader;
}
