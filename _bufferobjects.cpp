#include "_bufferobjects.h"
#include "_tools.h"

/*
 * Class: _FrameBuffer
 * Creates an individual instace for the setting and
 * binding of the framebufferObjects
 * Author: Aditya
*/
_FrameBuffer::_FrameBuffer() : QOpenGLExtraFunctions(QOpenGLContext::currentContext())
{
    fboShader = new _Shader();
}
_FrameBuffer::~_FrameBuffer()
{
    fboShader = nullptr;
    delete fboShader;
}
/*
 * this function sets up a basic quad to render the frabbuffer texture to
 * we put it in front of the camera for postprocessing implementations.
*/
void _FrameBuffer::setupQuad()
{
    fboShader->attachShaders(":/shaders/fboTexVshader.glsl", ":/shaders/fboTexFshader.glsl");
    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
                             // positions   // texCoords
                             -1.0f,  1.0f,  0.0f, 1.0f,
                             -1.0f, -1.0f,  0.0f, 0.0f,
                             1.0f, -1.0f,  1.0f, 0.0f,

                             -1.0f,  1.0f,  0.0f, 1.0f,
                             1.0f, -1.0f,  1.0f, 0.0f,
                             1.0f,  1.0f,  1.0f, 1.0f
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
}
/*
 *
 * this function sets up the the buffers for framebuffer to Texture rendering
*/
void _FrameBuffer::setupFramebuffer(int resWidth, int resHeight)
{
    resH = resHeight;
    resW = resWidth;

    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    // create a color attachment texture
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, resW, resH, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    glGenRenderbuffers(1, &renderBufferObject);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObject);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, resW, resH); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObject); // now actually attach it

    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        qDebug() << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
    }else{
        qDebug() << "Frambuffer is Initialisation Complete" ;
    }
    //reset to the default framebuffer 0
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, resH, resW );//revert vieport back to default
}
/*
 * this function calls to all function needed on initialisation
 * should be called on resize as well if you need the Framebuffer texture to
 * be the correct respective resolution on resize.
*/
void _FrameBuffer::initialise()
{
    qDebug() << "--------------Initailising Frambuffer -----------------";
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    //
    setupFramebuffer(viewport[2],viewport[3]);
    qDebug() << "Frambuffer initailised";
    setupQuad();
    qDebug() << "Quad setup complete";
    mousePosUniform = fboShader->getUniformLocation("iMouse");
    qDebug() << "mousePosUniform->" << mousePosUniform << "FBO";
}
/*
 * this function set the Frame for the frameBuffer
 * all the scene object Related bindings in the draw call like : shaderdata,model,etc
 * come after this so that it reads to frame and updates it in the renderFrame() function
 * which comes after.
*/
void _FrameBuffer::setUpdatedFrame()
{

    //    GLint viewport[4];
    //    glGetIntegerv(GL_MAX_VIEWPORT_DIMS, viewport);
    //    glViewport(0, 0, resW, resH);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);//bind the framebuffer instance to store the current frame on
    glEnable(GL_DEPTH_TEST | GL_STENCIL_TEST);// enable depth and stencil testing (is disabled for rendering screen-space quad)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//clear for goodmeasure
    //glViewport(0, 0, resW, resH);
}
/*
 * this function will render the frame generated via setFrame, and render it on the Quad
 * that has been set in setupQuad() and rendered below.
*/
void _FrameBuffer::renderFrameOnQuad()
{

    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);//always force the FBO quad to have a GL_FILL hint.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);// now bind the default(orignal) frame , draw a quad plane attaching the frambuffer texture on it.
    //glDisable(GL_DEPTH_TEST | GL_STENCIL_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

    fboShader->useShaderProgram();//pass the texture to this fboShader

    glBindVertexArray(quadVAO);//bind VAO for quad

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,textureColorbuffer);//bind the texture created above

    glUniform2f(mousePosUniform,mousePos.x(),mousePos.y());//passing mouse value to fboshader

    glDrawArrays(GL_TRIANGLES, 0, 6);//Draw the Quad with the texture
    glBindVertexArray(0);//Clear the buffer
}
/*
 * this function should help with implementation of the applying a FrameTextuere onto any
 * Object passed into it.
*/
void _FrameBuffer::renderFrameOn()
{
    //implentation pending
}
/*
*/
void _FrameBuffer::setMousePos(QVector2D mPos)
{
    //mouse y coordinate values are inverse of the the screen coordinate values
    //aligning it here the same way the resolution Height values(from gl_FragCoords) are aligned in the FBOshader.
    uint alignedMouseheight = resH - (uint)mPos.y();
    mousePos.setX(mPos.x());
    //this allignment is needed for it to be represented acurately in the shade.
    mousePos.setY(alignedMouseheight);
}

_StencilBuffer::_StencilBuffer() : QOpenGLExtraFunctions(QOpenGLContext::currentContext())
{

}

_StencilBuffer::~_StencilBuffer()
{

}
// We set its mask to 0x00 to not write to the stencil buffer.
void _StencilBuffer::dontWriteToStencil()
{
    glStencilMask(0x00);
}

void _StencilBuffer::writeToStencilPass()
{
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
}

void _StencilBuffer::drawStencilPass()
{
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
}

void _StencilBuffer::clearStencilBuffer()
{
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
}



_ShadowBuffer::_ShadowBuffer() : QOpenGLExtraFunctions(QOpenGLContext::currentContext()){}
_ShadowBuffer::~_ShadowBuffer(){ delete sbShader;}

void _ShadowBuffer::init()
{
    sbShader = new _Shader();

    glGenFramebuffers(1, &depthMapFBO);
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;//shadow res

    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void _ShadowBuffer::startWriteToDepthBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
}

void _ShadowBuffer::stopWrite()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint _ShadowBuffer::getShadowDepthTexture()
{
    return this->depthMap;
}
