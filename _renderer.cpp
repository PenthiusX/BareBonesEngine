#include "_renderer.h"
#include <iostream>
#include <tools.h>
#include <qquaternion.h>
/*
 * The Renderer class
 * Created: 8_02_2019
 * Author: Aditya,Saurabh
*/
/* Constructor: _Renderer Class
 * The "QOpenGLExtraFunctions(QOpenGLContext::currentContext())" is passed by parameter
 * to avoid using initialiseopenglfunction() in the initcallback.
 * Create:11_02_2019
 */
_Renderer::_Renderer() : QOpenGLExtraFunctions(QOpenGLContext::currentContext())
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_FRONT_AND_BACK);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClearColor(0.0, 0.3, 0.3, 1.0);//sets the bckground color of the openglContext.
	shdr = new _Shader();//initialising the _shader() class * object
	setShader();//will run this shader by default
	//
	view4x4.setToIdentity();
	model4x4.setToIdentity();
	projection4x4.setToIdentity();
	//
	glm_model4x4 = glm::mat4(1.0f);
	glm_projection4x4 = glm::mat4(1.0f);
	glm_view4x4 = glm::mat4(1.0f);
	//
	timer.start();
}
/*
 *Distructor: _Renderer Class
*/
_Renderer::~_Renderer()
{
	shdr = NULL;
	delete shdr;
}
/*
 * Function: setShader(no params)
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
 * Function: setShader(QString vSh, QString fSh)
 * Takes the path to the relative qrc aided directory
 * to set shader paths externaly on the render object.
 * Is being used by the _glWidget class
 * Create:11_02_2019
*/
void _Renderer::setShader(QString vSh, QString fSh)
{
	shdr->attachShaders(vSh,fSh);
}
/*
 * Function: setBuffers(std::vector<float> vertexArray, std::vector<int> indexArray)
 * set Vertex and Index data into
 * the GPU buffers to use for the current model.
 * May have extended implementation for inclusion of UV for texture and Normals for 
 * lighting.
 * Used by: the _glWidget class initializeGL().
 * Created: 11_02_2019
*/
void _Renderer::setBuffers(std::vector<float> vertexArray, std::vector<unsigned int> indexArray)
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArray.size() * sizeof(int), &indexArray[0], GL_STATIC_DRAW);
    //
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
	//
	this->colorUniform = shdr->getUniformLocation("aColor");//For uniform with a position of 1 pass the value as colors
	//this->mvpUniform = shdr->getUniformLocation("mvp");	//Pass the mvp4x4 matrix to the vertex shader uniform mvp
	this->modelUnifrom = shdr->getUniformLocation("model");
	this->viewUniform  = shdr->getUniformLocation("view");
	this->projectionUniform = shdr->getUniformLocation("projection");
}
/*
 * Function: setTexture(char *texBitmap)
 * Implementation pending
*/
void _Renderer::setTexture(char *texBitmap)
{

}
/*
* Function: setMatrices(int w,int h)
* sets the Model,View and Projection matrix into the vertex shader,
* Passing information for 
* Model - positon,translation,scaling,rotation
* Camera - postion,translation,rotation, lookat,upDirection 
* Projection - Aspect ratio , Near/Far clipping, Field of view. 
* Used by: the _glWidget class resizeGL()
* Created: 22_02_2019
*/
void _Renderer::setMatrices(int w,int h)
{	
	setModelMatrix(QVector3D(0.0, 0.0, 0.0), 1.0, QQuaternion(QVector3D(0.0, 0.0, 0.0)));
	setCamViewMatrix(QVector3D(0.0, 0.0, 5.0), QVector3D(0.0, 0.0, 0.0), QVector3D(0.0, 1.0, 0.0));
	setProjectionMatrix(w, h, 45.0, 10.0, 2.0);
	//
	glm_model4x4 = glm::mat4();
	glm_view4x4 = glm::lookAt(
		glm::vec3(2.5f, 2.5f, 2.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f));
	glm_projection4x4 = glm::perspective(45.0f, 800.0f / 600.0f, 1.0f, 10.0f);

}
/*
* Function: setModelMatrix 
* Sets the values matrices for the model matrix 
* works in implementing translation , rotation and scaling
* Used by: the _glWidget class initialiseGl() or paintGl() 
* Created: 25_02_2019
*/
void _Renderer::setModelMatrix(QVector3D position,float scale,QQuaternion rotation)
{
	model4x4.translate(position);
	model4x4.scale(scale);
	model4x4.rotate(rotation);
	//
	//glm_model4x4 = glm::translate(glm_model4x4,glm::vec3(position.x(), position.y(), position.z()));
	//glm_model4x4 = glm::rotate(glm_model4x4, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	glm_model4x4 = glm::scale(glm_model4x4, glm::vec3(scale, scale, scale));
}

