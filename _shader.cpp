#include "_shader.h"
#include "_tools.h"
#include <iostream>
/*
 * The _Shader class
 * Created: 14_02_2019
 * Author: Aditya , Suarabh
*/
/*
* Constructor
* Object initialised in _renderer class
* Created: 14_02_2019
*/
_Shader::_Shader() : QOpenGLExtraFunctions(QOpenGLContext::currentContext())
{

}
/*Distructor*/
_Shader::~_Shader(){}
/*
 * Function: getShaderProgram() retrun the shaderprogram
 * unsigned int variable.
 * Is being used by the _renderer class
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
    //Fragment Shader
    setChildShader(f,GL_FRAGMENT_SHADER);
}
/*
 * Function: setVertexShader(QString v) copiles and
 * binds the vertex shader passed in from a Qstring parameter,
 * and returns an unsigned int;
 * Is being used by the _renderer class
 * Created: 14_02_2019
*/
void _Shader::setVertexShader(QString v)
{
    setChildShader(v,GL_VERTEX_SHADER);
}
/*
 * Function: attachShaders(), attaches the shaders to the GLProgram
 * this will only work if the fragment and vertex shader
 * have been compiled before this function.\
 * Created: 14_02_2019
*/
void _Shader::attachShaders()
{
    //check for child shaders
    if(child_shaders.size()!=0)
    {
        //create Shader Program
        this->shaderProgram = glCreateProgram();

        //attatch child shaders to program
        for (auto const& child_shader : child_shaders)
        {
            glAttachShader(shaderProgram, child_shader.second);//second specifies value at key in map(dictionary)
        }
        glLinkProgram(shaderProgram);

        //check for link error
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADERPROGRAM::LINK_FAILED." << infoLog << std::endl;
        }

        //delete shader
        for (auto const& child_shader : child_shaders)
        {
            glDeleteShader( child_shader.second);
        }
    }
    else//if no child shaders attatched
    {
        std::cerr << "ERROR::SHADERCLASS::NO_CHILD_SHADERS_ATTATCHED." << std::endl;
        exit(0);
    }
}
/*
 * Function: attachShaders(QString v,QString f),
 * this then binds the
 * Created: 14_02_2019
*/
void _Shader::attachShaders(QString v,QString f)
{
    setVertexShader(v);
    setFragmentShader(f);
    attachShaders();
}
/*
 * Function: attachGeometryShader(QString geoS)
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
    glAttachShader(this->shaderProgram, geometryShader);
    glLinkProgram(this->shaderProgram);
}

void _Shader::setComputeShader(QString compShader)
{
    static const char* ComputeShader = \
    "#version 430 core\n"
    "writeonly uniform image2D writer;"
    "layout (local_size_x = 16, local_size_y = 16) in;"
    "void main()"
    "{"
        "vec2 coordinates = gl_GlobalInvocationID.xy;"
        "vec2 resolution = vec2(512,512);"
        "vec2 k = sign(cos(coordinates/resolution.yy*32.0));"
        "imageStore(writer,ivec2(gl_GlobalInvocationID.xy),vec4(k.x*k.y));"
    "}" ;
}
/*
* Function: getUniformLocation(char* nameOfUniform)
* returns a uint representing the loaction index of
* the uniform in the shader takes the name of the uniform
* as the parameter
* Created: 18_02_2019
*/
GLint _Shader::getUniformLocation(const char* nameOfUniform)
{
    return  glGetUniformLocation(this->shaderProgram, nameOfUniform);
}
/*
 *
*/
void _Shader::setChildShader(QString s, unsigned int typ)
{
    unsigned int shader = compileShader(tools.ReadStringFromQrc(s),typ);
    child_shaders[typ]=shader;//setting dictionary value shader ID at key typ
}
/*
 *
*/
void _Shader::setChildShader(std::vector<QString> shader_parts, unsigned int typ)
{
    QString combined_src;
    for (auto const& shader_part : shader_parts)
    {
        combined_src = combined_src + tools.ReadStringFromQrc(shader_part);//second specifies value at key in map(dictionary)
    }
    //tools.ReadStringFromQrc(s);
    unsigned int shader = compileShader(combined_src,typ);
    child_shaders[typ]=shader;//setting dictionary value shader ID at key typ
}

/*
 * Function: useShaderProgram() Needs to be called before draw
 * everyloop for multiple
 * sets which shader needs to be used in the
 * current context
 * Created: 14_02_2019
 */
void _Shader::useShaderProgram()
{
    glUseProgram(this->shaderProgram);
}
/*
 * Function: compileShader()
 * everyloop for multiple
 * Created: 26_02_2019
 */
unsigned int _Shader::compileShader(QString src, unsigned int typ)
{
    unsigned int shader;
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
    }
    return shader;
}
