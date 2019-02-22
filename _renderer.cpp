#include "_renderer.h"
#include <iostream>
#include <tools.h>
#include <qquaternion.h>
/*
 * The Renderer class
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
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_FRONT_AND_BACK);
	glClearColor(0.0, 0.1, 0.1, 1.0);//sets the bckground color of the openglContext.
	shdr = new _Shader();//initialising the _shader() class * object
	setShader();//will run this shader by default
	//
	view4x4.setToIdentity();
	model4x4.setToIdentity();
	//
	timer.start();
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
*/
void _Renderer::setMatrices(int w,int h)
{
	//z += 0.005;
	//model4x4.setToIdentity();
	model4x4.translate(0.0, 0.0, 0.0);
	model4x4.scale(1.0);
	//const QQuaternion q = QQuaternion(QVector3D(3.0,0.0,0.3));
	//model4x4.rotate(q);
	//
	view4x4.lookAt(
		QVector3D(0.0, 0.0, 5.0), // Eye
		QVector3D(0.0, 0.0, 0.0),  // Focal Point
		QVector3D(0.0, 1.0, 0.0)); // Up vector
	//
	// Calculate aspect ratio
	qreal aspect = qreal(w)/qreal(h ? h : 1);
	// Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
	const qreal zNear = 3.0, zFar = 7.0, fov = 90.0;
	projection4x4.setToIdentity();
	projection4x4.perspective(fov, aspect, zNear, zFar);
	//
	mvp = (projection4x4 * view4x4 * model4x4);
	//
	//For uniform with a position of 1 pass the value as colors
	this->colorUniform = shdr->getUniformLocation("aColor");
	//pass the mvp4x4 matrix to the vertex shader uniform mvp
	this->mvpUniform = shdr->getUniformLocation("mvp");
}
/*
 * This is your proprietory draw function 
 * Is being used by the _glWidget class
 * Create:11_02_2019
*/
void _Renderer::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //Using the shader program in the current context
    //can be called once in the init or every frame
    //if the shader is switching between objects
	shdr->useShaderProgram();
	//Bind the Buffers data of the respective buffer object
	//in the context each frame.
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	//Setting the uniform each frame.
	//Depends if the need is to update the values
	float r = abs(cos(timer.elapsed()* 0.002));
	float g = abs(sin(timer.elapsed() * 0.003));
	float b = abs(cos(timer.elapsed() * 0.005));
	glUniform4f(colorUniform, r,g,b, 1.0f);
	glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, mvp.constData());
	//
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
