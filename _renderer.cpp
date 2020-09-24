#include "_renderer.h"
#include <iostream>
#include <_tools.h>
#include <qquaternion.h>


/*
 * The Renderer class
 *
 * Author: Aditya
*/
/*
 * The "QOpenGLExtraFunctions(QOpenGLContext::currentContext())" is passed by parameter
*/
_Renderer::_Renderer() : QOpenGLExtraFunctions(QOpenGLContext::currentContext())
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0);//sets the bckground color of the openglContext.
    //
   // shdr = new _Shader();//initialising the _shader() class * object.
    setShader();//will run this shader by default.
    timer.start();
    //
    projectionMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    viewMatrix = glm::mat4(1.0f);
    pivotTmat = glm::mat4(1.0f);
    qDebug() << "render initialised ";
        sceneEntity = new _SceneEntity();
}
/*
 *Distructor: _Renderer Class
*/
_Renderer::~_Renderer()
{
    delete sceneEntity;
    for(auto sr : shaderVec){delete  sr;}
    shaderVec.clear();
}
/*
*
* returns the current scene entity object.
*/
_SceneEntity *_Renderer::getSceneEntity() const
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
* Sets the sceen entity object locally and sets the
* shader ,Model data , projection matrix, texture,
* and initialises the modelMatrix.
*/
void _Renderer::initSceneEntityInRenderer(_SceneEntity* s)
{
    sceneEntity = s;
    actualColor = sceneEntity->getColor();
    setShader(sceneEntity->getVertexShaderPath(), sceneEntity->getFragmentShaderPath());//1
    if(sceneEntity->getIsShadowCaster()){setShader(":/shaders/shadowDepthMapV.glsl",":/shaders/shadowDepthMapF.glsl");}//2
    //
    if(sceneEntity->getMaterial().getDiffuseTexture().size() != 0){setupTexture(sceneEntity->getMaterial().getDiffuseTexture(),_Texture::Type::Diffuse);}
    if(sceneEntity->getMaterial().getSpecualrTexture().size() != 0){setupTexture(sceneEntity->getMaterial().getSpecualrTexture(),_Texture::Type::Specular);}
    //
    setModelMatrix(sceneEntity->getPostion(), sceneEntity->getScale(), sceneEntity->getRotation());

    if(sceneEntity->getModelInfo().getVertexArray().size() > 1){setModelDataInBuffers(sceneEntity->getModelInfo().getVertexArray(), sceneEntity->getModelInfo().getIndexArray());}
    else{setModelDataInBuffers(sceneEntity->getModelInfo().getVertexInfoArray(), sceneEntity->getModelInfo().getIndexArray());}
    qDebug() << "setModelDataInBuffers() for entity" << sceneEntity->getTag().c_str();
}
/*
 sets a copy of sceneEntity in the renderer
*/
void _Renderer::setSceneEntityInRenderer(_SceneEntity* s){
    sceneEntity = s;
}
/*
 *
*/
void _Renderer::keepSceneEntityUpdated(){
    //Keeps a copy of the current matrix info in the respective sceneEntity
    sceneEntity->setTranslationMatrix(translationMatrix);
    sceneEntity->setRotationmatrix(rotationMatrix);
    sceneEntity->setScaleingMatrix(scalingMatrix);
    sceneEntity->setProjectionMatrix(projectionMatrix);
    sceneEntity->setViewMatrix(viewMatrix);
    sceneEntity->setModelMatrix(modelMatrix);

    //get the real position values from the modelMatrix
    glm::mat4x4 tmat4 = modelMatrix * glm::inverse(rotationMatrix) * glm::inverse(scalingMatrix);
    sceneEntity->setPosition(glm::vec3(tmat4[3][0],
            tmat4[3][1],
            tmat4[3][2]));
    //    qDebug()<< tmat4[3][0] <<tmat4[3][1] << tmat4[3][2];
}

