#include "_shader.h"
#include "_tools.h"
#include <iostream>
#include <stdlib.h> 
#include <time.h> 
/*
 * The _Shader class
*/
/*
* Constructor
* Object initialised in _renderer class
*/
_Shader::_Shader() : QOpenGLExtraFunctions(QOpenGLContext::currentContext())
{
    shaderProgram = 0;
}
/*Distructor*/
_Shader::~_Shader(){}
/*
 * getShaderProgram() retrun the shaderprogram
 * uint variable.
 * Is being used by the _renderer class
*/
uint _Shader::getShaderProgram()
{
    return shaderProgram;
}
/*
 * setFragmentShader(QString f) copiles and,
 * binds the fragment shader passed in form a Qstring,
 * and returns an uint.
 * Is being used by the _renderer class
*/
void _Shader::setFragmentShader(QString f)
{
    //Fragment Shader
    setChildShader(f,GL_FRAGMENT_SHADER);
}
/*
 * setVertexShader(QString v) copiles and
 * binds the vertex shader passed in from a Qstring parameter,
 * and returns an uint;
 * Is being used by the _renderer class
*/
void _Shader::setVertexShader(QString v)
{
    setChildShader(v,GL_VERTEX_SHADER);
}
/*
 * attachShaders(), attaches the shaders to the GLProgram
 * this will only work if the fragment and vertex shader
 * have been compiled before this function.\
*/
void _Shader::attachShaders()
{
    //check for child shaders
    if(child_shaders.size()!=0){
        //create Shader Program
        shaderProgram = glCreateProgram();

        //attatch child shaders to program
        for (auto const& child_shader : child_shaders){
            glAttachShader(shaderProgram, child_shader.second);//second specifies value at key in map(dictionary)
        }

        glLinkProgram(shaderProgram);
        //check for link error
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if(!success){

            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADERPROGRAM::LINK_FAILED." << infoLog << std::endl;
            assert(success != true);
        }
        //delete shader
        for (auto const& child_shader : child_shaders)
        {
            glDeleteShader(child_shader.second);
            glDetachShader(shaderProgram,child_shader.second);
        }
    }
    else{//if no child shaders attatched
        std::cerr << "ERROR::SHADERCLASS::NO_CHILD_SHADERS_ATTATCHED." << std::endl;
        exit(0);}
}
/*
 * attachShaders(QString v,QString f),
 * this then binds the
*/
void _Shader::attachShaders(QString v,QString f)
{
    setVertexShader(v);
    setFragmentShader(f);
    attachShaders();
}
/*
 * attachGeometryShader(QString geoS)
 * takes in a qrc path to the geometry shader for this
 * shaderprogram instance
 **NOT IN USE**
*/
void _Shader::setGeometryShader(QString geoS)
{
    QByteArray source_utf = geoS.toLocal8Bit(); // get shader source from qrc file
    const char *shader_src = source_utf.data(); //convert to const char*

    auto geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometryShader, 1, &shader_src, nullptr);
    glCompileShader(geometryShader);
    glAttachShader(shaderProgram, geometryShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(geometryShader);
    glDetachShader(shaderProgram,geometryShader);
}
/*
* getUniformLocation(char* nameOfUniform)
* returns a uint representing the loaction index of
* the uniform in the shader takes the name of the uniform
* as the parameter
*/
GLint _Shader::getUniformLocation(const char* nameOfUniform)
{
   // qInfo() << nameOfUniform << "-" <<glGetUniformLocation(shaderProgram, nameOfUniform);
    return  glGetUniformLocation(shaderProgram, nameOfUniform);
}
/*
 *
*/
void _Shader::setChildShader(QString s, uint typ)
{
    std::cout << s.data() ;
    uint shader = compileShader(tools.ReadStringFromQrc(s),typ);
    child_shaders[typ]=shader;//setting dictionary value shader ID at key typ
}
/*
 */
void _Shader::setChildShader(std::vector<QString> shader_parts, uint typ)
{
    QString combined_src;
    for (auto const& shader_part : shader_parts)
    {
        combined_src = combined_src + tools.ReadStringFromQrc(shader_part);//second specifies value at key in map(dictionary)
    }
    //tools.ReadStringFromQrc(s);
    uint shader = compileShader(combined_src,typ);
    child_shaders[typ]=shader;//setting dictionary value shader ID at key typ
}
/*
 * useShaderProgram() Needs to be called before draw
 * everyloop for multiple
 * sets which shader needs to be used in the
 * current context
 */
void _Shader::useShaderProgram()
{
    glUseProgram(shaderProgram);
}
/*
 * compileShader()
 * everyloop for multiple
 */
uint _Shader::compileShader(QString src, uint typ)
{
    uint shader;
    QByteArray source_utf = src.toLocal8Bit(); // get shader source from qrc file
    const char *shader_src = source_utf.data(); //convert to const char*

    //shader
    shader = glCreateShader(typ);
    glShaderSource(shader, 1, &shader_src, nullptr);
    glCompileShader(shader);

    //check for compile success
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED::TYPE_ENUM: " << typ  << infoLog << std::endl;
        assert(success != true);
    }
    return shader;
}

//prototype implementation
template<class T> void setUniformArrayValue(uint shaderProgram,
                                       const QString& arrayName,
                                       const QString& varName,
                                       int index,
                                       const T& value)
{
    const char* name = QString("%1[%2].%3")
            .arg(arrayName)
            .arg(index)
            .arg(varName)
            .toStdString().c_str();
   return getUniformLocation(name, value);
}
