#ifndef _FRAMEBUFFER_H
#define _FRAMEBUFFER_H

#include <qopenglextrafunctions.h>
#include "_shader.h"

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

    _Shader *fboShader;

    void setupQuad();

public:
    void initialise();
    void setFrame();
    void renderFrameOnQuad();
    void renderFrameOn();
    void setupFramebufferObjects(int w , int h);//can be called once but needs to update if the window is resized

};
#endif // _FRAMEBUFFER_H