/*
 * Sets a dafault hard-fed shader
 * on the render object
 * Is being used by the _glWidget class
 * Create:11_02_2019
*/
void _Renderer::setShader()
{
    shdr = new _Shader();
    shdr->attachShaders(":/shaders/dmvshader.glsl", ":/shaders/dmfshader.glsl");
    shaderVec.push_back(shdr);

    qDebug() << "default Shader attached for entity" << sceneEntity->getTag().c_str();
}
/*
 * Takes the path to the relative qrc aided directory
 * to set shader paths externaly on the render object.
 * Is being used by the _glWidget class
 * Create:11_02_2019
*/
void _Renderer::setShader(QString vSh, QString fSh)
{
    shdr = new _Shader();
    shdr->attachShaders(vSh,fSh);
    shaderVec.push_back(shdr);
    qDebug() << "setShader(QString"<<vSh<<", QString"<<fSh<<")" << sceneEntity->getTag().c_str();
}
/*
 * set Vertex and Index data into
 * the GPU buffers to use for the current model.
 * May have extended implementation for inclusion of UV for texture and Normals for
 * lighting.
 * Used by: the _glWidget class initializeGL().
*/
void _Renderer::setModelDataInBuffers(std::vector<float> vertexArray, std::vector<uint> indexArray)//Pass normals
{
    // Copy the vertex and index data locally for use in the current drawcall.
    indices = indexArray;
    // Initialization code (done once (unless your object frequently changes))
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    qDebug() << "VBO-" << VBO << "VAO-" << VAO << "EBO-" << EBO;
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, vertexArray.size() * sizeof(float), &vertexArray[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), &indices[0], GL_STATIC_DRAW);

//  glEnableVertexAttribArray(0);
//  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

//  position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    // normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
}

void _Renderer::setModelDataInBuffers(std::vector<VertexInfo> vertexInfoArray, std::vector<uint> indexArray)
{
      indices = indexArray;

       glGenVertexArrays(1, &VAO);
       glGenBuffers(1, &VBO);
       glGenBuffers(1, &EBO);

       glBindVertexArray(VAO);

       glBindBuffer(GL_ARRAY_BUFFER, VBO);
       glBufferData(GL_ARRAY_BUFFER, vertexInfoArray.size() * sizeof(VertexInfo),&vertexInfoArray[0], GL_STATIC_DRAW);

       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
       glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArray.size() * sizeof(uint),&indexArray[0], GL_STATIC_DRAW);

       // vertex positions
       glEnableVertexAttribArray(0);
       glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (void*)0);
       // vertex normals
       glEnableVertexAttribArray(1);
       glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (void*)offsetof(VertexInfo, Normal));
       // vertex texture coords
       glEnableVertexAttribArray(2);
       glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (void*)offsetof(VertexInfo, TexCoords));

       glBindVertexArray(0);
}
/*
 * creates new texture and adds into list(vector) of textures
 * set a default 8bit single color texture of size 1360 x 1024
 */
void _Renderer::setupTexture()
{
    char* img = new char[1360*1024];
    _Texture texture(img,1360,1024);
    texture.load(GL_RED,GL_UNSIGNED_BYTE);
    textures.push_back(texture);
    qDebug() << "setupTexture() on entity" << sceneEntity->getTag().c_str();
}
//Bind the textre set in the Matirial obj of sceneEntity->
void _Renderer::setupTexture(QString texfile,_Texture::Type t)
{
    QImage img = QImage(texfile).convertToFormat(QImage::Format_RGBA8888);
    _Texture texture;
    texture.load(img,t,GL_RGBA,GL_UNSIGNED_BYTE);//Load the tex , genrate a ID for that texture

    for(uint s = 0 ; s < shaderVec.size(); s++){
        shaderVec[s]->useShaderProgram();//need to do this before setting the uniforms Always to reduce ambiguity
        //Sets the Texture slot id here , is invoked by glActiveTexture(GL_TEXTUREn) where n is the value assigned
        //make sure this matches in the Texture objects in the texture array.
        glUniform1i(shaderVec[s]->getUniformLocation("diffuseTex"), 1);
        glUniform1i(shaderVec[s]->getUniformLocation("specularTex"), 2);
        glUniform1i(shaderVec[s]->getUniformLocation("bumpTex"), 3);
    }

    textures.push_back(texture);
}
/*
 * Contributor : saurabh
 * updates the first texture image from char pointer array
 * resolution of previous image is used
 * current context should be active while calling this function
 */
