#include "_framebuffer.h"
#include "_tools.h"

/*
 * Class: _FrameBuffer
 * Creates an individual instace for the setting and
 * binding of the framebufferObjects
 * Created: 30_04_2019
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

/* Function: setupQuad()
 * this function sets up a basic quad to render the frabbuffer texture to
 * we put it in front of the camera for postprocessing implementations.
 * Created: 30_04_2019
*/
void _FrameBuffer::setupQuad()
{
    fboShader->attachShaders(":/shaders/fboTexVshader.glsl", ":/shaders/fboTexFshader.glsl");
    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
                             // positions   // texCoords
                             -1.00f,  1.0f,  0.0f, 1.0f,
                             -1.00f, -1.0f,  0.0f, 0.0f,
                             1.00f, -1.0f,  1.0f, 0.0f,

                             -1.00f,  1.0f,  0.0f, 1.0f,
                             1.00f, -1.0f,  1.0f, 0.0f,
                             1.00f,  1.0f,  1.0f, 1.0f
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
 * Function: setupFramebufferObjects(int resWidth, int resHeight)
 * this function sets up the the buffers for framebuffer to Texture rendering
 * Created: 30_04_2019
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

/*
 * Function: initialise()
 * this function calls to all function needed on initialisation
 * should be called on resize as well if you need the Framebuffer texture to
 * be the correct respective resolution on resize.
 * Created: 30_04_2019
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
 * Function: setFrame()
 * this function set the Frame for the frameBuffer
 * all the scene object Related bindings in the draw call like : shaderdata,model,etc
 * come after this so that it reads to frame and updates it in the renderFrame() function
 * which comes after.
 * Created: 30_04_2019
*/
void _FrameBuffer::setUpdatedFrame()
{
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);//bind the framebuffer instance to store the current frame on
    glEnable(GL_DEPTH_TEST | GL_STENCIL_TEST);// enable depth and stencil testing (is disabled for rendering screen-space quad)
}

/*
 * Function: renderFrameOnQuad()
 * this function will render the frame generated via setFrame, and render it on the Quad
 * that has been set in setupQuad() and rendered below.
 * Created: 30_04_2019
*/
void _FrameBuffer::renderFrameOnQuad()
{
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);//always force the FBO quad to have a GL_FILL hint.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);// now bind the default(orignal) frame , draw a quad plane attaching the frambuffer texture on it.
    glDisable(GL_DEPTH_TEST | GL_STENCIL_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//clear for goodmeasure

    glBindTexture(GL_TEXTURE_2D,textureColorbuffer);//bind the texture created above
    fboShader->useShaderProgram();//pass the texture to this fboShader

    glBindVertexArray(quadVAO);//bind VAO for quad
    glUniform2f(mousePosUniform,mousePos.x(),mousePos.y());//passing mouse value to fboshader

    glDrawArrays(GL_TRIANGLES, 0, 6);//Draw the Quad with the texture
}
/*
 * Function: renderFrameOn()
 * this function should help with implementation of the applying a FrameTextuere onto any
 * Object passed into it.
 * Created: 30_04_2019
*/
void _FrameBuffer::renderFrameOn()
{
    //implentation pending
}
/*
 * Created: 30_04_2019
*/
void _FrameBuffer::setMousePos(QVector2D mPos)
{
    //mouse y coordinate values are inverse of the the screen coordinate values
    //aligning it here the same way the resolution Height values(from gl_FragCoords) are aligned in the FBOshader.
    unsigned int alignedMouseheight = resH - (unsigned int)mPos.y();
    mousePos.setX(mPos.x());
    //this allignment is needed for it to be represented acurately in the shade.
    mousePos.setY(alignedMouseheight);
}
