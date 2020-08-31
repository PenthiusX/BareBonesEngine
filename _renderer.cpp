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
*
* returns the current scene entity object.
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
* Sets the sceen entity object locally and sets the
* shader ,Model data , projection matrix, texture,
* and initialises the modelMatrix.
*/
void _Renderer::initSceneEntityInRenderer(_SceneEntity s)
{
    sceneEntity = s;
    actualColor = sceneEntity.getColor();
    setShader(sceneEntity.getVertexShaderPath(), sceneEntity.getFragmentShaderPath());
    //
    if(sceneEntity.getMaterial().getDiffuseTexture().size() != 0){setupTexture(sceneEntity.getMaterial().getDiffuseTexture(),_Texture::Type::Diffuse);}
    if(sceneEntity.getMaterial().getSpecualrTexture().size() != 0){setupTexture(sceneEntity.getMaterial().getSpecualrTexture(),_Texture::Type::Specular);}
    //
    setModelMatrix(sceneEntity.getPostion(), sceneEntity.getScale(), sceneEntity.getRotation());

    if(sceneEntity.getModelInfo().getVertexArray().size() > 1){setModelDataInBuffers(sceneEntity.getModelInfo().getVertexArray(), sceneEntity.getModelInfo().getIndexArray());}
    else{setModelDataInBuffers(sceneEntity.getModelInfo().getVertexInfoArray(), sceneEntity.getModelInfo().getIndexArray());}
    setuniformLocations();//sets all uniform locations to respective variables.
    qDebug() << "setModelDataInBuffers() for entity" << sceneEntity.getTag();
}
/*
 sets a copy of sceneEntity in the renderer
*/
void _Renderer::setSceneEntityInRenderer(_SceneEntity s){
    sceneEntity = s;
}
/*
 *
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
 * sets the unform location uints into there respectively
 * named variables. These variables are used based on definition in shader.
*/
void _Renderer::setuniformLocations()
{
    qInfo() << "---------------UNIFORM INFO------------------------";
    qDebug() <<"Tag ->"<<sceneEntity.getTag();
    colorUniform = shdr->getUniformLocation("aColor");
    qDebug() << "colorUniform ->" << colorUniform;
    modelUnifrom = shdr->getUniformLocation("model");
    qDebug() << "modelUnifrom ->" << modelUnifrom;
    viewUniform  = shdr->getUniformLocation("view");
    qDebug() << "viewUniform ->" << viewUniform;
    projectionUniform = shdr->getUniformLocation("projection");
    qDebug() << "projectionUniform ->" << projectionUniform;
    mousePosUniform = shdr->getUniformLocation("iMouse");
    qDebug() << "mousePosUniform ->" << mousePosUniform;
    qDebug() <<"---------------------------------------------------";
    qDebug() <<"---------------------------------------------------";
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
    qDebug() << "setupTexture() on entity" << sceneEntity.getTag();
}