void _Renderer::setTexture(char* texBitmap)
{
    if(!textures.empty())
        textures[0].setImage(texBitmap);
    qDebug() << "setTexture(char* texBitmap) on entity" << sceneEntity->getTag().c_str();
}
void _Renderer::setTexture(char* texBitmap,uint iwidth,uint iheight)
{
    if(!textures.empty())
        textures[0].setImage(texBitmap,iwidth,iheight);
    qDebug() << "setTexture(char* texBitmap,uint iwidth,uint iheight) on entity" << sceneEntity->getTag().c_str();
}
void _Renderer::setTexture(QString pathtoTexture)
{
    if(!textures.empty())
        textures[0].setImage(pathtoTexture);
    qDebug() << "setTexture(QString pathtoTexture) on entity" << sceneEntity->getTag().c_str();
}

/*
* Sets the values matrices for the model matrix
* works in implementing translation , rotation and scaling
* Used by: the _glWidget class initialiseGl() or paintGl().
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
    qDebug() << "setModelMatrix() on entity" << sceneEntity->getTag().c_str();
    keepSceneEntityUpdated();
}
/*
* sets the camera view for the scene through this matrix
* helps set the camera , eye positon , rotation, lookat.
* Used by: the _glWidget class initialiseGl() or paintGl().
* depending if the camra needs to update its position in  realtime.
*/
void _Renderer::setCamViewMatrix(glm::vec3 eyePos,glm::vec3 focalPoint,glm::vec3 upVector)
{
    camposForLight = glm::vec3(eyePos.x, eyePos.y, eyePos.z);//temp
    this->focalPoint = focalPoint;
    viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::lookAt(
                        glm::vec3(eyePos.x, eyePos.y, eyePos.z),
                        glm::vec3(focalPoint.x, focalPoint.y, focalPoint.z),
                        glm::vec3(upVector.x, upVector.y, upVector.z));
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
* takes thew width and height of the window and sets the relative
* field of view and the aspect ration bindings. will update itself each time the
* window is resized.and needs to be called in the resizeGl function.
* Used by: the _glWidget class resizeGL().
*/
void _Renderer::setProjectionMatrix(int resW, int resH, float fov, float zNear, float zFar){
    // Calculate aspect ratio
    float aspect = float(resW) / float(resH ? resH : 1);
    projectionMatrix = glm::perspective(glm::radians(fov), float(aspect), zNear, zFar);
    //qDebug() << "setProjectionMatrix() on entity" << sceneEntity->getTag().c_str();
}
void _Renderer::setOrthoProjectionMatrix(float left, float right, float bottom, float top, float zNear, float zFar){
//    float near_plane = 1.0f, far_plane = 7.5f;
//     orthoProjMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
     orthoProjMatrix = glm::ortho(left,right,bottom,top, zNear, zFar);
     //sceneEntity->setOrthoProjMatrix(orthoProjMatrix);//keep local  updated
}
void _Renderer::setProjectionMatrix(int type, glm::mat4x4 m)
{
    projectionMatrix = m;
}
/*
 ▄▄▄▄▄▄▄▄   ▄▄▄·  ▐ ▄ .▄▄ · ·▄▄▄      ▄▄▄  • ▌ ▄ ·.
 •██  ▀▄ █·▐█ ▀█ •█▌▐█▐█ ▀. ▐▄▄·▪     ▀▄ █··██ ▐███▪
  ▐█.▪▐▀▀▄ ▄█▀▀█ ▐█▐▐▌▄▀▀▀█▄██▪  ▄█▀▄ ▐▀▀▄ ▐█ ▌▐▌▐█·
  ▐█▌·▐█•█▌▐█ ▪▐▌██▐█▌▐█▄▪▐███▌.▐█▌.▐▌▐█•█▌██ ██▌▐█▌
  ▀▀▀ .▀  ▀ ▀  ▀ ▀▀ █▪ ▀▀▀▀ ▀▀▀  ▀█▄▀▪.▀  ▀▀▀  █▪▀▀▀
*/
/*
 * updates the specific trasformations that affect the model matrix
 * of the matrices of the individual object.In this case the positions
 * Used by: _render class in draw()
*/
void _Renderer::setPosition(glm::vec3 pos)
{
    if(sceneEntity->getIsTransformationAllowed())
    {
        if(sceneEntity->getIsTransformationLocal())
        {
            modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix,pos);
        }
        else if(!sceneEntity->getIsTransformationLocal())
        {
            translationMatrix = glm::mat4(1.f);
            translationMatrix = glm::translate(translationMatrix,pos);
            modelMatrix = translationMatrix * rotationMatrix * scalingMatrix;
        }
        keepSceneEntityUpdated();
    }
}
/*
 *
*/
void _Renderer::translate(glm::vec3 pos)
{
    if(sceneEntity->getIsTransformationAllowed())
    {
        //update the traformation matrix with the current values
        setPosition(sceneEntity->getPostion());
        if(sceneEntity->getIsTransformationLocal())
        {
            //        modelMatrix *= translationMatrix;
            modelMatrix = glm::translate(modelMatrix,pos);
        }
        else if(!sceneEntity->getIsTransformationLocal())
        {
            translationMatrix = glm::translate(translationMatrix,pos);
            modelMatrix = translationMatrix * rotationMatrix * scalingMatrix;
        }
        keepSceneEntityUpdated();
    }
}
/*
 * updates the specific trasformations that affect the model matrix
 * of the matrices of the individual object.In this case the rotation
 * Used by: _glwidgetClass on mousemovement

*/
void _Renderer::setRotation(glm::vec3 rot)
{
    if(sceneEntity->getIsTransformationAllowed())
    {
        if(sceneEntity->getIsPivotSet() == false)
        {
            sceneEntity->setRotation(rot);
            if(sceneEntity->getIsTransformationLocal())
            {
                glm::vec3 eulerAngles(sceneEntity->getRotation());
                glm::quat quat = glm::quat(eulerAngles);
                modelMatrix *= glm::mat4_cast(quat);
            }
            else if(!sceneEntity->getIsTransformationLocal())
            {
                //rotationMatrix = glm::mat4x4(1.f);
                glm::vec3 eulerAngles(sceneEntity->getRotation());
                glm::quat quat = glm::quat(eulerAngles);
                rotationMatrix = glm::mat4_cast(quat);
                //rotate at center
                modelMatrix =  translationMatrix * rotationMatrix * scalingMatrix;
            }
        }
        else if(sceneEntity->getIsPivotSet() == true)
        {
            setRotationAroundPivot(rot, sceneEntity->getPivot());
        }
        keepSceneEntityUpdated();
    }
}
/*
 * sets the rotation to be around an defined point
 * Used by: _glwidgetClass on Mousemovement
*/
void _Renderer::setRotationAroundPivot(glm::vec3 rot, glm::vec3 pivot)
{
    if(sceneEntity->getIsTransformationAllowed())
    {
        sceneEntity->setRotation(rot);
        if(sceneEntity->getIsTransformationLocal()){
            //still buggy
            setPosition(pivot);
            glm::vec3 EulerAngles(sceneEntity->getRotation());
            glm::quat quat = glm::quat(EulerAngles);
            modelMatrix *= glm::mat4_cast(quat);
        }
        if(!sceneEntity->getIsTransformationLocal()){
            //this works like an ofset pivot rather than rotae around a point
            //(Alterante implementation involves multiplying parent rotation matrix with childrens model matrix)#Not implemented
            pivotTmat = glm::mat4x4(1.0f);

            pivotTmat[3][0] = pivot.x;
            pivotTmat[3][1] = pivot.y;
            pivotTmat[3][2] = pivot.z;
            glm::vec3 EulerAngles(sceneEntity->getRotation());
            glm::quat quat = glm::quat(EulerAngles);
            rotationMatrix = glm::mat4_cast(quat);
            modelMatrix = translationMatrix * rotationMatrix * pivotTmat * scalingMatrix;
        }
        keepSceneEntityUpdated();
    }
}
/*
 * updates the specific trasformations that affect the model matrix
 * of the matrices of the individual object.In this case the scale
 * Used by: _render class in draw()
*/
void _Renderer::setscale(float scale)
{
    if(sceneEntity->getIsTransformationAllowed())
    {
        sceneEntity->setScale(scale);//reimplemnt
        scalingMatrix = glm::mat4(1.f);
        //scale eqally on all axis(dont need respective sclaing)
        scalingMatrix = glm::scale(scalingMatrix, glm::vec3(sceneEntity->getScale(),
                                                            sceneEntity->getScale(),
                                                            sceneEntity->getScale()));
        modelMatrix = translationMatrix * rotationMatrix * scalingMatrix;
    }
    keepSceneEntityUpdated();
}
/*
*/
void _Renderer::lookAt(QVector3D ptl) //Not Implemented properly yet needs to be fixed after Mesh on Mesh Collision
{
    //    glm::vec3 obPos = glm::vec3(sceneEntity->getPostion().x(),sceneEntity->getPostion().y(),sceneEntity->getPostion().z());
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
    //    _Tools::Debugmatrix4x4(this->sceneEntity->getModelMatrix());

    RotationBetweenVectors(glm::vec3(ptl.x(),ptl.y(),ptl.z()));
}

