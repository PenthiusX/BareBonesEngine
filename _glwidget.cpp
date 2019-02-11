#include "_glwidget.h"
#include <iostream>
//#include <QOpenGLShader>
//#include <tools.h>

_GLWidget::_GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}

/*
float vertices[] =
{
    0.5f,  0.5f, 0.0f,  // top right
    0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left
};
unsigned int indices[] = {  // note that we start from 0!
                            0, 1, 3,   // first triangle
                            1, 2, 3    // second triangle
                         };

float texCoords[] =
{
    0.0f, 0.0f,  // lower-left corner
    1.0f, 0.0f,  // lower-right corner
    0.5f, 1.0f   // top-center corner
};
*/

void _GLWidget::initializeGL()
{
    //---------------------------------

    initializeOpenGLFunctions();

    renderers.push_back(_Renderer());
    renderers.push_back(_Renderer());

    renderers[0].init(1.0f);
    renderers[1].init(0.5f);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
/*
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //---------------------------------
    //shader string literals


    QByteArray v_source_utf = ReadStringFromQrc(":/shaders/vshader.glsl").toLocal8Bit(); // get shader source from qrc file
    QByteArray f_source_utf = ReadStringFromQrc(":/shaders/fshader.glsl").toLocal8Bit(); // get shader source from qrc file

    const char *vshader = v_source_utf.data(); //convert to const char *
    const char *fShader = f_source_utf.data(); //convert to const char *

    //Vertex Shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vshader, NULL);
    glCompileShader(vertexShader);
    //check for compile success
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    //Fragment Shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShader, NULL);
    glCompileShader(fragmentShader);

    //shader program is a uint in the header
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    //-----------------------------------
    //  Initialization code (done once (unless your object frequently changes))
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    //
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //
    //-----------------------------------
    glUseProgram(shaderProgram);
    //-----------------------------------
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    //-----------------------------------
*/
}

void _GLWidget::resizeGL(int w, int h)
{

}
void _GLWidget::paintGL()
{
    renderers[0].draw();
    renderers[1].draw();

    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}
