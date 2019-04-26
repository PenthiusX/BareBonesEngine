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
    //    glEnable(GL_DEPTH_TEST);
    //    glEnable(GL_STENCIL_TEST);
    glEnable(GL_FRONT_AND_BACK);
    //    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1f, 0.1f, 0.3f, 1.0);//sets the bckground color of the openglContext.

    shdr = new _Shader();//initialising the _shader() class * object.
    fboShader = new _Shader();
    setShader();//will run this shader by default.
    timer.start();

    glm_projection4x4 = glm::mat4(1.0f);
    glm_model4x4 = glm::mat4(1.0f);
    glm_view4x4 = glm::mat4(1.0f);
    isTranfomationLocal = false;
    isFramebufferActive = false;

    qDebug() << "render initialised ";
}
/*
 *Distructor: _Renderer Class
*/
_Renderer::~_Renderer()
{
    shdr = nullptr;
    fboShader = nullptr;
    delete fboShader;
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
    qDebug() << "default Shader attached for entity" << this->sceneEntity.getId();
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
    qDebug() << "setShader(QString"<<vSh<<", QString"<<fSh<<")" << this->sceneEntity.getId();
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

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    setuniformLocations();//sets all uniform locations to respective variables.
    qDebug() << "setModelDataInBuffers() for entity" << this->sceneEntity.getId();
}
/*
 * Function: setuniformLocations()
 * sets a seperate functional implementation to the existing
 * Created:11_02_2019
*/
void _Renderer::setuniformLocations()
{
    this->colorUniform = shdr->getUniformLocation("aColor");//will be replaced with texture and UVs
    this->modelUnifrom = shdr->getUniformLocation("model");
    this->viewUniform  = shdr->getUniformLocation("view");
    this->projectionUniform = shdr->getUniformLocation("projection");
    this->mousePosUniform = shdr->getUniformLocation("mousePos");
}