void _Renderer::RotationBetweenVectors(glm::vec3 dest)
{
    glm::vec3 start = glm::vec3(sceneEntity->getPostion());
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
    this->sceneEntity->setModelMatrix(modelMatrix);
    keepSceneEntityUpdated();
}
//updates the local material info
void _Renderer::updateMaterial(_Material m)
{
    sceneEntity->setMaterial(m);
}

/*
  ·▄▄▄▄  ▄▄▄   ▄▄▄· ▄▄▌ ▐ ▄▌
  ██▪ ██ ▀▄ █·▐█ ▀█ ██· █▌▐█
  ▐█· ▐█▌▐▀▀▄ ▄█▀▀█ ██▪▐█▐▐▌
  ██. ██ ▐█•█▌▐█ ▪▐▌▐█▌██▐█▌
  ▀▀▀▀▀• .▀  ▀ ▀  ▀  ▀▀▀▀ ▀▪
*/
/*
 * This is your proprietory draw function
 * Draws frames on a avg of 60frames per second(is subjective and changes with hardware)
 * Used by: the _glWidget class paintGl().
*/
void _Renderer::_Renderer::draw(uint shaderSelector)
{
    setGLEnablements();//function sets openGL Rasterisation modifiers
    shaderSelector > shaderVec.size() ? shaderSelector = 0 : shaderSelector;
    ssl = shaderSelector;
    if(sceneEntity->getIsActive())
    {
        //Using the shader program in the current context
        shaderVec[ssl]->useShaderProgram();
        //Bind Textures
        for(uint t=0;t<textures.size();t++){
            textures[t].bind(t+1);//starts with 1 , as the 0th is assigned to the FBO tex
        }
        //Bind the Buffers data of the respective buffer object(only needed if mesh need chenging on runtime)
        if(sceneEntity->getIsMeshEditable())
        {
            glBindBuffer(GL_ARRAY_BUFFER,VBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
        }
        //Bind the VAO of the respective buffer object (needs to be bound everytime)
        glBindVertexArray(VAO);
        //
        //Sets the values for the MVP matrix in the vertex shader
        glUniformMatrix4fv(shaderVec[ssl]->getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(sceneEntity->getViewMatrix()));
        glUniformMatrix4fv(shaderVec[ssl]->getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(sceneEntity->getProjectionMatrix()));
        glUniformMatrix4fv(shaderVec[ssl]->getUniformLocation("orthoProjection"), 1, GL_FALSE, glm::value_ptr(orthoProjMatrix));//for shadow calcs
        glUniformMatrix4fv(shaderVec[ssl]->getUniformLocation("model"),1,GL_FALSE,glm::value_ptr(sceneEntity->getModelMatrix()));
        //glUniformMatrix4fv(shdr->getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(sceneEntity->getTranslationMatrix()*sceneEntity->getRotationmatrix()*pivotTmat *sceneEntity->getScaleingMatrix()));
        //
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);//The Final draw call for each frame
        //
        glBindVertexArray(0);//Clear the buffer
        for(uint t=0;t<textures.size();t++){textures[t].unbind();}
        //
        updateColorUniforms();//Setting the uniform for color if shader allows
    }
}
/*
 *  Used in the Draw functon
 * Updates the color and relative uniforms
 */
