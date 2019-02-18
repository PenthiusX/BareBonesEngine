#include "_renderer.h"
#include <tools.h>
#include <iostream>
/*
 * The Renderer class
 * To create an abstraction for randering data
 * Extends the _glwidget class, will be included in the _glwidget class.
 * Created: 8_02_2019
 * Author: Aditya,Saurabh
*/
/* Constructor for the renderer class
 * The "QOpenGLExtraFunctions(QOpenGLContext::currentContext())" is passed by parameter
 * to avoid using initialiseopenglfunction() in the initcallback.
 * Create:11_02_2019
 */
_Renderer::_Renderer() : QOpenGLExtraFunctions(QOpenGLContext::currentContext())
{
	glEnable(GL_DEPTH_TEST);//
	glClearColor(0.0, 0.1, 0.1, 1.0);//sets the bckground color of the openglContext.
	shdr = new _Shader();//initialising the _shader() class * object
	setShader();//will run this shader by default
}
/*
 * Class Distructor
*/
_Renderer::~_Renderer()
{
	shdr = NULL;
	delete shdr;
}
/*
 * Set Shader Function(no params)
 * Sets a dafault hardfed shader
 * on the render object
 * Is being used by the _glWidget class
 * Create:11_02_2019 
*/
void _Renderer::setShader()
{
	shdr->attachShaders(":/shaders/vshader.glsl", ":/shaders/fshader.glsl");
}
/*
 * SetShadr function (Qstring , Q string)
 * takes the path to the relative qrc aided directory
 * to set shader paths externaly on the render object.
 * Is being used by the _glWidget class
 * Create:11_02_2019
*/
void _Renderer::setShader(QString vSh, QString fSh)
{
	shdr->attachShaders(vSh,fSh);
	int a = glGetUniformLocation(shdr->getShaderProgram(), "aColor");
	//we use 1 as the color index location as we have set it to 1 in the shader
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
}
/*
 * SetBUffers set Vertex and Index data into
 * the GPU buffers to use for the current model.
 * The functions takes two parameters.
 * Is being used by the _glWidget class
 * Created: 11_02_2019
*/
void _Renderer::setBuffers(std::vector<float> vertexArray, std::vector<int> indexArray)
{
    //  Initialization code (done once (unless your object frequently changes))
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    //
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //
    glBufferData(GL_ARRAY_BUFFER, vertexArray.size() * sizeof (float), &vertexArray[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArray.size()* sizeof(int), &indexArray[0], GL_STATIC_DRAW);
    //
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}
/*
 * 
*/
void _Renderer::setTexture(char *texBitmap)
{

}
/*
 * This is your proprietory draw function 
 * Is being used by the _glWidget class
 * Create:11_02_2019
*/
void _Renderer::draw()
{
    //using the shader program in the current context
    //can be called once in the init or every frame
    //if the shader is switching between objects
   	shdr->useShaderProgram();
	//
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    //
    //Dont need to reset the shaderprogram to 0;
    //will be overriten anyways
    //glUseProgram(0);
}