/*
 * Function: setupTexture()
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
    qDebug() << "setupTexture() on entity" << this->sceneEntity.getId();
}
void _Renderer::setupTexture(QString texfile)
{
    QImage img = QImage(texfile);
    _Texture texture(img);
    texture.load(GL_RGBA,GL_UNSIGNED_BYTE);
    textures.push_back(texture);
    qDebug() << "setupTexture(QString texfile) on entity" << this->sceneEntity.getId();
}
/*
 * Function: setTexture()
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
    qDebug() << "setTexture(char* texBitmap) on entity" << this->sceneEntity.getId();
}
void _Renderer::setTexture(char* texBitmap,unsigned int iwidth,unsigned int iheight)
{
    if(!textures.empty())
        textures[0].setImage(texBitmap,iwidth,iheight);
    qDebug() << "setTexture(char* texBitmap,unsigned int iwidth,unsigned int iheight) on entity" << this->sceneEntity.getId();
}
void _Renderer::setTexture(QString pathtoTexture)
{
    if(!textures.empty())
        textures[0].setImage(pathtoTexture);
    qDebug() << "setTexture(QString pathtoTexture) on entity" << this->sceneEntity.getId();
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
    translationMatrix = glm::mat4(1.f);
    rotationMatrix = glm::mat4(1.f);
    scalingMatrix = glm::mat4(1.f);

    scalingMatrix = glm::scale(scalingMatrix, glm::vec3(scale, scale, scale));//scale equally on all sides
    glm::vec3 EulerAngles(rotation.x(),rotation.y(),rotation.z());
    glm::quat quat = glm::quat(EulerAngles);
    rotationMatrix = glm::mat4_cast(quat);
    translationMatrix = glm::translate(translationMatrix,glm::vec3(position.x(), position.y(), position.z()));

    glm_model4x4 = translationMatrix * rotationMatrix * scalingMatrix;
    qDebug() << "setModelMatrix() on entity" << this->sceneEntity.getId();
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
    //qDebug() << "setCamViewMatrix() on entity" << this->sceneEntity.getId();
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
    qDebug() << "setProjectionMatrix() on entity" << this->sceneEntity.getId();
}
/*
 * Function: setPosition(QVector3D pos)\translate(QVector3D pos)
 * updates the specific trasformations that affect the model matrix
 * of the matrices of the individual object.In this case the positions
 * Used by: _render class in draw()
 * Created: 1_03_2019
*/
void _Renderer::setPosition(QVector3D pos)
{
    this->isTranfomationLocal = this->sceneEntity.getIsTransfomationLocal();
    if(isTranfomationLocal)
    {
        glm_model4x4 = glm::mat4(1.0f);
        glm_model4x4 = glm::translate(glm_model4x4,glm::vec3(pos.x(), pos.y(), pos.z()));
    }
    else if(!isTranfomationLocal)
    {
        translationMatrix = glm::mat4(1.f);
        translationMatrix = glm::translate(translationMatrix,glm::vec3( pos.x(),
                                                                        pos.y(),
                                                                        pos.z()));
        glm_model4x4 = translationMatrix * rotationMatrix * scalingMatrix;
    }
    this->sceneEntity.setPosition(QVector3D(translationMatrix[3][0], //sets the actual matrix positons to the Entity
            translationMatrix[3][1],
            translationMatrix[3][2]));
}
void _Renderer::translate(QVector3D pos)
{
    this->isTranfomationLocal = this->sceneEntity.getIsTransfomationLocal();
    if(isTranfomationLocal)
    {
        glm_model4x4 = glm::translate(glm_model4x4,glm::vec3(pos.x(), pos.y(), pos.z()));
    }
    else if(!isTranfomationLocal)
    {
        translationMatrix = glm::translate(translationMatrix,glm::vec3(pos.x(), pos.y(), pos.z()));
        glm_model4x4 = translationMatrix * rotationMatrix * scalingMatrix;
    }
    this->sceneEntity.setPosition(QVector3D(translationMatrix[3][0], //sets the actual matrix positons to the Entity
            translationMatrix[3][1],
            translationMatrix[3][2]));
}
/*
 * Function: setRotation(QVector3D pos)
 * updates the specific trasformations that affect the model matrix
 * of the matrices of the individual object.In this case the rotation
 * Used by: _render class in draw()
 * Created: 1_03_2019
*/
void _Renderer::setRotation(QVector3D rot)
{
    this->sceneEntity.setRotation(rot);
    this->isTranfomationLocal = this->sceneEntity.getIsTransfomationLocal();
    if(isTranfomationLocal)
    {//still buggy
        glm::vec3 EulerAngles(this->sceneEntity.getRotation().x(),
                              this->sceneEntity.getRotation().y(),
                              this->sceneEntity.getRotation().z());
        glm::quat quat = glm::quat(EulerAngles);
        glm_model4x4 *= glm::mat4_cast(quat);
    }
    else if(!isTranfomationLocal)
    {
        rotationMatrix = glm::mat4x4(1.f);
        glm::vec3 EulerAngles(this->sceneEntity.getRotation().x(),
                              this->sceneEntity.getRotation().y(),
                              this->sceneEntity.getRotation().z());
        glm::quat quat = glm::quat(EulerAngles);
        rotationMatrix = glm::mat4_cast(quat);
        glm_model4x4 =  translationMatrix * rotationMatrix * scalingMatrix;
    }
}
/*
 * Function: setscale(float scale)
 * updates the specific trasformations that affect the model matrix
 * of the matrices of the individual object.In this case the scale
 * Used by: _render class in draw()
 * Created: 1_03_2019
*/
void _Renderer::setscale(float scale)
{
    this->sceneEntity.setScale(scale);//reimplemnt
    scalingMatrix = glm::mat4(1.f);
    scalingMatrix = glm::scale(scalingMatrix, glm::vec3(this->sceneEntity.getScale(),//scale eqally on all axis(dont need respective sclaing)
                                                        this->sceneEntity.getScale(),
                                                        this->sceneEntity.getScale()));
    glm_model4x4 = translationMatrix * rotationMatrix * scalingMatrix;
}
/*
* Function: setSceneEntity(_SceneEntity s)
* Sets the sceen entity object locally and sets the
* shader ,Model data , projection matrix, texture,
* and initialises the modelMatrix.
* Created: 1_03_2019
*/
void _Renderer::setSceneEntityInRenderer(_SceneEntity s)
{
    this->sceneEntity = s;
    this->isTranfomationLocal = s.getIsTransfomationLocal();
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
 * Function: setFrameBuffer()
 * Created:24_04_2019
 */
void _Renderer::setFrameBuffer(unsigned int resW, unsigned int resH)
{
    isFramebufferActive = true;
    qDebug() << "----------Frambuffer Enabled for ID----------------" << this->sceneEntity.getId();
    // framebuffer configuration
    //---------------------------------------------------------------------------------------
    glGenFramebuffers(1, &frameBuffer1);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer1);

    // create a color attachment texture
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, resW, resH, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, resW, resH); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it

    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        qDebug() << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;}else{qDebug() << "Frambuffer is Initialisation Complete" ;}
    //reset to the default framebuffer 0
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //---------------------------------------FBOQUAD--------------------------------------------
    fboShader->attachShaders(":/shaders/texVshader.glsl", ":/shaders/texFshader.glsl");
    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
                             // positions   // texCoords
                             -0.5f,  0.5f,  0.0f, 1.0f,
                             -0.5f, -0.5f,  0.0f, 0.0f,
                             0.5f, -0.5f,  1.0f, 0.0f,

                             -0.5f,  0.5f,  0.0f, 1.0f,
                             0.5f, -0.5f,  1.0f, 0.0f,
                             0.5f,  0.5f,  1.0f, 1.0f
                           };
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    //---------------------------------------------------------------------------------------------
}