void _Renderer::updateColorUniforms()
{
    glUniform4f(shaderVec[ssl]->getUniformLocation("aColor"),sceneEntity->getColor().x(),sceneEntity->getColor().y(),sceneEntity->getColor().z(),sceneEntity->getColor().w());
    //set sfidex color attributes for defined objects
    if(sceneEntity->getId() == 999 || sceneEntity->getId() == 991|| sceneEntity->getId() == 992 || sceneEntity->getId() == 993)//PointHelpers
    {
        QVector4D col = sceneEntity->getColor();
        col.setX(col.x() + abs(cos(timer.elapsed() * 0.002)));
        col.setY(col.y() + abs(cos(timer.elapsed() * 0.003)));
        col.setZ(col.z() + abs(cos(timer.elapsed() * 0.005)));
        glUniform4f(shaderVec[ssl]->getUniformLocation("aColor"), col.x(),col.y(), col.z(), col.w());
    }
    if(sceneEntity->getId() == 888)//pivot
    {
        QVector4D col = sceneEntity->getColor();
        col.setX(col.x() + abs(cos(timer.elapsed() * 0.04)));
        col.setY(col.y() + abs(cos(timer.elapsed() * 0.03)));
        col.setZ(col.z() + abs(cos(timer.elapsed() * 0.05)));
        glUniform4f(shaderVec[ssl]->getUniformLocation("aColor"), col.x(),col.y(), col.z(), col.w());
    }

    sceneEntity->getisHitByRay() ? sceneEntity->setColor(actualColor * 2.0) : sceneEntity->setColor(actualColor * 1.0);

//    if(sceneEntity->getIsHitByTri()){
//        QVector4D qc = actualColor;
//         qc.setX(actualColor.x() + 1.0f);
//        sceneEntity->setColor(qc);
//    }
//    else
//        sceneEntity->setColor(actualColor);
}
/*
 *  Used in the Draw functon
 * Updates the light uniforms on the model
 * is called in the Scene class in the draw function();
 */

