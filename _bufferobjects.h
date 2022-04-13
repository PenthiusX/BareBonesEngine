#pragma once

#include <qopenglextrafunctions.h>
#include "_shader.h"

/*
 * Class: _FrameBuffer
 * Creates an individual instace for the setting and
 * binding of the framebufferObjects
*/
class _FrameBuffer : public QOpenGLExtraFunctions
{
public:
    _FrameBuffer();
    ~_FrameBuffer();

private:
    GLuint frameBuffer;
    GLuint textureColorbuffer;
    GLuint renderBufferObject;
    GLuint depthrenderbuffer;
    GLuint quadVAO,quadVBO;

    GLuint mousePosUniform;
    QVector2D mousePos;

    _Shader *fboShader;//local shader for the Fbo quad
    void setupQuad();//setup the parameters for the Quad object to render the fbuffer on

    uint resH,resW;

public:
    void initialise();//initialises the framebuffer objects
    void setupFramebuffer(int w , int h);//can be called once but needs to update if the window is resized
    void setUpMsAAFrameBuffer(int w , int h);
    //
    void setUpdatedFrame();
    void renderFrameOnQuad();
    void renderFrameOn();
    //
    void setMousePos(QVector2D mousePos);
};


class _StencilBuffer : public QOpenGLExtraFunctions
{
public:

    _StencilBuffer();
    ~_StencilBuffer();

    void dontWriteToStencil();
    void writeToStencilPass();
    void drawStencilPass();
    void clearStencilBuffer();

private:

};

/*
 *
 *
 */
class _ShadowBuffer : public QOpenGLExtraFunctions
{
public:
    _ShadowBuffer();
    ~_ShadowBuffer();

    void init();
    void onResize(uint wRes,uint hRes);
    void startWriteToDepthBuffer();
    void stopWrite();
    GLuint getShadowDepthTexture();



private:
   GLuint depthMapFBO;
   GLuint depthMap;
};