/*
 * Function: draw()
 * This is your proprietory draw function
 * Draws frames on a avg of 60frames per second(is subjective and changes with hardware)
 * Used by: the _glWidget class paintGl().
 * Created:11_02_2019
*/
void _Renderer::_Renderer::draw()
{
    //----------------Framebuffer test-----------------------------
    //default is 0 (your scene) , genrated is frameBuffer1
    if(isFramebufferActive){
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer1);
        glEnable(GL_DEPTH_TEST | GL_STENCIL_TEST);}// enable depth and stencil testing (is disabled for rendering screen-space quad)
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //-------------------------------------------------------------
    //Using the shader program in the current context
    shdr->useShaderProgram();
    //Bind Textures
    for(unsigned int t=0;t<textures.size();t++){
        textures[t].bind();
    }
    //Bind the Buffers data of the respective buffer object
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    //Sets the values for the MVP matrix in the vertex shader
    glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(glm_view4x4));
    glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(glm_projection4x4));
    glUniformMatrix4fv(modelUnifrom, 1, GL_FALSE, glm::value_ptr(glm_model4x4));
    transitionColors(QVector2D(0.0,0.0));//Setting the uniform for color trnasitioning//just a temporary debug use
    //The Final draw call for each frame
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, nullptr);
    //----------------Framebuffer test--------------------
    if(isFramebufferActive){
        // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST | GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        fboShader->useShaderProgram();
        glBindVertexArray(quadVAO);

        glBindTexture(GL_TEXTURE_2D,textureColorbuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);}	// use the color attachment texture as the texture of the quad plane
    //----------------------------------------------------
    //    glUseProgram(0);//Reset shader ??
}

/*
 * Temporary debugging implemetation, trasitions colors of object
 * hot reloding of shaders also needs to be implemented
*/
void _Renderer::transitionColors(QVector2D mousePos)
{
    float x = mousePos.x();
    float y = mousePos.y();
    x = x / 824;//limiting the value between 0 and 1
    y = y / 375;//limiting the value between 0 and 1
    double r = abs(cos(timer.elapsed() * 0.002));
    double g = abs(sin(timer.elapsed() * 0.003));
    double b = abs(cos(timer.elapsed() * 0.005));
    glUniform4f(colorUniform, r, g, b, 1.0f);//will be replaced by Texture
    glUniform2f(mousePosUniform,x,y);//passing mouse value to shader
}
/*
 *
*/
void _Renderer::unProject(QVector2D mousePressPosition)
{
    // Where The Viewport Values Will Be Stored
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);           // Retrieves The Viewport Values (X, Y, Width, Height)

    for(int i = 0 ; i < 4 ;i++){
        qDebug() << viewport[i];
    }

    // Holds Our X, Y and Z Coordinates
    GLfloat winX, winY, winZ;
    winX = (float)mousePressPosition.x();                  // Holds The Mouse X Coordinate
    winY = (float)mousePressPosition.y();                  // Holds The Mouse Y Coordinate
    qDebug() << mousePressPosition;
    GLbyte color[4];
    GLfloat depth;
    GLuint index;

    glFlush();
    winY = viewport[3] - winY - 1;// Subtract The Current Mouse Y Coordinate From The Screen Height.
    glReadPixels(winX, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
    qDebug() << glGetError();;
    qDebug() << mousePressPosition <<"-depthz"<< winZ;
    glReadPixels(winX, winY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
    qDebug() << glGetError();
    qDebug() << mousePressPosition <<"-color"<< color[0]<<color[1]<<color[2]<<color[3];
    glReadPixels(winX, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    qDebug() << glGetError();
    qDebug() << mousePressPosition <<"-depth"<< depth;
    glReadPixels(winX, winY, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);
    qDebug() << glGetError();
    qDebug() << mousePressPosition <<"-index"<< index;

    //        glm::vec3 mPos = glm::unProject(
    //                    glm::vec3(x, float(this->width()) - y, 1.0f),
    //                    glm::mat4(1.0f),
    //                    this->glm_projection4x4,
    //                    glm::vec4(0.0f, 0.0f, float(mWindowWidth), float(mWindowHeight))
    //                 );

    //      ROUND 2 DECIMAL
    //        posX= ((int)(posX * 10 + .5) / 10.0);
    //        posY= ((int)(posY * 10+ .5) / 10.0);
    //        posZ= ((int)(posZ * 10 + .5) / 10.0);
}
