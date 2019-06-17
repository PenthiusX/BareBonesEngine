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
    glEnable(GL_FRONT_AND_BACK);//shows bot back and front of the model
    glEnable(GL_BLEND);//for transparency in alpha values
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.1f, 0.1f, 0.3f, 1.0);//sets the bckground color of the openglContext.
    //
    shdr = new _Shader();//initialising the _shader() class * object.
    fboShader = new _Shader();
    setShader();//will run this shader by default.
    timer.start();
    //
    glm_projection4x4 = glm::mat4(1.0f);
    glm_model4x4 = glm::mat4(1.0f);
    glm_view4x4 = glm::mat4(1.0f);
    pivotTmat = glm::mat4(1.0f);
    qDebug() << "render initialised ";
}
/*
 *Distructor: _Renderer Class
*/
_Renderer::~_Renderer()
{
    delete fboShader;
    delete shdr;
}
/*
* Function: getSceneEntity()
* returns the current scene entity object.
* Created:11_02_2019
*/
_SceneEntity _Renderer::getSceneEntity() const
{
    return this->sceneEntity;
}
/*
  ▪   ▐ ▄ ▪  ▄▄▄▄▄▪   ▄▄▄· ▄▄▌  ▪  ·▄▄▄▄•▄▄▄ .
  ██ •█▌▐███ •██  ██ ▐█ ▀█ ██•  ██ ▪▀·.█▌▀▄.▀·
  ▐█·▐█▐▐▌▐█· ▐█.▪▐█·▄█▀▀█ ██▪  ▐█·▄█▀▀▀•▐▀▀▪▄
  ▐█▌██▐█▌▐█▌ ▐█▌·▐█▌▐█ ▪▐▌▐█▌▐▌▐█▌█▌▪▄█▀▐█▄▄▌
  ▀▀▀▀▀ █▪▀▀▀ ▀▀▀ ▀▀▀ ▀  ▀ .▀▀▀ ▀▀▀·▀▀▀ • ▀▀▀
*/
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
    qDebug() << "default Shader attached for entity" << this->sceneEntity.getTag();
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
    qDebug() << "setShader(QString"<<vSh<<", QString"<<fSh<<")" << this->sceneEntity.getTag();;
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
    qDebug() << "VBO-" << VBO << "VAO-" << VAO << "EBO-" << EBO;
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    setuniformLocations();//sets all uniform locations to respective variables.
    qDebug() << "setModelDataInBuffers() for entity" << this->sceneEntity.getTag();
}
/*
 * Function: setuniformLocations()
 * sets the unform location uints into there respectively
 * named variables. These variables are used based on definition in shader.
 * Created:11_02_2019
*/
void _Renderer::setuniformLocations()
{
    qDebug() <<"--------"<<this->sceneEntity.getTag() <<"--------";
    this->colorUniform = shdr->getUniformLocation("aColor");//will be replaced with texture and UVs
    qDebug() << "colorUniform ->" << colorUniform;
    this->modelUnifrom = shdr->getUniformLocation("model");
    qDebug() << "modelUnifrom ->" << modelUnifrom;
    this->viewUniform  = shdr->getUniformLocation("view");
    qDebug() << "viewUniform ->" << viewUniform;
    this->projectionUniform = shdr->getUniformLocation("projection");
    qDebug() << "projectionUniform ->" << projectionUniform;
    this->mousePosUniform = shdr->getUniformLocation("iMouse");
    qDebug() << "mousePosUniform ->" << mousePosUniform;
    qDebug() <<"-----------------------------------------";
}
/*
 * Function: setupTexture()
 * creates new texture and adds into list(vector) of textures
 * set a default 8bit single color texture of size 1360 x 1024
 * Created: 2_3_2019
 */