void _Renderer::updateLightUniforms(std::vector<I_Light*> il)
{
    glUniform1f(shaderVec[ssl]->getUniformLocation("time"),qtimer.elapsed());
    uint piter = 0;
    for(uint li = 0 ; li < il.size(); li++)
    {
        if(il[li]->getLightType() == "DirLight")
        {
            //positions
            glUniform3f(shaderVec[ssl]->getUniformLocation("viewPos"),camposForLight.x,camposForLight.y,camposForLight.z);
            glUniform3f(shaderVec[ssl]->getUniformLocation("light.position"),il[li]->getLightParams()[0].x,il[li]->getLightParams()[0].y,il[li]->getLightParams()[0].z);
            // Dir properties
            glUniform3f(shaderVec[ssl]->getUniformLocation("light.ambient"), il[li]->getLightParams()[2].x,il[li]->getLightParams()[2].y,il[li]->getLightParams()[2].z); // note that all light colors are set at full intensity
            glUniform3f(shaderVec[ssl]->getUniformLocation("light.diffuse"), il[li]->getLightParams()[1].x,il[li]->getLightParams()[1].y,il[li]->getLightParams()[1].z);
            glUniform3f(shaderVec[ssl]->getUniformLocation("light.specular"), il[li]->getLightParams()[3].x,il[li]->getLightParams()[3].y,il[li]->getLightParams()[3].z);

        }
        if(il[li]->getLightType() == "PointLight")
        {
            glUniform3f(shdr->getUniformLocation("viewPos"),camposForLight.x,camposForLight.y,camposForLight.z);
            //PointL
                f  = "pointLights[";
                u  = std::to_string(piter);
                e1 = "].position";
                e2 = "].ambient";
                e3 = "].diffuse";
                e4 = "].specular";
                e5 = "].constant";
                e6 = "].linear";
                e7 = "].quadratic";

                glUniform3f(shaderVec[ssl]->getUniformLocation((f+u+e1).c_str()), il[li]->getLightParams()[0].x,il[li]->getLightParams()[0].y,il[li]->getLightParams()[0].z);
                glUniform3f(shaderVec[ssl]->getUniformLocation((f+u+e2).c_str()), il[li]->getLightParams()[2].x,il[li]->getLightParams()[2].y,il[li]->getLightParams()[2].z); // note that all light colors are set at full intensity
                glUniform3f(shaderVec[ssl]->getUniformLocation((f+u+e3).c_str()), il[li]->getLightParams()[1].x,il[li]->getLightParams()[1].y,il[li]->getLightParams()[1].z);
                glUniform3f(shaderVec[ssl]->getUniformLocation((f+u+e4).c_str()), il[li]->getLightParams()[3].x,il[li]->getLightParams()[3].y,il[li]->getLightParams()[3].z);
                glUniform1f(shaderVec[ssl]->getUniformLocation((f+u+e5).c_str()), il[li]->getLightParams()[4].x); // note that all light colors are set at full intensity
                glUniform1f(shaderVec[ssl]->getUniformLocation((f+u+e6).c_str()), il[li]->getLightParams()[5].x);
                glUniform1f(shaderVec[ssl]->getUniformLocation((f+u+e7).c_str()), il[li]->getLightParams()[6].x);

                piter++;
                piter > 2 ? piter = 0: piter;//3 is max no of point light should also reflect in the shader
        }
        if(il[li]->getLightType() == "SpotLight")
        {
//            //positions
//            glUniform3f(shdr->getUniformLocation("viewPos"),camposForLight.x,camposForLight.y,camposForLight.z);
//            glUniform3f(shdr->getUniformLocation("light.position"),l.getPosition().x,l.getPosition().y,l.getPosition().z);
//            //SpotL-
//            glm::vec3 dir = glm::normalize(focalPoint - camposForLight);
//            glUniform3f(shdr->getUniformLocation("light.direction"),dir.x,dir.y,dir.z); // note that all light colors are set at full intensity
//            glUniform1f(shdr->getUniformLocation("light.cutOff"),glm::cos(glm::radians(12.5f)));
//            glUniform1f(shdr->getUniformLocation("light.outerCutOff"),glm::cos(glm::radians(17.5f)));
        }
    }

    // material properties
    glUniform3f(shaderVec[ssl]->getUniformLocation("material.ambient"), sceneEntity->getMaterial().getAmbient().x,sceneEntity->getMaterial().getAmbient().y,sceneEntity->getMaterial().getAmbient().z);
    glUniform3f(shaderVec[ssl]->getUniformLocation("material.diffuse"), sceneEntity->getMaterial().getDiffuse().x,sceneEntity->getMaterial().getDiffuse().y,sceneEntity->getMaterial().getDiffuse().z);
    glUniform3f(shaderVec[ssl]->getUniformLocation("material.specular"), sceneEntity->getMaterial().getSpecular().x,sceneEntity->getMaterial().getSpecular().y,sceneEntity->getMaterial().getSpecular().z);
    glUniform1f(shaderVec[ssl]->getUniformLocation("material.shininess"), sceneEntity->getMaterial().getShine());
}


