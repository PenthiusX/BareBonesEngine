#include "_shader.h"
#include "_tools.h"
#include <iostream>
#include <stdlib.h> 
#include <time.h> 
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
    QByteArray source_utf = compShader.toLocal8Bit(); // get shader source from qrc file
    const char *shader_src = source_utf.data(); //convert to const char*

    auto computeShader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(computeShader, 1, &shader_src, nullptr);
    glCompileShader(computeShader);

    // Check if there were any issues compiling the shader.
    int rvalue;
    glGetShaderiv(computeShader, GL_COMPILE_STATUS, &rvalue);

    if (!rvalue)
    {
        glGetShaderInfoLog(computeShader, 512, nullptr, infoLog);
        qDebug() << "Error: Compiler log:\n%s\n" << infoLog ;
    }

    // Bind the compute program so it can read the radius uniform location.
    glUseProgram(computeShader);

    // Retrieve the radius uniform location
   GLuint iLocRadius = glGetUniformLocation(computeShader, "radius");
    // See the compute shader: “layout(std140, binding = 0) buffer destBuffer”
    GLuint gIndexBufferBinding = 0;

   // Bind the compute program.
   glUseProgram(computeShader);

   // Set the radius uniform.
   glUniform1f(iLocRadius, (float)2.0);

   // Bind the VBO to the SSBO, that is filled in the compute shader.
   // gIndexBufferBinding is equal to 0. This is the same as the compute shader binding.
   GLuint gVBO;
   glGenBuffers( 1, &gVBO);
   glBindBufferBase(GL_SHADER_STORAGE_BUFFER, gIndexBufferBinding,gVBO);

   // Submit a job for the compute shader execution.
   // GROUP_SIZE = 64
   // NUM_VERTS = 256
   // As the result the function is called with the following parameters:
   // glDispatchCompute(4, 1, 1)
   glDispatchCompute(4, 1, 1);

   // Unbind the SSBO buffer.
   // gIndexBufferBinding is equal to 0. This is the same as the compute shader binding.
   glBindBufferBase(GL_SHADER_STORAGE_BUFFER, gIndexBufferBinding, 0);

   // Call this function before you submit a draw call,
   // that uses a dependency buffer, to the GPU
   glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

   // Bind the VBO
   glBindBuffer( GL_ARRAY_BUFFER, gVBO );

//   // Bind the vertex and fragment rendering shaders
//   glUseProgram(gProgram);
//   glEnableVertexAttribArray(iLocPosition);
//   glEnableVertexAttribArray(iLocFillColor);

//    setUpParticles();
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

//Proptotype implemntation , only for testing
//Not in use at the moment.
//void _Shader::setUpParticles()
//{
//#define NUM_PARTICLES 1024*1024 // total number of particles to move
//    //Setting up the Shader Storage Buffer Objects in Your C Program
//#define WORK_GROUP_SIZE 128 // # work-items per work-group
//    struct pos
//    {
//        float x, y, z, w; // positions // positions
//    };
//    struct vel
//    {
//        float vx, vy, vz, vw; // l iti // velocities
//    };
//    struct color
//    {
//        float r, g, b, a; // colors
//    };
//    // need to do the following for both position, velocity, and colors

//    GLuint posSSbo;
//    GLuint velSSbo;
//    GLuint colSSbo;
//    float Xmin,Xmax;
//    Xmin = -10.0f;
//    Xmax = 10.0f;
//    float vXmin,vXmax;
//    vXmin = 0.0;
//    vXmax = 2.0;

//    glGenBuffers( 1, &posSSbo);
//    glBindBuffer( GL_SHADER_STORAGE_BUFFER, posSSbo );
//    glBufferData( GL_SHADER_STORAGE_BUFFER, NUM_PARTICLES * sizeof(struct pos), nullptr, GL_STATIC_DRAW );

//    GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT ;
//    struct pos *points = (struct pos*)glMapBufferRange( GL_SHADER_STORAGE_BUFFER,0,NUM_PARTICLES * sizeof (struct pos),bufMask);
//    srand (time(nullptr));
//    for(int i = 0;i <NUM_PARTICLES;i++)
//    {
//        points[i].x = rand() % (int)(Xmax - Xmin);
//        points[i].y = rand() % (int)(Xmax - Xmin);
//        points[i].z = rand() % (int)(Xmax - Xmin);
//        points[i].w = 1.0f;
//    }
//    glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );

//    glGenBuffers( 1, &velSSbo);
//    glBindBuffer( GL_SHADER_STORAGE_BUFFER, velSSbo );
//    glBufferData( GL_SHADER_STORAGE_BUFFER, NUM_PARTICLES * sizeof(struct vel), nullptr, GL_STATIC_DRAW );

//    struct vel *vels = (struct vel *) glMapBufferRange( GL_SHADER_STORAGE_BUFFER, 0, NUM_PARTICLES * sizeof(struct vel), bufMask );
//    for( int i = 0; i < NUM_PARTICLES; i++ )
//    {
//        vels[ i ].vx = rand() % (int)(Xmax - Xmin);
//        vels[ i ].vy = rand() % (int)(Xmax - Xmin);
//        vels[ i ].vz = rand() % (int)(Xmax - Xmin);
//        vels[ i ].vw = 0.;
//    }
//    glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );

//    glGenBuffers( 1, &colSSbo);
//    glBindBuffer( GL_SHADER_STORAGE_BUFFER, velSSbo );
//    glBufferData( GL_SHADER_STORAGE_BUFFER, NUM_PARTICLES * sizeof(struct vel), nullptr, GL_STATIC_DRAW );

//    struct color *cols = (struct color *) glMapBufferRange( GL_SHADER_STORAGE_BUFFER, 0, NUM_PARTICLES * sizeof(struct color), bufMask );
//    for( int i = 0; i < NUM_PARTICLES; i++ )
//    {
//        vels[ i ].vx = rand() % (int)(Xmax - Xmin);
//        vels[ i ].vy = rand() % (int)(Xmax - Xmin);
//        vels[ i ].vz = rand() % (int)(Xmax - Xmin);
//        vels[ i ].vw = 0.;
//    }

//    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 4, posSSbo );
//    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 5, velSSbo );
//    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 6, colSSbo );
//}
