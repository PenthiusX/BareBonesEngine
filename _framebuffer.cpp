#include "_framebuffer.h"
#include "_tools.h"

_FrameBuffer::_FrameBuffer() : QOpenGLExtraFunctions(QOpenGLContext::currentContext())
{
    fboShader = new _Shader();
}

_FrameBuffer::~_FrameBuffer()
{
    fboShader = nullptr;
    delete fboShader;
}

void _FrameBuffer::setupQuad()
{
    fboShader->attachShaders(":/shaders/texVshader.glsl", ":/shaders/texFshader.glsl");
    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
                             // positions   // texCoords
                             -0.75f,  0.75f,  0.0f, 1.0f,
                             -0.75f, -0.75f,  0.0f, 0.0f,
                             0.75f, -0.75f,  1.0f, 0.0f,

                             -0.75f,  0.75f,  0.0f, 1.0f,
                             0.75f, -0.75f,  1.0f, 0.0f,
                             0.75f,  0.75f,  1.0f, 1.0f
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

void _FrameBuffer::setupFramebufferObjects(int resWidth, int resHeight)
{
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    // create a color attachment texture
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, resWidth, resHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    glGenRenderbuffers(1, &renderBufferObject);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObject);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, resWidth, resHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObject); // now actually attach it

    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        qDebug() << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;}else{qDebug() << "Frambuffer is Initialisation Complete" ;}
    //reset to the default framebuffer 0
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//initilise once
void _FrameBuffer::initialise()
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    //
    setupFramebufferObjects(viewport[2],viewport[3]);
    setupQuad();
}

//Put at the start of frame
void _FrameBuffer::setFrame()
{
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glEnable(GL_DEPTH_TEST | GL_STENCIL_TEST);// enable depth and stencil testing (is disabled for rendering screen-space quad)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//Put at end of Frame
void _FrameBuffer::renderFrameOnQuad()
{
    // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST | GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    fboShader->useShaderProgram();
    glBindVertexArray(quadVAO);

    glBindTexture(GL_TEXTURE_2D,this->textureColorbuffer);
    glDrawArrays(GL_TRIANGLES, 0, 6);	// use the color attachment texture as the texture of the quad plane
}