//Sets the rastersation enablements for Opengl
//via implecit extension invocations.
//is setting enablements explicitly for each object in its own render callback
//but can be utilised in the scene class to render entire sets of objs with the same settings
void _Renderer::setGLEnablements()
{
//-----------------------------Default Overide------------------------------------------------------

    glEnable(GL_BLEND);//for transparency in alpha values
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//define how the blending needs to be applied
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); // Accept fragment if it closer to the camera than the former one
    // Cull triangles which normal is not towards the camera
//    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);//culls the backface saving some raster ops

    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

//    //The poligon winding order in which it is rendered,
//    //glFrontFace(GL_CW);//clockwise
    glFrontFace(GL_CCW);//counterClokwise, default, even if not explicitly stated
    //!!**NOTE**!!
//    //Opengl uses right handed cordinates meaning X+ = right , y up and -z towards you.
//    //if switching it to left handed(in the modelview matrix) dont forget to change the winding order

//-----------------------------------------------------------------------------------------------------
    //The obove is needed as default as the settings  might be enabled/disabled in other instances or
    //entites and causes fast switchign between raster states which causes artifacts in the final render.
//-------------------------------Controlled Overide----------------------------------------------------

    _SceneEntity::GlEnablements g = sceneEntity->getGLModes();
    _SceneEntity::GlEnablements::frameBufferModes frameBufferMode;
    _SceneEntity::GlEnablements::cullModes cullMode;
    _SceneEntity::GlEnablements::fillModes fillMode;

    frameBufferMode = g.frameBufferMode;
    switch (frameBufferMode){
    case  _SceneEntity::GlEnablements::ColorOnly:
//        glDisable(GL_BLEND);
//        glDisable(GL_DEPTH_TEST);
                break;
    case _SceneEntity::GlEnablements::Blend:
        glEnable(GL_BLEND);//for transparency in alpha values
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//define how the blending needs to be applied
                break;
    case _SceneEntity::GlEnablements::Depth:
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);//Accept fragment if it closer to the camera than the former one
        glDepthMask(GL_TRUE);//Enable writing to Dbuffer.
                break;
    case _SceneEntity::GlEnablements::DepthAlwaysPass:
          glEnable(GL_DEPTH_TEST);
          glDepthFunc(GL_ALWAYS);
          glDepthMask(GL_FALSE);//Disable writingas glDepthFunc(GL_ALWAYS) as test needs to pass always.
                break;
    case _SceneEntity::GlEnablements::Stencil:
        //Pending imp
                break;
    case _SceneEntity::GlEnablements::BlendAndDepth:
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH);
        glDepthFunc(GL_LESS);
                break;
    case _SceneEntity::GlEnablements::BlendAndDepthAndStencil:
                break;
    }
    cullMode = g.cullMode;
    switch (cullMode) {
    case _SceneEntity::GlEnablements::BackFace:
        glEnable(GL_CULL_FACE);// Cull triangles which normal is not towards the camera
        glCullFace(GL_BACK);//culls the backfaces saving some raster ops
                break;
    case  _SceneEntity::GlEnablements::FrontFace:
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);//culls the front faces
                break;
    case  _SceneEntity::GlEnablements::FrontAndBack:
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT_AND_BACK);//culls the entire object
                break;
    }
    fillMode = g.fillMode;
    switch(fillMode){
    case _SceneEntity::GlEnablements::FrontAndBackLine:
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        break;
    case _SceneEntity::GlEnablements::FrontAndBackFill:
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        break;
    }
}

void _Renderer::setShderSelector(uint ssl)
{
    this->ssl = ssl;
}
