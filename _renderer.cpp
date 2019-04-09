#include "_renderer.h"
#include <iostream>
#include <_tools.h>
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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_FRONT_AND_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1f, 0.1f, 0.3f, 1.0);//sets the bckground color of the openglContext.
	//
	shdr = new _Shader();//initialising the _shader() class * object.
	setShader();//will run this shader by default.
	timer.start();
	//
	glm_projection4x4 = glm::mat4(1.0f);
    glm_model4x4 = glm::mat4(1.0f);
    glm_view4x4 = glm::mat4(1.0f);
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
 * Sets a dafault hard-fed shader
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
void _Renderer::setModelDataInBuffers(std::vector<float> vertexArray, std::vector<unsigned int> indexArray)
{
	// Copy the vertex and index data locally for use in the current drawcall.
	this->vertices = vertexArray;
	this->indices = indexArray;
    // Initialization code (done once (unless your object frequently changes))
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    // 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof (float), &vertices[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);
    //
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//
    this->colorUniform = shdr->getUniformLocation("aColor");//will be replaced with texture and UVs
	this->modelUnifrom = shdr->getUniformLocation("model");
	this->viewUniform  = shdr->getUniformLocation("view");  
	this->projectionUniform = shdr->getUniformLocation("projection");
}
/*
 * Function: setupTexture()
 * Contributor : Saurabh
 * creates new texture and adds into list(vector) of textures
 * current context should be active while calling these functions
 * use makeCurrent() to make context current
 * set a default 8bit single color texture of size 1360 x 1024
 * Created: 2_3_2019
 */
void _Renderer::setupTexture()
{
    char* img = new char[1360*1024];
    _Texture texture(img,1360,1024);
    texture.load(GL_RED,GL_UNSIGNED_BYTE);
    textures.push_back(texture);
}

/*
 * Function: setupTexture()
 * Contributor : Aditya
 * creates new texture from texfile image path and adds into list(vector) of textures
 * current context should be active while calling these function
 * Created: 28_3_2019
 */
void _Renderer::setupTexture(QString texfile)
{
    QImage img = QImage(texfile);
    _Texture texture(img);
    texture.load(GL_RGBA,GL_UNSIGNED_BYTE);
    textures.push_back(texture);
}

/*
 * Function: setTexture(char* texBitmap)
 * Contributor : saurabh
 * updates the first texture image from char pointer array
 * resolution of previous image is used
 * current context should be active while calling this function
 * Created: 2_3_2019
 */
void _Renderer::setTexture(char* texBitmap)
{
    if(!textures.empty())
        textures[0].setImage(texBitmap);
}

/*
 * Function: setTexture(char* texBitmap,unsigned int iwidth,unsigned int iheight)
 * Contributor : Saurabh
 * updates the first texture image from char pointer array
 * resolution of texture is updated to given values
 * current context should be active while calling this function
 * Created: 2_3_2019
 */
void _Renderer::setTexture(char* texBitmap,unsigned int iwidth,unsigned int iheight)
{
    if(!textures.empty())
        textures[0].setImage(texBitmap,iwidth,iheight);
}

/*
 * Function: setTexture(QString pathtoTexture)
 * Contributor : Saurabh
 * updates the first texture image from a texfile
 * current context should be active while calling this function
 * Created: 2_3_2019
 */
void _Renderer::setTexture(QString pathtoTexture)
{
     if(!textures.empty())
     textures[0].setImage(pathtoTexture);
}

/*
* Function: setModelMatrix(QVector3D position,float scale,QQuaternion rotation)
* Sets the values matrices for the model matrix 
* works in implementing translation , rotation and scaling
* Used by: the _glWidget class initialiseGl() or paintGl().
* Created: 25_02_2019
*/
void _Renderer::setModelMatrix(QVector3D position,float scale,QVector3D rotation)
{
    glm_model4x4 = glm::mat4(1.0f);
    glm_model4x4 = glm::scale(glm_model4x4, glm::vec3(scale, scale, scale));//scale equally on all sides
    glm::vec3 EulerAngles(rotation.x(),rotation.y(),rotation.z());
    glm::quat quat = glm::quat(EulerAngles);
    glm_model4x4 *= glm::mat4_cast(quat);
    glm_model4x4 = glm::translate(glm_model4x4,glm::vec3(position.x(), position.y(), position.z()));
}