void _Renderer::setupTexture(QString texfile,_Texture::Type t)
{
    QImage img = QImage(texfile).convertToFormat(QImage::Format_RGBA8888);
    _Texture texture;
    texture.load(img,t,this->shdr,GL_RGBA,GL_UNSIGNED_BYTE);
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
    qDebug() << "setTexture(char* texBitmap) on entity" << sceneEntity.getTag();
}
void _Renderer::setTexture(char* texBitmap,uint iwidth,uint iheight)
{
    if(!textures.empty())
        textures[0].setImage(texBitmap,iwidth,iheight);
    qDebug() << "setTexture(char* texBitmap,uint iwidth,uint iheight) on entity" << sceneEntity.getTag();
}
void _Renderer::setTexture(QString pathtoTexture)
{
    if(!textures.empty())
        textures[0].setImage(pathtoTexture);
    qDebug() << "setTexture(QString pathtoTexture) on entity" << sceneEntity.getTag();
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
    qDebug() << "setModelMatrix() on entity" << sceneEntity.getTag();
    keepSceneEntityUpdated();
}
/*
* sets the camera view for the scene through this matrix
* helps set the camera , eye positon , rotation, lookat.
* Used by: the _glWidget class initialiseGl() or paintGl().
* depending if the camra needs to update its position in  realtime.
*/
void _Renderer::setCamViewMatrix(QVector3D eyePos,glm::vec3 focalPoint,QVector3D upVector)
{
    camposForLight = glm::vec3(eyePos.x(), eyePos.y(), eyePos.z());//temp
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
* takes thew width and height of the window and sets the relative
* field of view and the aspect ration bindings. will update itself each time the
* window is resized.and needs to be called in the resizeGl function.
* Used by: the _glWidget class resizeGL().
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
 * updates the specific trasformations that affect the model matrix
 * of the matrices of the individual object.In this case the positions
 * Used by: _render class in draw()
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
/*
 *
*/
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
 * updates the specific trasformations that affect the model matrix
 * of the matrices of the individual object.In this case the rotation
 * Used by: _glwidgetClass on mousemovement

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
 * sets the rotation to be around an defined point
 * Used by: _glwidgetClass on Mousemovement
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
 * updates the specific trasformations that affect the model matrix
 * of the matrices of the individual object.In this case the scale
 * Used by: _render class in draw()
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
*/
void _Renderer::lookAt(QVector3D ptl) //Not Implemented properly yet needs to be fixed after Mesh on Mesh Collision
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

void _Renderer::RotationBetweenVectors(glm::vec3 dest)
{
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
  ·▄▄▄▄  ▄▄▄   ▄▄▄· ▄▄▌ ▐ ▄▌
  ██▪ ██ ▀▄ █·▐█ ▀█ ██· █▌▐█
  ▐█· ▐█▌▐▀▀▄ ▄█▀▀█ ██▪▐█▐▐▌
  ██. ██ ▐█•█▌▐█ ▪▐▌▐█▌██▐█▌
  ▀▀▀▀▀• .▀  ▀ ▀  ▀  ▀▀▀▀ ▀▪
*/

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
//    glCullFace(GL_BACK);//culls the backface saving some raster ops

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

    _SceneEntity::GlEnablements g = sceneEntity.getGLModes();
    _SceneEntity::GlEnablements::frameBufferModes frameBufferMode;
    _SceneEntity::GlEnablements::cullModes cullMode;
    _SceneEntity::GlEnablements::fillModes fillMode;

    frameBufferMode = g.frameBufferMode;
    switch (frameBufferMode){
    case _SceneEntity::GlEnablements::Blend:
        glEnable(GL_BLEND);//for transparency in alpha values
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//define how the blending needs to be applied
                break;
    case _SceneEntity::GlEnablements::Depth:
        glEnable(GL_DEPTH_TEST);// Enable depth test
        glDepthFunc(GL_LESS);//Accept fragment if it closer to the camera than the former one
                break;
    case _SceneEntity::GlEnablements::Stencil:
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
/*
 * This is your proprietory draw function
 * Draws frames on a avg of 60frames per second(is subjective and changes with hardware)
 * Used by: the _glWidget class paintGl().
*/
void _Renderer::_Renderer::draw()
{
    setGLEnablements();//function sets openGL Rasterisation modifiers


    if(sceneEntity.getIsActive())
    {
        //Using the shader program in the current context
        shdr->useShaderProgram();
        //Bind Textures
        for(uint t=0;t<textures.size();t++){
            textures[t].bind(t+1);
        }
        //Bind the Buffers data of the respective buffer object(only needed if mesh need chenging on runtime)
        if(sceneEntity.getIsMeshEditable())
        {
            glBindBuffer(GL_ARRAY_BUFFER,VBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
        }
        //Bind the VAO of the respective buffer object (needs to be bound everytime)
        glBindVertexArray(VAO);
        //
        //Sets the values for the MVP matrix in the vertex shader
        glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(sceneEntity.getViewMatrix()));
        glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(sceneEntity.getProjectionMatrix()));
        glUniformMatrix4fv(modelUnifrom,1,GL_FALSE,glm::value_ptr(sceneEntity.getModelMatrix()));
        //glUniformMatrix4fv(modelUnifrom, 1, GL_FALSE, glm::value_ptr(sceneEntity.getTranslationMatrix()*sceneEntity.getRotationmatrix()*pivotTmat *sceneEntity.getScaleingMatrix()));
        //
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);//The Final draw call for each frame
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
    if(sceneEntity.getisHitByRay())
        sceneEntity.setColor(actualColor * 0.5);
    else if(sceneEntity.getIsHitByTri()){
        QVector4D qc = actualColor;
         qc.setX(actualColor.x() + 1.0f);
        sceneEntity.setColor(qc);}
    else
        sceneEntity.setColor(actualColor);
}

void _Renderer::updateMaterial(_Material m)
{
    sceneEntity.getMaterial();
}

/*
 *  Used in the Draw functon
 * Updates the light uniforms on the model
 * is called in the Scene class in the draw function();
 */
void _Renderer::updateLightUniforms(_Light l)//neds a restructure
{

    glUniform3f(shdr->getUniformLocation("viewPos"),camposForLight.x,camposForLight.y,camposForLight.z);
    glUniform3f(shdr->getUniformLocation("light.position"),l.getPosition().x,l.getPosition().y,l.getPosition().z);

    // light properties
    glUniform3f(shdr->getUniformLocation("light.ambient"), l.getColor().x,l.getColor().y,l.getColor().z); // note that all light colors are set at full intensity
    glUniform3f(shdr->getUniformLocation("light.diffuse"), l.getDiffuse(),l.getDiffuse(),l.getDiffuse());
    glUniform3f(shdr->getUniformLocation("light.specular"), l.getSpecular(),l.getSpecular(),l.getSpecular());

    // material properties
    glUniform3f(shdr->getUniformLocation("material.ambient"), sceneEntity.getMaterial().getAmbient().x,sceneEntity.getMaterial().getAmbient().y,sceneEntity.getMaterial().getAmbient().z);
    glUniform3f(shdr->getUniformLocation("material.diffuse"), sceneEntity.getMaterial().getDiffuse().x,sceneEntity.getMaterial().getDiffuse().y,sceneEntity.getMaterial().getDiffuse().z);
    glUniform3f(shdr->getUniformLocation("material.specular"), sceneEntity.getMaterial().getSpecular().x,sceneEntity.getMaterial().getSpecular().y,sceneEntity.getMaterial().getSpecular().z);
    glUniform1f(shdr->getUniformLocation("material.shininess"), sceneEntity.getMaterial().getShine());

    glUniform1f(shdr->getUniformLocation("ambientStrength"),l.getAmbientStr());
}

