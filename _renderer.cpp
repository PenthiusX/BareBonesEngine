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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//clear for goodmeasure
    glClearColor(0.1f, 0.1f, 0.3f, 1.0);//sets the bckground color of the openglContext.
    //
    shdr = new _Shader();//initialising the _shader() class * object.
    fboShader = new _Shader();
    setShader();//will run this shader by default.
    timer.start();
    //
    projectionMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    viewMatrix = glm::mat4(1.0f);
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
    return sceneEntity;
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
    qDebug() << "default Shader attached for entity" << sceneEntity.getTag();
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
    qDebug() << "setShader(QString"<<vSh<<", QString"<<fSh<<")" << sceneEntity.getTag();;
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
void _Renderer::setModelDataInBuffers(std::vector<float> vertexArray, std::vector<uint> indexArray)
{
    // Copy the vertex and index data locally for use in the current drawcall.
    vertices = vertexArray;
    indices = indexArray;
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
    qDebug() << "setModelDataInBuffers() for entity" << sceneEntity.getTag();
}
/*
 * Function: setuniformLocations()
 * sets the unform location uints into there respectively
 * named variables. These variables are used based on definition in shader.
 * Created:11_02_2019
*/
void _Renderer::setuniformLocations()
{
    qInfo() << "---------------UNIFORM INFO------------------------";
    qDebug() <<"Tag ->"<<sceneEntity.getTag();
    colorUniform = shdr->getUniformLocation("aColor");//will be replaced with texture and UVs
    qDebug() << "colorUniform ->" << colorUniform;
    modelUnifrom = shdr->getUniformLocation("model");
    qDebug() << "modelUnifrom ->" << modelUnifrom;
    viewUniform  = shdr->getUniformLocation("view");
    qDebug() << "viewUniform ->" << viewUniform;
    projectionUniform = shdr->getUniformLocation("projection");
    qDebug() << "projectionUniform ->" << projectionUniform;
    mousePosUniform = shdr->getUniformLocation("iMouse");
    qDebug() << "mousePosUniform ->" << mousePosUniform;
    aspectUnifrom = shdr->getUniformLocation("aspect");
    qDebug() << "aspectUniform ->" << aspectUnifrom;
    qDebug() <<"---------------------------------------------------";
    qDebug() <<"---------------------------------------------------";
}
/*
 * Function: setupTexture(char* img,unsigned int width,unsigned int height,GLenum format=GL_RED)
 * Created: 28_3_2019
 * Contributor : saurabh
 * creates new texture and adds into list(vector) of textures
 * current context should be active while calling these functions
 * use makeCurrent() to make context current
 * set a 8bit single color texture of size width x height
 */
void _Renderer::setupTexture(char* img,unsigned int width,unsigned int height,GLenum format,GLenum data_type,GLenum internal_format)
{
    textures.push_back(_Texture(img,width,height,format,data_type,internal_format));
    textures[textures.size()-1].load();
}
void _Renderer::setupTexture(QString texfile)
{
    QImage img = QImage(texfile);
    _Texture texture(img);
    textures.push_back(texture);
    textures[textures.size()-1].load(GL_RGBA,GL_UNSIGNED_BYTE);
    qDebug() << "setupTexture(QString texfile) on entity" << sceneEntity.getTag();
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
    qDebug() << "setTexture(char* texBitmap) on entity" << sceneEntity.getTag();
}
void _Renderer::setTexture(char* texBitmap,uint iwidth,uint iheight){
    if(!textures.empty())
        textures[0].setImage(texBitmap,iwidth,iheight);
}

bool _Renderer::isTexturePresent(){
    return !textures.empty();
    qDebug() << "setTexture(char* texBitmap,uint iwidth,uint iheight) on entity" << sceneEntity.getTag();
}

/*
 * Function: setTexture(QString pathtoTexture)
 * Created: 28_3_2019
 * Contributor : saurabh
 * updates the first texture image from a texfile
 * current context should be active while calling this function
 */
void _Renderer::setTexture(QString pathtoTexture)
{
    if(!textures.empty())
        textures[0].setImage(pathtoTexture);
    qDebug() << "setTexture(QString pathtoTexture) on entity" << sceneEntity.getTag();
}