void _Renderer::setupTexture()
{
    char* img = new char[1360*1024];
    _Texture texture(img,1360,1024);
    texture.load(GL_RED,GL_UNSIGNED_BYTE);
    textures.push_back(texture);
    qDebug() << "setupTexture() on entity" << this->sceneEntity.getTag();
}
void _Renderer::setupTexture(QString texfile)
{
    QImage img = QImage(texfile);
    _Texture texture(img);
    texture.load(GL_RGBA,GL_UNSIGNED_BYTE);
    textures.push_back(texture);
    qDebug() << "setupTexture(QString texfile) on entity" << this->sceneEntity.getTag();
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
    qDebug() << "setTexture(char* texBitmap) on entity" << this->sceneEntity.getTag();
}
void _Renderer::setTexture(char* texBitmap,unsigned int iwidth,unsigned int iheight)
{
    if(!textures.empty())
        textures[0].setImage(texBitmap,iwidth,iheight);
    qDebug() << "setTexture(char* texBitmap,unsigned int iwidth,unsigned int iheight) on entity" << this->sceneEntity.getTag();
}
void _Renderer::setTexture(QString pathtoTexture)
{
    if(!textures.empty())
        textures[0].setImage(pathtoTexture);
    qDebug() << "setTexture(QString pathtoTexture) on entity" << this->sceneEntity.getTag();
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
    qDebug() << "setModelMatrix() on entity" << this->sceneEntity.getTag();
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
    keepSceneEntityUpdated();
}
/*
        ▐ ▄     ▄▄▄  ▄▄▄ ..▄▄ · ▪  ·▄▄▄▄•▄▄▄ .
 ▪     •█▌▐█    ▀▄ █·▀▄.▀·▐█ ▀. ██ ▪▀·.█▌▀▄.▀·
  ▄█▀▄ ▐█▐▐▌    ▐▀▀▄ ▐▀▀▪▄▄▀▀▀█▄▐█·▄█▀▀▀•▐▀▀▪▄
 ▐█▌.▐▌██▐█▌    ▐█•█▌▐█▄▄▌▐█▄▪▐█▐█▌█▌▪▄█▀▐█▄▄▌
  ▀█▄▀▪▀▀ █▪    .▀  ▀ ▀▀▀  ▀▀▀▀ ▀▀▀·▀▀▀ • ▀▀▀
*/
/*
* Function: setProjectionMatrix(int w, int h)
* takes thew width and height of the window and sets the relative
* field of view and the aspect ration bindings. will update itself each time the
* window is resized.and needs to be called in the resizeGl function.
* Used by: the _glWidget class resizeGL().
* Created: 25_02_2019
*/
void _Renderer::setProjectionMatrix(int resW, int resH, float fov, float zNear, float zFar)
{
    // Calculate aspect ratio
    float aspect = float(resW) / float(resH ? resH : 1);
    glm_projection4x4 = glm::perspective(glm::radians(fov), float(aspect), zNear, zFar);

    qDebug() << "setProjectionMatrix() on entity" << this->sceneEntity.getTag();
}
/*
 ▄▄▄▄▄▄▄▄   ▄▄▄·  ▐ ▄ .▄▄ · ·▄▄▄      ▄▄▄  • ▌ ▄ ·.
 •██  ▀▄ █·▐█ ▀█ •█▌▐█▐█ ▀. ▐▄▄·▪     ▀▄ █··██ ▐███▪
  ▐█.▪▐▀▀▄ ▄█▀▀█ ▐█▐▐▌▄▀▀▀█▄██▪  ▄█▀▄ ▐▀▀▄ ▐█ ▌▐▌▐█·
  ▐█▌·▐█•█▌▐█ ▪▐▌██▐█▌▐█▄▪▐███▌.▐█▌.▐▌▐█•█▌██ ██▌▐█▌
  ▀▀▀ .▀  ▀ ▀  ▀ ▀▀ █▪ ▀▀▀▀ ▀▀▀  ▀█▄▀▪.▀  ▀▀▀  █▪▀▀▀
*/
/*
 * Function: setPosition(QVector3D pos)\translate(QVector3D pos)
 * updates the specific trasformations that affect the model matrix
 * of the matrices of the individual object.In this case the positions
 * Used by: _render class in draw()
 * Created: 1_03_2019
*/
void _Renderer::setPosition(QVector3D pos)
{
    if(this->sceneEntity.getIsTransformationAllowed())
    {
        if(this->sceneEntity.getIsTransformationLocal())
        {
            glm_model4x4 = glm::mat4(1.0f);
            glm_model4x4 = glm::translate(glm_model4x4,glm::vec3(pos.x(), pos.y(), pos.z()));
        }
        else if(!this->sceneEntity.getIsTransformationLocal())
        {
            translationMatrix = glm::mat4(1.f);
            translationMatrix = glm::translate(translationMatrix,glm::vec3( pos.x(),
                                                                            pos.y(),
                                                                            pos.z()));
            glm_model4x4 = translationMatrix * rotationMatrix * scalingMatrix;
        }
        keepSceneEntityUpdated();
    }
}
void _Renderer::translate(QVector3D pos)
{
    if(this->sceneEntity.getIsTransformationAllowed())
    {
        //update the traformation matrix with the current values
        setPosition(sceneEntity.getPostion());
        if(this->sceneEntity.getIsTransformationLocal())
        {
            //        glm_model4x4 *= translationMatrix;
            glm_model4x4 = glm::translate(glm_model4x4,glm::vec3(pos.x(), pos.y(), pos.z()));
        }
        else if(!this->sceneEntity.getIsTransformationLocal())
        {
            translationMatrix = glm::translate(translationMatrix,glm::vec3(pos.x(), pos.y(), pos.z()));
            glm_model4x4 = translationMatrix * rotationMatrix * scalingMatrix;
        }
        keepSceneEntityUpdated();
    }
}
/*
 * Function: setRotation(QVector3D pos)
 * updates the specific trasformations that affect the model matrix
 * of the matrices of the individual object.In this case the rotation
 * Used by: _glwidgetClass on mousemovement
 * Created: 1_03_2019
*/
void _Renderer::setRotation(QVector3D rot)
{
    if(this->sceneEntity.getIsTransformationAllowed())
    {
        if(this->sceneEntity.getIsPivotSet() == false)
        {
            this->sceneEntity.setRotation(rot);
            if(this->sceneEntity.getIsTransformationLocal())
            {
                glm::vec3 EulerAngles(this->sceneEntity.getRotation().x(),
                                      this->sceneEntity.getRotation().y(),
                                      this->sceneEntity.getRotation().z());
                glm::quat quat = glm::quat(EulerAngles);
                glm_model4x4 *= glm::mat4_cast(quat);
            }
            else if(!this->sceneEntity.getIsTransformationLocal())
            {
                //        rotationMatrix = glm::mat4x4(1.f);
                glm::vec3 EulerAngles(this->sceneEntity.getRotation().x(),
                                      this->sceneEntity.getRotation().y(),
                                      this->sceneEntity.getRotation().z());
                glm::quat quat = glm::quat(EulerAngles);
                rotationMatrix = glm::mat4_cast(quat);
                //rotate at center
                glm_model4x4 =  translationMatrix * rotationMatrix * scalingMatrix;
            }
        }
        else if(this->sceneEntity.getIsPivotSet() == true)
        {
            setRotationAroundPivot(rot, this->sceneEntity.getPivot());
        }
        keepSceneEntityUpdated();
    }
}
/*
 * Function: setRotationAroundPivot(QVector3D rot, QVector3D pivot)
 * sets the rotation to be around an defined point
 * Used by: _glwidgetClass on Mousemovement
 * Created: 16_05_2019
*/
void _Renderer::setRotationAroundPivot(QVector3D rot, QVector3D pivot)
{
    if(this->sceneEntity.getIsTransformationAllowed())
    {
        this->sceneEntity.setRotation(rot);
        if(this->sceneEntity.getIsTransformationLocal())
        {//still buggy
            setPosition(pivot);
            glm::vec3 EulerAngles(this->sceneEntity.getRotation().x(),
                                  this->sceneEntity.getRotation().y(),
                                  this->sceneEntity.getRotation().z());
            glm::quat quat = glm::quat(EulerAngles);
            glm_model4x4 *= glm::mat4_cast(quat);
        }
        if(!this->sceneEntity.getIsTransformationLocal())
        {
            pivotTmat = glm::mat4x4(1.0f);//this works like an ofsetpivot rather than rotae around a point (need to fix)
            pivotTmat[3][0] = pivot.x();
            pivotTmat[3][1] = pivot.y();
            pivotTmat[3][2] = pivot.z();
            glm::vec3 EulerAngles(this->sceneEntity.getRotation().x(),
                                  this->sceneEntity.getRotation().y(),
                                  this->sceneEntity.getRotation().z());
            glm::quat quat = glm::quat(EulerAngles);
            rotationMatrix = glm::mat4_cast(quat);
            glm_model4x4 = translationMatrix * rotationMatrix * pivotTmat * scalingMatrix;
        }
        keepSceneEntityUpdated();
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
    if(this->sceneEntity.getIsTransformationAllowed())
    {
        this->sceneEntity.setScale(scale);//reimplemnt
        scalingMatrix = glm::mat4(1.f);
        //scale eqally on all axis(dont need respective sclaing)
        scalingMatrix = glm::scale(scalingMatrix, glm::vec3(this->sceneEntity.getScale(),
                                                            this->sceneEntity.getScale(),
                                                            this->sceneEntity.getScale()));
        glm_model4x4 = translationMatrix * rotationMatrix * scalingMatrix;
    }
}
/*
* Function: setSceneEntity(_SceneEntity s)
* Sets the sceen entity object locally and sets the
* shader ,Model data , projection matrix, texture,
* and initialises the modelMatrix.
* Created: 1_03_2019
*/
void _Renderer::initSceneEntityInRenderer(_SceneEntity s)
{
    this->sceneEntity = s;
    actualColor = this->sceneEntity.getColor();
    setShader(s.getVertexShaderPath(), s.getFragmentShaderPath());
    setupTexture(s.getTexturePath());
    //setModelDataInBuffers() happens for every object,and is sufficent for the usecases // !!!Comment for structural change!!!!
    //can be converted to using the same VAO for the same set of vertex+index data.
    //will need to move the whole model loading and id geenration to assetLoader class
    //and only pass the relavant ids to VAO at runtime to reduce ovehead.
    setModelDataInBuffers(s.getModelInfo().getVerticexArray(), s.getModelInfo().getIndexArray());
    setModelMatrix(s.getPostion(), s.getScale(), s.getRotation());
}
/*
 * Created: 1_03_2019
*/
void _Renderer::setSceneEntityInRenderer(_SceneEntity s){
    this->sceneEntity = s;
}
/*
 * Created: 2_03_2019
*/
void _Renderer::keepSceneEntityUpdated(){
    //Keeps a copy of the current matrix info in the respective sceneEntity
    this->sceneEntity.setTranslationMatrix(this->translationMatrix);
    this->sceneEntity.setRotationmatrix(this->rotationMatrix);
    this->sceneEntity.setScaleingMatrix(this->scalingMatrix);
    this->sceneEntity.setProjectionMatrix(this->glm_projection4x4);
    this->sceneEntity.setViewMatrix(this->glm_view4x4);
    this->sceneEntity.setModelMatrix(this->glm_model4x4);

    //get the real position values from the modelMatrix
    glm::mat4x4 tmat4 = glm_model4x4 * glm::inverse(rotationMatrix) * glm::inverse(scalingMatrix);
    this->sceneEntity.setPosition(QVector3D(tmat4[3][0],
            tmat4[3][1],
            tmat4[3][2]));
    //    qDebug()<< tmat4[3][0] <<tmat4[3][1] << tmat4[3][2];
}
/*
  ·▄▄▄▄  ▄▄▄   ▄▄▄· ▄▄▌ ▐ ▄▌
  ██▪ ██ ▀▄ █·▐█ ▀█ ██· █▌▐█
  ▐█· ▐█▌▐▀▀▄ ▄█▀▀█ ██▪▐█▐▐▌
  ██. ██ ▐█•█▌▐█ ▪▐▌▐█▌██▐█▌
  ▀▀▀▀▀• .▀  ▀ ▀  ▀  ▀▀▀▀ ▀▪
*/
/*
 * Function: draw()
 * This is your proprietory draw function
 * Draws frames on a avg of 60frames per second(is subjective and changes with hardware)
 * Used by: the _glWidget class paintGl().
 * Created:11_02_2019
*/
void _Renderer::_Renderer::draw()
{
    if(this->sceneEntity.getIsLineMode())
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    else if(!this->sceneEntity.getIsLineMode())
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    if(this->sceneEntity.getIsActive())
    {
        //Using the shader program in the current context
        shdr->useShaderProgram();
        //Bind Textures
        for(unsigned int t=0;t<textures.size();t++){
            textures[t].bind();
        }
        //Bind the Buffers data of the respective buffer object(only needed if mesh need chenging on runtime)
        if(this->sceneEntity.getIsMeshEditable()){
            glBindBuffer(GL_ARRAY_BUFFER,VBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);}
        //Bind the VAO of the respective buffer object (needs to be bound everytime)
        glBindVertexArray(VAO);
        //
        //Sets the values for the MVP matrix in the vertex shader
        glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(this->sceneEntity.getViewMatrix()));
        glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(this->sceneEntity.getProjectionMatrix()));
        glUniformMatrix4fv(modelUnifrom, 1, GL_FALSE, glm::value_ptr(this->sceneEntity.getTranslationMatrix()*
                                                                     this->sceneEntity.getRotationmatrix()*
                                                                     this->pivotTmat *
                                                                     this->sceneEntity.getScaleingMatrix()));
        //
        setColors();//Setting the uniform for color
        //
        glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, nullptr);//The Final draw call for each frame
        glBindVertexArray(0);//Clear the buffer
    }
}
/*
 *  Used in the Draw functon
 * Updates the color to the newly defined one;
 */
