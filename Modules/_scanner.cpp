#include "_scanner.h"
#include <QDebug>
#include <_texture.h>
#include <_shader.h>

/*
 * Scanner Class
 * used for converting from camera images to 3d model
 * contains a seperate GL context in thread
 * moveToThread should be called after object creation and signal slots assigning
 * does image preprocessing operations from texture to texture in compute shader
 *
 * Author : Saurabh
 * created : 23_02_2019
*/

/*
 * Scanner class empty costructor
 */
_Scanner::_Scanner(QObject *parent) : QObject(parent) , QOpenGLExtraFunctions()
{

}

/* Scanner class constructor
 * takes machine object as input which is common to while application
 * creates a offscreen surface to do the compute operations
 * surface will be created in parent thread when instance is created
 *
 */
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

/*
 * Function : init
 * should be called when thread starts inside hardware interaction thread
 * creates context and initializes _GPU_Compute object to d compute operations
 * sets defaults for hardware interation
*/
void _Scanner::init()
{
    //creating context
    if (!context->create())
        qFatal("Cannot create the requested OpenGL context!");
    else {
        //make context active
        bool success = context->makeCurrent(surface);
        qDebug() << "making context current in thread" << QThread::currentThread()<< "success:" << success;

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
}


/* Function : scan_save_images()
 * this function only saves the images captured by camera
*/
void _Scanner::scan_save_images()
{
    for(int t = 0;t<200;t++)
    {
        //move the stage by 80 steps
        machine->TurnTableMotorDiff(80);

        //grab new frame from camera
        machine->GrabFrame(QString("scan_image_stage_%1").arg(t));

        //send the grabbed frame to gui widget for display
        emit set_image(machine->camera->get_frame(),1360,1024);
    }
}


/* Function : scan_generate_model()
 * this function scans the stone by rotating stage by 1.8 degrees every step
 * captures the image and does preprocessing on the image(currently)
 * this function should generate a 3d model of stone-- afterwards
*/
void _Scanner::scan_generate_model()
{
    bool success = context->makeCurrent(surface);
    qDebug() << "making context current in thread" << QThread::currentThread()<< "success:" << success;

    //FILE* imagefile;

    unsigned int rttWidth = 1360;
    unsigned int rttHeight = 1024;

    char *colours=new char[rttWidth*rttHeight];

    //initialise empty textures for processing
    static _Texture texture(0,rttWidth,rttHeight);
    static _Texture texture_out(0,rttWidth,rttHeight);
    static _Texture texture_outt(0,rttWidth,rttHeight);

    unsigned int framebuffer;

    //load texture
    texture.load(GL_RED,GL_UNSIGNED_BYTE);
    texture_out.load(GL_RED,GL_UNSIGNED_BYTE);
    texture_outt.load(GL_RED,GL_UNSIGNED_BYTE);

    //texture.unbind();

    //create framebuffer to get processed image from texture should use glGetTexImage afterwards
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
        machine->GrabFrame(filename);

        //send the grabbed frame to gui widget for display
        //emit set_image(machine->camera->get_frame(),1360,1024);

        //Do the Processing

        //send the image to gpu texture
        texture.setImage(machine->camera->get_frame(),1360,1024);

        //compute operation(edge detection currently)
        //gpu_compute->compute_sobel_edge(texture,texture_out);
        gpu_compute->compute_threshold(texture,texture_outt);
        gpu_compute->compute_sobel_edge(texture_outt,texture_out);

        //bind to framebuffer for grabbing
        texture_out.bindForFramebuffer();

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glViewport(0, 0, rttWidth, rttHeight);

        //get image from gpu texture
        glReadPixels(0, 0, rttWidth, rttHeight,GL_RED, GL_UNSIGNED_BYTE,colours);

        //send signal to update display texture
        emit set_image(colours,1360,1024);

        //For saving processed image
//        imagefile=fopen(filename.toLocal8Bit(), "wb");

//        if( imagefile == NULL) {
//            qDebug() << "Can't create:" << filename;
//        }
//        fprintf(imagefile,"P5\n%u %u 255\n", rttWidth, rttHeight);
//        fwrite(colours, 1, rttWidth*rttHeight, imagefile);
//        fclose(imagefile);

        //qDebug() << "wrote: " << filename;

    }

    //delete imagefile;
}
