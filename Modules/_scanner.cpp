#include "_scanner.h"
#include <QDebug>
#include <_texture.h>
#include <_shader.h>

_Scanner::_Scanner(QObject *parent) : QObject(parent) , QOpenGLExtraFunctions()
{

}

_Scanner::_Scanner(_Machine *global_machine,QObject *parent) : QObject(parent) ,QOpenGLExtraFunctions()
{
    machine = global_machine;

    QSurfaceFormat format;

    //create the context
    if (!context) {
        context = new QOpenGLContext(this);
        context->setFormat(format);
    }

    if(!context->isValid())
    {
        qDebug() << "GL context not Valid";
    }

    surface = new QOffscreenSurface();
    surface->setFormat(context->format());

    //if glWidget's context need to be shared with scanner context, make glwidget's context current first and uncomment following line
    //context->setShareContext(QOpenGLContext::currentContext());

    surface->create();

    qDebug() << "offscreen render init" ;

}

void _Scanner::init()
{
    if (!context->create())
        qFatal("Cannot create the requested OpenGL context!");
    else {

        bool success = context->makeCurrent(surface);
        qDebug() << "making context current in thread" << QThread::currentThread()<< "success:" << success;

        initializeOpenGLFunctions();

        qDebug() << QString::fromUtf8((char*)glGetString(GL_VERSION));

    }
}

void _Scanner::scan_save_images()
{
    for(int t = 0;t<200;t++)
    {
        //move the stage by 80 steps
        machine->TurnTableMotorDiff(80);

        //grab new frame from camera
        machine->camera->grab_frame(QString("scan_image_stage_%1").arg(t));

        //send the grabbed frame to gui widget for display
        emit set_image(machine->camera->get_frame(),1360,1024);
    }
}

void _Scanner::scan_generate_model()
{
    bool success = context->makeCurrent(surface);
    qDebug() << "making context current in thread" << QThread::currentThread()<< "success:" << success;

    FILE* imagefile;

    unsigned int rttWidth = 1360;
    unsigned int rttHeight = 1024;

    char *colours=new char[rttWidth*rttHeight];

    _Texture texture(colours,rttWidth,rttHeight);
    _Texture texture_out(colours,rttWidth,rttHeight);

    unsigned int framebuffer;

    texture.load(GL_RED,GL_UNSIGNED_BYTE);
    texture_out.load(GL_RED,GL_UNSIGNED_BYTE);

    //texture.unbind();

    _Shader compute_shader;

    compute_shader.setChildShader(":/shaders/compute_edge.glsl",GL_COMPUTE_SHADER);

    compute_shader.attachShaders();

    glGenFramebuffers(1,&framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);

    texture_out.bindForFramebuffer();

    glGenRenderbuffers(1,&renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT16,rttWidth, rttHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER, renderbuffer);


    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) qDebug() << "fbo complete";
    else qDebug() << "incomplete";

    for(int t = 0;t<200;t++)
    {
        QString filename = QString("scan_image_stage_%1").arg(t);
        //move the stage by 80 steps
        machine->TurnTableMotorDiff(80);

        //grab new frame from camera
        machine->camera->grab_frame(QString("processed_image_stage_%1").arg(t));

        //send the grabbed frame to gui widget for display
        emit set_image(machine->camera->get_frame(),1360,1024);

        //Do the Processing

        texture.setImage(machine->camera->get_frame(),1360,1024);

        texture.bindForCompute(0,GL_R8,GL_READ_ONLY);
        texture_out.bindForCompute(1,GL_R8,GL_WRITE_ONLY);

        compute_shader.useShaderProgram();

        glDispatchCompute(rttWidth / 16, rttHeight / 16, 1);

        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        texture_out.bindForFramebuffer();

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glViewport(0, 0, rttWidth, rttHeight);

        glReadPixels(0, 0, rttWidth, rttHeight,GL_RED, GL_UNSIGNED_BYTE,colours);

        imagefile=fopen(filename.toLocal8Bit(), "wb");

        if( imagefile == NULL) {
            qDebug() << "Can't create:" << filename;
        }
        fprintf(imagefile,"P5\n%u %u 255\n", rttWidth, rttHeight);
        fwrite(colours, 1, rttWidth*rttHeight, imagefile);
        fclose(imagefile);

        qDebug() << "wrote: " << filename;

    }
}
