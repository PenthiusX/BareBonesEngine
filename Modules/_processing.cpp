#include "_processing.h"
#include <QDebug>
#include <QThread>
#include <QMetaMethod>


/* Processing class
*/
_Processing::_Processing(QObject *parent) : QObject(parent) , QOpenGLExtraFunctions()
{
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
void _Processing::setActiveProcess(const char* slot)
{
    if(active_slot)
        disconnect(this,SIGNAL(inputImageRecived(char*,unsigned int,unsigned int)),this,active_slot);
    if(slot)
        connect(this,SIGNAL(inputImageRecived(char*,unsigned int,unsigned int)),this,slot);
    active_slot = slot;
}

void _Processing::inputImage(char *img, unsigned int iwidth, unsigned int iheight)
{
    emit inputImageRecived(img,iwidth,iheight);
}

void _Processing::passThroughFrame(char *img, unsigned int iwidth, unsigned int iheight)
{
    if(!colorFrame) colorFrame = new char[MAX_FRAME_WIDTH*MAX_FRAME_HEIGHT*4];
    for (int index = 0; index < (iwidth*iheight); index++) {
        colorFrame[index*4] = img[index];
        colorFrame[index*4+1] = img[index];
        colorFrame[index*4+2] = img[index];
        colorFrame[index*4+3] = 255;
    }
    emit outputImage(colorFrame,iwidth,iheight);
}

void _Processing::init()
{
    if (!context->create())
        qFatal("Cannot create the requested OpenGL context!");
    else {
        //make context active
        bool success = context->makeCurrent(surface);
        qDebug() << "making context current in thread processing" << QThread::currentThread()<< "success:" << success;

        //qDebug() << "making context current in thread" << QThread::currentThread()<< "success:" << success;

        initializeOpenGLFunctions();

        //this object will handle all compute operations
        gpu_compute = new _GPU_Compute();//should be created when context is active

        //checking if gl context is working

        qDebug() << QString::fromUtf8((char*)glGetString(GL_VERSION));

        //GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS

                int workGroupSizes[3] = { 0 };
                glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &workGroupSizes[0]);
                glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &workGroupSizes[1]);
                glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &workGroupSizes[2]);
                int workGroupCounts[3] = { 0 };
                glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &workGroupCounts[0]);
                glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &workGroupCounts[1]);
                glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &workGroupCounts[2]);
                int max_compute_workgroup_invocations;
                //glGetIntegeri(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS,&max_compute_workgroup_invocations);

        qDebug()<< "workGroupSizes" << workGroupSizes[0] << " " << workGroupSizes[1]<< " " << workGroupSizes[2];
        qDebug()<< "workGroupCounts" << workGroupCounts[0] << " " << workGroupCounts[1]<< " " << workGroupCounts[2];
        qDebug()<< "max_compute_workgroup_invocations" << max_compute_workgroup_invocations;

    }

    //connect(this,SIGNAL(set_image(char*,unsigned int,unsigned int)),machine,SLOT(updateFrameColor(char*,unsigned int ,unsigned int)));
}

void _Processing::markLineLaser(char *img, unsigned int iwidth, unsigned int iheight)
{
    static bool init = true;
    static _Texture texture(nullptr,iwidth,iheight);
    static _Texture texture_out(nullptr,iwidth,iheight);
    static _Texture texture_outt(nullptr,iwidth,iheight);

    if(init)
    {


    //initialise empty textures for processing


    //load texture
    texture.load(GL_RED,GL_UNSIGNED_BYTE);
    texture_out.load(GL_RGBA,GL_UNSIGNED_BYTE);
    texture_outt.load(GL_R32I,GL_RED_INTEGER, GL_INT);

    //texture.unbind();
    init = false;
    }
    //Do the Processing

    //send the image to gpu texture
    texture.setImage(img,iwidth,iheight);

    //compute operation(edge detecton currently)
    //gpu_compute->compute_row_wise_mean(texture,texture_out);
    //gpu_compute->compute_threshold(texture,texture_outt);
//        gpu_compute->compute_sobel_edge(texture_outt,texture_out);
//        gpu_compute->compute_copy_8_to_32(texture,texture_outt);
//        gpu_compute->compute_copy_32_to_8(texture_outt,texture_out);
    //gpu_compute->compute_canny_edge(texture_outt,texture_out);
    gpu_compute->compute_row_wise_arg_max(texture,texture_outt);
    gpu_compute->compute_copy_red_to_rgba(texture,texture_out);
    gpu_compute->compute_mark_column_index(texture_outt,texture_out);
    //gpu_compute->compute_copy_32_to_8(texture_outt,texture_out);

    //bind to framebuffer for grabbing

    //get image from gpu texture
    //glReadPixels(0, 0, iwidth, iheight,GL_RGBA, GL_UNSIGNED_BYTE,colorFrame);

    //send signal to update display texture
    emit outputImage(gpu_compute->get_texture_image_framebuffer(texture_out),iwidth,iheight);

}

bool _Processing::makeCurrent()
{
    return context->makeCurrent(surface);
}

void _Processing::doneCurrent()
{
    context->doneCurrent();
}
