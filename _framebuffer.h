#ifndef _FRAMEBUFFER_H
#define _FRAMEBUFFER_H

#include <qopenglextrafunctions.h>
#include "_shader.h"

/*
 * Class: _FrameBuffer
 * Creates an individual instace for the setting and
 * binding of the framebufferObjects
 * Created: 30_04_2019
 * Author: Aditya
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
    GLuint quadVAO,quadVBO;

    GLuint mousePosUniform;
    QVector2D mousePos;

    _Shader *fboShader;//local shader for the Fbo quad
    void setupQuad();//setup the parameters for the Quad object to render the fbuffer on

public:
    void initialise();//initialises the framebuffer objects
    void setupFramebufferObjects(int w , int h);//can be called once but needs to update if the window is resized
    //
    void setFrame();
    void renderFrameOnQuad();
    void renderFrameOn();
};
#endif // _FRAMEBUFFER_H