/*
* Function: setCamViewMatrix()
* sets the camera view for the scene through this matrix
* helps set the camera , eye positon , rotation, lookat.
* Used by: the _glWidget class initialiseGl() or paintGl() 
* depending if the camra needs to update its position in  realtime.
* Created: 25_02_2019
*/
void _Renderer::setCamViewMatrix(QVector3D eyePos,QVector3D focalPoint,QVector3D upVector)
{
	view4x4.lookAt(
		QVector3D(eyePos),//Eye
		QVector3D(focalPoint),// Focal Point
		QVector3D(upVector));// Up vector

	glm_view4x4 = glm::lookAt(
		glm::vec3(eyePos.x(), eyePos.y(), eyePos.z()),
		glm::vec3(focalPoint.x(), focalPoint.y(), focalPoint.z()),
		glm::vec3(upVector.x(), upVector.y(), upVector.z()));

}
/*
* Function: setProjectionMatrix(int w, int h)
* takes thew width and height of the window and sets the relative 
* field of view and the aspect ration bindings. will update itself each time the 
* window is resized.and needs to be called in the resizeGl function.
* Used by: the _glWidget class resizeGL()
* Created: 25_02_2019
*/
void _Renderer::setProjectionMatrix(int resW, int resH, float fov, float zFar, float zNear )
{
	// Calculate aspect ratio
	qreal aspect = qreal(resW) / qreal(resH ? resH : 1);
	// Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
	projection4x4.perspective(fov, aspect, zNear, zFar);
	//
	glm_projection4x4 = glm::perspective(fov, float(aspect), zNear, zFar);
}
/*
* 
*/
float i = 0;
void _Renderer::generateMVP()
{
	i -= 0.0005;
	//float n = abs(cos(timer.elapsed() * 0.0002)) * 0.05;
	glm_model4x4 = glm::translate(glm_model4x4, glm::vec3(0.0,0.0,0.0 ));

}
/*
 * Function: draw()
 * This is your proprietory draw function 
 * Used by: the _glWidget class paintGl()
 * Create:11_02_2019
*/
void _Renderer::_Renderer::draw()
{
	//updates the model matrix as the scene 
	generateMVP();
	//clears the color and depth buffer before new frame draw.
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
	float r = abs(cos(timer.elapsed() * 0.002));
	float g = abs(sin(timer.elapsed() * 0.003));
	float b = abs(cos(timer.elapsed() * 0.005));
	//glmaintex(0,dasasd);
	glUniform4f(colorUniform, r,g,b, 1.0f);

	//----Debug Use
	//i -= 0.01;
	//float a[] = {1.0,0.0, 0.0,  i,
	//			 0.0, 1.0, 0.0, 0,
	//			 0.0, 0.0, 1.0, 0.0,
	//			 0.0, 0.0, 0.0, 1.0};
	//----
	//glUniformMatrix4fv(modelUnifrom, 1, GL_FALSE, model4x4.transposed().data());
	//glUniformMatrix4fv(viewUniform, 1, GL_FALSE, view4x4.data());
	//glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, projection4x4.data());
	//
	glUniformMatrix4fv(modelUnifrom, 1, GL_FALSE, glm::value_ptr(glm_model4x4));
	glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(glm_view4x4));
	glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(glm_projection4x4));
	//
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