void _Renderer::setColors()
{
    glUniform4f(colorUniform, this->sceneEntity.getColor().x(),this->sceneEntity.getColor().y(), this->sceneEntity.getColor().z(), this->sceneEntity.getColor().w());
    //set sfidex color attributes for defined objects
    if(this->sceneEntity.getId() == 999)//pivot
    {
        QVector4D col = this->sceneEntity.getColor();
        col.setX(col.x() + abs(cos(timer.elapsed() * 0.002)));
        col.setY(col.y() + abs(cos(timer.elapsed() * 0.003)));
        col.setZ(col.z() + abs(cos(timer.elapsed() * 0.005)));
        glUniform4f(colorUniform, col.x(),col.y(), col.z(), col.w());
    }
    if(this->sceneEntity.getId() == 888)//pivot
    {
        QVector4D col = this->sceneEntity.getColor();
        col.setX(col.x() + abs(cos(timer.elapsed() * 0.04)));
        col.setY(col.y() + abs(cos(timer.elapsed() * 0.03)));
        col.setZ(col.z() + abs(cos(timer.elapsed() * 0.05)));
        glUniform4f(colorUniform, col.x(),col.y(), col.z(), col.w());
    }
    if( this->sceneEntity.getisHitByRay())
        this->sceneEntity.setColor(actualColor * 0.5);
    else
        this->sceneEntity.setColor(actualColor);
}
