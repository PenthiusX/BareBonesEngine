#include "_shader.h"
#include <_tools.h>
#include <iostream>
#include <QDebug>
/*
 * The _Shader class
 * Created: 14_02_2019
 * Author: Aditya ,Saurabh
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
    if(child_shaders.size()!=0){

        //create Shader Program
        shaderProgram = glCreateProgram();

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
 * Function: shader_parser(QString shader_file)
 * this function solves all #include dependancies
 * eg. if shader source:
 *
 *   vshader_uniforms.glsl:
 *                      uniform mat4 model;
 *                      uniform mat4 view;
 *                      uniform mat4 projection;
 *                      uniform mat4 mvp;
 *
 *   vshader_main.glsl :
 *                      #version 330 core
 *                      layout (location = 0) in vec3 aPos;
 *                      out vec2 TexCoord;
 *
 *                      #include vshader_uniforms.glsl// <<<<< shader source of vshader_uniforms.glsl will be inserted here
 *
 *                      void main()
 *                      {
 *                          gl_Position = vec4(aPos, 1.0);
 *                          TexCoord = vec2(aPos.x/2.0+0.5, 0.5-aPos.y/2.0);
 *                      }
 *   use fnction as :
 *   setChildshader(std::vector<QString> parts{"path_to/vshader_head.glsl","path_to/vshader_uniforms.glsl","path_to/vshader_main.glsl"})
*/
QString _Shader::shader_parser(QString shader_file,glm::ivec3 workgroup_size)
{

    //for dealing with #include statements and dynamic workgroup sizes
    QStringList included_files;

    QString shader_src = tools.ReadStringFromQrc(shader_file);

    int pos = shader_file.lastIndexOf(QChar('/'));
    QString shader_folder = shader_file.left(pos) + "/";

    while (true) {
        pos = shader_src.indexOf("#include");
        if(pos != -1){
            QString src = shader_src.left(pos);
            //file_name_should be less than 30 characters
            QString include_line = shader_src.mid(pos+9,40);

            int f_pos = include_line.indexOf(".glsl");
            if(f_pos == -1) f_pos = include_line.indexOf(' ');
            else f_pos+=5;
            if(f_pos == -1) f_pos = include_line.indexOf('\n');
            QString include_file = shader_folder + include_line.left(f_pos);
            src = src + tools.ReadStringFromQrc(include_file);
            src = src + shader_src.right(shader_src.length()-(pos+9+f_pos));
            shader_src = src;
        }
        else break;
    }

    shader_src.replace("#local_size_define",QString("layout (local_size_x =%1, local_size_y = %2, local_size_z = %3) in;").arg(workgroup_size.x).arg(workgroup_size.y).arg(workgroup_size.z));

    return shader_src;
}

/* Function : setChildShader(QString s, unsigned int typ)
 * vertex , fragment , geometry etc are child shaders of shader program object
 * this function addds these shaders to the shader program objects
 * takes path to shader file and typ of shader ie. GL_VERTEX_SHADER
*/
void _Shader::setChildShader(QString s, unsigned int typ, glm::ivec3 workgroup_size)
{
    //solve shader dependancies and create final shader source
    QString shader_src = shader_parser(s,workgroup_size);


    unsigned int shader = compile_shader(shader_src,typ);

    child_shaders[typ]=shader;//setting dictionary value shader ID at key typ
}

/* Function : setChildShader(QString s, unsigned int typ)
 * this function takes child shader in seprated parts and create combined shader source
 * eg if shader is seprated in 3 parts:
 *   vshader_head.glsl :     #version 330 core
 *                      layout (location = 0) in vec3 aPos;
 *                      out vec2 TexCoord;
 *
 *   vshader_uniforms.glsl:
 *                      uniform mat4 model;
 *                      uniform mat4 view;
 *                      uniform mat4 projection;
 *                      uniform mat4 mvp;
 *
 *   vshader_main.glsl :
 *                      void main()
 *                      {
 *                          gl_Position = vec4(aPos, 1.0);
 *                          TexCoord = vec2(aPos.x/2.0+0.5, 0.5-aPos.y/2.0);
 *                      }
 *   use fnction as :
 *   setChildshader(std::vector<QString> parts{"path_to/vshader_head.glsl","path_to/vshader_uniforms.glsl","path_to/vshader_main.glsl"})
*/
void _Shader::setChildShader(std::vector<QString> shader_parts, unsigned int typ)
{
    QString combined_src;

    for (auto const& shader_part : shader_parts)
    {
        combined_src = combined_src + tools.ReadStringFromQrc(shader_part);//second specifies value at key in map(dictionary)
    }

    //tools.ReadStringFromQrc(s);

    unsigned int shader = compile_shader(combined_src,typ);
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
 * Function: compile_shader(QString src, unsigned int typ)
 * compiles given shader source src and type eg. GL_VERTEX_SHADER
 * returns ID of shader
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