/*
* Function: setModelMatrix(QVector3D position,float scale,QQuaternion rotation)
* Sets the values matrices for the model matrix
* works in implementing translation , rotation and scaling
* Used by: the _glWidget class initialiseGl() or paintGl().
* Created: 25_02_2019
*/
void _Renderer::setModelMatrix(glm::vec3 position,float scale,glm::vec3 rotation)
{
    modelMatrix = glm::mat4(1.0f);
    translationMatrix = glm::mat4(1.f);
    rotationMatrix = glm::mat4(1.f);
    scalingMatrix = glm::mat4(1.f);

    scalingMatrix = glm::scale(scalingMatrix, glm::vec3(scale, scale, scale));//scale equally on all sides
    glm::vec3 EulerAngles(rotation.x,rotation.y,rotation.z);
    glm::quat quat = glm::quat(EulerAngles);
    rotationMatrix = glm::mat4_cast(quat);
    translationMatrix = glm::translate(translationMatrix,position);

    modelMatrix = translationMatrix * rotationMatrix * scalingMatrix;
    qDebug() << "setModelMatrix() on entity" << sceneEntity.getTag();
    keepSceneEntityUpdated();
}
/*
* Function: setCamViewMatrix(QVector3D eyePos,QVector3D focalPoint,QVector3D upVector)
* sets the camera view for the scene through this matrix
* helps set the camera , eye positon , rotation, lookat.
* Used by: the _glWidget class initialiseGl() or paintGl().
* depending if the camra needs to update its position in  realtime.
* Created: 25_02_2019
*/
void _Renderer::setCamViewMatrix(QVector3D eyePos,glm::vec3 focalPoint,QVector3D upVector)
{
    viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::lookAt(
                glm::vec3(eyePos.x(), eyePos.y(), eyePos.z()),
                glm::vec3(focalPoint.x, focalPoint.y, focalPoint.z),
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
    projectionMatrix = glm::perspective(glm::radians(fov), float(aspect), zNear, zFar);
   //qDebug() << "setProjectionMatrix() on entity" << sceneEntity.getTag();
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
void _Renderer::setPosition(glm::vec3 pos)
{
    if(sceneEntity.getIsTransformationAllowed())
    {
        if(sceneEntity.getIsTransformationLocal())
        {
            modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix,pos);
        }
        else if(!sceneEntity.getIsTransformationLocal())
        {
            translationMatrix = glm::mat4(1.f);
            translationMatrix = glm::translate(translationMatrix,pos);
            modelMatrix = translationMatrix * rotationMatrix * scalingMatrix;
        }
        keepSceneEntityUpdated();
    }
}

void _Renderer::translate(glm::vec3 pos)
{
    if(sceneEntity.getIsTransformationAllowed())
    {
        //update the traformation matrix with the current values
        setPosition(sceneEntity.getPostion());
        if(sceneEntity.getIsTransformationLocal())
        {
            //        modelMatrix *= translationMatrix;
            modelMatrix = glm::translate(modelMatrix,pos);
        }
        else if(!sceneEntity.getIsTransformationLocal())
        {
            translationMatrix = glm::translate(translationMatrix,pos);
            modelMatrix = translationMatrix * rotationMatrix * scalingMatrix;
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
void _Renderer::setRotation(glm::vec3 rot)
{
    if(sceneEntity.getIsTransformationAllowed())
    {
        if(sceneEntity.getIsPivotSet() == false)
        {
            sceneEntity.setRotation(rot);
            if(sceneEntity.getIsTransformationLocal())
            {
                glm::vec3 eulerAngles(sceneEntity.getRotation());
                glm::quat quat = glm::quat(eulerAngles);
                modelMatrix *= glm::mat4_cast(quat);
            }
            else if(!sceneEntity.getIsTransformationLocal())
            {
                //rotationMatrix = glm::mat4x4(1.f);
                glm::vec3 eulerAngles(sceneEntity.getRotation());
                glm::quat quat = glm::quat(eulerAngles);
                rotationMatrix = glm::mat4_cast(quat);
                //rotate at center
                modelMatrix =  translationMatrix * rotationMatrix * scalingMatrix;
            }
        }
        else if(sceneEntity.getIsPivotSet() == true)
        {
            setRotationAroundPivot(rot, sceneEntity.getPivot());
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
void _Renderer::setRotationAroundPivot(glm::vec3 rot, glm::vec3 pivot)
{
    if(sceneEntity.getIsTransformationAllowed())
    {
        sceneEntity.setRotation(rot);
        if(sceneEntity.getIsTransformationLocal()){
            //still buggy
            setPosition(pivot);
            glm::vec3 EulerAngles(sceneEntity.getRotation());
            glm::quat quat = glm::quat(EulerAngles);
            modelMatrix *= glm::mat4_cast(quat);
        }
        if(!sceneEntity.getIsTransformationLocal()){
            //this works like an ofset pivot rather than rotae around a point
            //(Alterante implementation involves multiplying parent rotation matrix with childrens model matrix)#Not implemented
            pivotTmat = glm::mat4x4(1.0f);

            pivotTmat[3][0] = pivot.x;
            pivotTmat[3][1] = pivot.y;
            pivotTmat[3][2] = pivot.z;
            glm::vec3 EulerAngles(sceneEntity.getRotation());
            glm::quat quat = glm::quat(EulerAngles);
            rotationMatrix = glm::mat4_cast(quat);
            modelMatrix = translationMatrix * rotationMatrix * pivotTmat * scalingMatrix;
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
    if(sceneEntity.getIsTransformationAllowed())
    {
        sceneEntity.setScale(scale);//reimplemnt
        scalingMatrix = glm::mat4(1.f);
        //scale eqally on all axis(dont need respective sclaing)
        scalingMatrix = glm::scale(scalingMatrix, glm::vec3(sceneEntity.getScale(),
                                                            sceneEntity.getScale(),
                                                            sceneEntity.getScale()));
        modelMatrix = translationMatrix * rotationMatrix * scalingMatrix;
    }
    keepSceneEntityUpdated();
}
/*
 * Created:18_06_2019
*/
void _Renderer::lookAt(QVector3D ptl)
{
    //    glm::vec3 obPos = glm::vec3(sceneEntity.getPostion().x(),sceneEntity.getPostion().y(),sceneEntity.getPostion().z());
    //    glm::vec3 tarPo = glm::vec3(ptl.x(),ptl.y(),ptl.z());

    //    glm::vec3 delta = tarPo - obPos;//targetPosition-objectPosition
    //    glm::vec3 up;
    //    glm::vec3 dir(glm::normalize(delta));

    //    if(abs(dir.x) < 0.00001 && abs(dir.z) < 0.00001){
    //        if(dir.y > 0)
    //            up = glm::vec3(0.0, 0.0, -1.0); //if direction points in +y
    //        else
    //            up = glm::vec3(0.0, 0.0, 1.0); //if direction points in -y
    //    } else {
    //        up = glm::vec3(0.0, 1.0, 0.0); //y-axis is the general up
    //    }
    //    up = glm::normalize(up);
    //    glm::vec3 right = glm::normalize(glm::cross(up,dir));
    //    up = glm::normalize(glm::cross(dir, right));

    //    rotationMatrix *= glm::mat4(right.x, right.y, right.z, 0.0f,
    //                                up.x, up.y, up.z, 0.0f,
    //                                dir.x, dir.y, dir.z, 0.0f,
    //                                obPos.x, obPos.y, obPos.z, 1.0f);

    //    keepSceneEntityUpdated();
    //    _Tools::Debugmatrix4x4(this->sceneEntity.getModelMatrix());

    RotationBetweenVectors(glm::vec3(ptl.x(),ptl.y(),ptl.z()));
}

void _Renderer::RotationBetweenVectors(glm::vec3 dest){
    glm::vec3 start = glm::vec3(sceneEntity.getPostion());
    start = glm::normalize(start);//this object location
    dest = glm::normalize(dest);

    float cosTheta = glm::dot(start, dest);
    glm::vec3 rotationAxis;

    glm::quat axisangle;
    if (cosTheta < -1 + 0.001f){
        rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
        if (glm::length(rotationAxis) < 0.01 )
            rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), start);
        rotationAxis = glm::normalize(rotationAxis);
        axisangle = glm::angleAxis(180.0f, rotationAxis);
    }

    rotationAxis = glm::cross(start, dest);

    float s = sqrt((1+cosTheta)*2);
    float invs = 1 / s;

    modelMatrix *= glm::mat4_cast(glm::quat(
                                      s * 0.5f,
                                      rotationAxis.x * invs,
                                      rotationAxis.y * invs,
                                      rotationAxis.z * invs
                                      ));

    this->sceneEntity.setModelMatrix(modelMatrix);
    keepSceneEntityUpdated();
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
    sceneEntity = s;
    actualColor = sceneEntity.getColor();
    setShader(sceneEntity.getVertexShaderPath(), sceneEntity.getFragmentShaderPath());
    if(s.getTexturePath()!="")//set texture only if it is mentioned in the scenentity
            setupTexture(s.getTexturePath());
    //setModelDataInBuffers() happens for every object,and is sufficent for the usecases // !!!Comment for structural change!!!!
    //can be converted to using the same VAO for the same set of vertex+index data.
    //will need to move the whole model loading and id geenration to assetLoader class
    //and only pass the relavant ids to VAO at runtime to reduce ovehead.
    setModelDataInBuffers(sceneEntity.getModelInfo().getVertexArray(), sceneEntity.getModelInfo().getIndexArray());
    setModelMatrix(sceneEntity.getPostion(), sceneEntity.getScale(), sceneEntity.getRotation());
}
/*
 * Created: 1_03_2019
*/
void _Renderer::setSceneEntityInRenderer(_SceneEntity s){
    sceneEntity = s;
}
/*
 * Created: 2_03_2019
*/
void _Renderer::keepSceneEntityUpdated(){
    //Keeps a copy of the current matrix info in the respective sceneEntity
    sceneEntity.setTranslationMatrix(translationMatrix);
    sceneEntity.setRotationmatrix(rotationMatrix);
    sceneEntity.setScaleingMatrix(scalingMatrix);
    sceneEntity.setProjectionMatrix(projectionMatrix);
    sceneEntity.setViewMatrix(viewMatrix);
    sceneEntity.setModelMatrix(modelMatrix);

    //get the real position values from the modelMatrix
    glm::mat4x4 tmat4 = modelMatrix * glm::inverse(rotationMatrix) * glm::inverse(scalingMatrix);
    sceneEntity.setPosition(glm::vec3(tmat4[3][0],
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
    if(sceneEntity.getIsLineMode())
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    else if(sceneEntity.getIsLineNoCullMode())
        glPolygonMode(GL_FRONT,GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    if(sceneEntity.getIsActive())
    {
        //Using the shader program in the current context
        shdr->useShaderProgram();
        //Bind Textures
        for(uint t=0;t<textures.size();t++){
            textures[t].bind();
        }
        //Bind the Buffers data of the respective buffer object(only needed if mesh need chenging on runtime)
        if(sceneEntity.getIsMeshEditable()){
            glBindBuffer(GL_ARRAY_BUFFER,VBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);}
        //Bind the VAO of the respective buffer object (needs to be bound everytime)
        glBindVertexArray(VAO);
        //
        //Sets the values for the MVP matrix in the vertex shader
        glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(sceneEntity.getViewMatrix()));
        glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(sceneEntity.getProjectionMatrix()));
        //glUniformMatrix4fv(modelUnifrom, 1, GL_FALSE, glm::value_ptr(sceneEntity.getTranslationMatrix()*sceneEntity.getRotationmatrix()*pivotTmat *sceneEntity.getScaleingMatrix()));
        glUniformMatrix4fv(modelUnifrom,1,GL_FALSE,glm::value_ptr(sceneEntity.getModelMatrix()));
        float aspect = sceneEntity.getAspectImage();
        glUniformMatrix4fv(aspectUnifrom,1,GL_FALSE,&aspect);
        //
        setColors();//Setting the uniform for color
        //
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);//The Final draw call for each frame
        glBindVertexArray(0);//Clear the buffer
    }
}
/*
 *  Used in the Draw functon
 * Updates the color to the newly defined one;
 */
void _Renderer::setColors()
{
    glUniform4f(colorUniform,sceneEntity.getColor().x(),sceneEntity.getColor().y(),sceneEntity.getColor().z(),sceneEntity.getColor().w());
    //set sfidex color attributes for defined objects
    if(sceneEntity.getId() == 999 || sceneEntity.getId() == 991|| sceneEntity.getId() == 992 || sceneEntity.getId() == 993)//PointHelpers
    {
        QVector4D col = sceneEntity.getColor();
        col.setX(col.x() + abs(cos(timer.elapsed() * 0.002)));
        col.setY(col.y() + abs(cos(timer.elapsed() * 0.003)));
        col.setZ(col.z() + abs(cos(timer.elapsed() * 0.005)));
        glUniform4f(colorUniform, col.x(),col.y(), col.z(), col.w());
    }
    if(sceneEntity.getId() == 888)//pivot
    {
        QVector4D col = sceneEntity.getColor();
        col.setX(col.x() + abs(cos(timer.elapsed() * 0.04)));
        col.setY(col.y() + abs(cos(timer.elapsed() * 0.03)));
        col.setZ(col.z() + abs(cos(timer.elapsed() * 0.05)));
        glUniform4f(colorUniform, col.x(),col.y(), col.z(), col.w());
    }
    if( sceneEntity.getisHitByRay())
        sceneEntity.setColor(actualColor);
    else
        sceneEntity.setColor(actualColor * 0.8);
}