/*
* Function: setCamViewMatrix(QVector3D eyePos,QVector3D focalPoint,QVector3D upVector)
* sets the camera view for the scene through this matrix
* helps set the camera , eye positon , rotation, lookat.
* Used by: the _glWidget class initialiseGl() or paintGl().
* depending if the camra needs to update its position in  realtime.
* Created: 25_02_2019
*/
void _Renderer::setCamViewMatrix(QVector3D eyePos,QVector3D focalPoint,QVector3D upVector)
{
	glm_view4x4 = glm::mat4(1.0f);
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
* Used by: the _glWidget class resizeGL().
* Created: 25_02_2019
*/
void _Renderer::setProjectionMatrix(int resW, int resH, float fov, float zNear, float zFar )
{
	// Calculate aspect ratio
    float aspect = float(resW) / float(resH ? resH : 1);
    glm_projection4x4 = glm::perspective(glm::radians(fov), float(aspect), zNear, zFar);
}
/*
 * Function: setPosition/Rotation/Scale/Translate
 * updates the specific trasformations that affect the model matrix
 * of the matrices of the individual object.
 * Used by: _render class in draw()
 * Created: 1_03_2019
*/
void _Renderer::setPosition(QVector3D pos)
{
    glm_model4x4 = glm::mat4(1.f);
    glm_model4x4 = glm::translate(glm_model4x4,glm::vec3(pos.x(), pos.y(), pos.z()));
    glm_model4x4 = glm::scale(glm_model4x4, glm::vec3(sceneEntity.getScale()));
    sceneEntity.setPosition(pos);
}
void _Renderer::translate(QVector3D pos)
{
    glm_model4x4 = glm::translate(glm_model4x4, glm::vec3(pos.x(),pos.y(), pos.z()));
    this->sceneEntity.setPosition(sceneEntity.getPostion() + pos);
}
void _Renderer::rotate(QVector3D rot)
{
    //Quat
    this->sceneEntity.setRotation(this->sceneEntity.getRotation() + rot);
    glm::vec3 EulerAngles(rot.x(),rot.y(),rot.z());
    glm::quat quat = glm::quat(EulerAngles);
    glm_model4x4 *= glm::mat4_cast(quat);
    //Euler
//  glm_model4x4 = glm::rotate(glm_model4x4, glm::radians(45.0f), glm::vec3(rot.x(),rot.y(),rot.z()));
}
void _Renderer::scale(float scale)
{
    glm_model4x4 = glm::mat4(1.f);
    this->sceneEntity.setScale(scale);
    glm_model4x4 = glm::scale(glm_model4x4, glm::vec3(sceneEntity.getScale(), sceneEntity.getScale(), sceneEntity.getScale()));//scale equally on all sides
}
/*
* Function: setSceneEntity(_SceneEntity s)
* Sets the sceen entity object locally and sets the 
* Model and projection matrix as well 
* Created: 1_03_2019
*/
void _Renderer::setSceneEntityInRenderer(_SceneEntity s)
{
	this->sceneEntity = s;	
	setShader(s.getVertexShaderPath(), s.getFragmentShaderPath());
    setupTexture(s.getTexturePath());
    setModelDataInBuffers(s.getvertexData(), s.getIndexData());
	setModelMatrix(s.getPostion(), s.getScale(), s.getRotation());
}
/*
* Function: getSceneEntity()
* returns the current scene entity object.
* Created:11_02_2019
*/
_SceneEntity _Renderer::getSceneEntity()
{
	return this->sceneEntity;
}
/*
 * Function: draw()
 * This is your proprietory draw function 
 * Used by: the _glWidget class paintGl().
 * Created:11_02_2019
*/
void _Renderer::_Renderer::draw()
{ 
    //Using the shader program in the current context
    //can be called once in the init or every frame
    //if the shader is switching between objects
    shdr->useShaderProgram();
	//Setting the uniform each frame.
	//Depends if the need is to update the values
    double r = abs(cos(timer.elapsed() * 0.002));
    double g = abs(sin(timer.elapsed() * 0.003));
    double b = abs(cos(timer.elapsed() * 0.005));
	glUniform4f(colorUniform, r, g, b, 1.0f);//will be replaced by Texture
    //bind all textures-->dosent look like more than 1 texture is being pushedback in the vector needs work
    for(unsigned int t=0;t<textures.size();t++)
    {
        textures[t].bind();
    }
    //Bind the Buffers data of the respective buffer object
    //in the context each frame.
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	//sets the values for the MVP matrix in the vertex shader
	glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(glm_view4x4));
	glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(glm_projection4x4));
    glUniformMatrix4fv(modelUnifrom, 1, GL_FALSE, glm::value_ptr(glm_model4x4));
	//The Final draw call for each frame
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, nullptr);
}

