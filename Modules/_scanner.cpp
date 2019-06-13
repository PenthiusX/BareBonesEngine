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
_Scanner::_Scanner(_Machine *global_machine,_Processing* proc,QObject *parent) : QObject(parent) ,QOpenGLExtraFunctions()
{
    machine = global_machine;
    processing = proc;

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

    bool success = processing->makeCurrent();
    //qDebug() << "making context current in thread" << QThread::currentThread()<< "success:" << success;

    gpu_compute = new _GPU_Compute;

    initializeOpenGLFunctions();

    connect(this,SIGNAL(set_image(char*,unsigned int,unsigned int)),machine,SLOT(updateFrameColor(char*,unsigned int ,unsigned int)));
}


/* Function : scan_save_images()
 * this function only saves the images captured by camera
 */
void _Scanner::scanImages()
{
    machine->frameUpdateMutex.lock();// display will not be updated by camera frame

    for(int t = 0;t<200;t++)
    {
        QString filename = QString("scan_image_stage_%1").arg(t);

        //move the stage by 80 steps
        machine->TurnTableMotorDiff(80);

        //grab new frame from camera
        machine->GrabFrame(filename);

        //send the grabbed frame to gui widget for display
        //emit set_image(machine->camera->get_frame(),machine->camera->getWidth(),machine->camera->getHeight());

        processing->passThroughFrame(machine->camera->get_frame(),machine->camera->getWidth(),machine->camera->getHeight());

    }

    machine->frameUpdateMutex.unlock();
}


/* Function : scan_generate_model()
 * this function scans the stone by rotating stage by 1.8 degrees every step
 * captures the image and does preprocessing on the image(currently)
 * this function should generate a 3d model of stone-- afterwards
*/
void _Scanner::scanGenerateModel()
{
    bool success = processing->makeCurrent();
    qDebug() << "making context current in thread" << QThread::currentThread()<< "success:" << success;


    machine->frameUpdateMutex.lock();// display will not be updated by camera frame

    for(int t = 0;t<200;t++)
    {
        QString filename = QString("scan_image_stage_%1").arg(t);
        //move the stage by 80 steps
        machine->TurnTableMotorDiff(80);
        //QThread::msleep(100);

        //grab new frame from camera
        machine->GrabFrame(filename);

        //send the grabbed frame to gui widget for display
        //emit set_image(machine->camera->get_frame(),machine->camera->getWidth(),machine->camera->getHeight());

        processing->markLineLaser(machine->camera->get_frame(),machine->camera->getWidth(),machine->camera->getHeight(),t);


        //For saving processed image
        //filename = QString("processed_image_stage_%1.pgm").arg(t);
        //_Tools::SaveImageToPgm(colours, 1, machine->camera->getWidth()*machine->camera->getHeight(), filename);

    }

    glUniform1f(4,4);

    machine->frameUpdateMutex.unlock();

    //delete imagefile;
}

/* Function : scan_generate_model()
 * this function scans the stone by rotating stage by 1.8 degrees every step
 * captures the image and does preprocessing on the image(currently)
 * this function should generate a 3d model of stone-- afterwards
*/
void _Scanner::scanGenerateModelEdge()
{
    bool success = processing->makeCurrent();
    qDebug() << "making context current in thread" << QThread::currentThread()<< "success:" << success;


    machine->frameUpdateMutex.lock();// display will not be updated by camera frame

    glm::vec2 stage_center;

    stage_center.x = machine->config["Hardware"]["Scan"]["Caliberation"].getFloatEntity("STAGE_CENTER_X");
    stage_center.y = machine->config["Hardware"]["Scan"]["Caliberation"].getFloatEntity("STAGE_CENTER_Y");

    for(int t = 0;t<200;t++)
    {
        QString filename = QString("scan_image_stage_%1").arg(t);
        //move the stage by 80 steps
        machine->TurnTableMotorDiff(80);
        //QThread::msleep(50);

        //grab new frame from camera
        machine->GrabFrame(filename);

        //send the grabbed frame to gui widget for display
        //emit set_image(machine->camera->get_frame(),machine->camera->getWidth(),machine->camera->getHeight());

        processing->generateEdgeModel(machine->camera->get_frame(),machine->camera->getWidth(),machine->camera->getHeight(),t,stage_center);

        //For saving processed image
        //filename = QString("processed_image_stage_%1.pgm").arg(t);
        //_Tools::SaveImageToPgm(colours, 1, machine->camera->getWidth()*machine->camera->getHeight(), filename);

    }

    machine->frameUpdateMutex.unlock();

    //delete imagefile;
}

void _Scanner::scanGenerateModelVoxels()
{
    bool success = processing->makeCurrent();
    qDebug() << "making context current in thread" << QThread::currentThread()<< "success:" << success;


    machine->frameUpdateMutex.lock();// display will not be updated by camera frame

    glm::vec2 stage_center;

    stage_center.x = machine->config["Hardware"]["Scan"]["Caliberation"].getFloatEntity("STAGE_CENTER_X");
    stage_center.y = machine->config["Hardware"]["Scan"]["Caliberation"].getFloatEntity("STAGE_CENTER_Y");

    for(int t = 0;t<100;t++)
    {
        QString filename = QString("scan_image_stage_%1").arg(t+20);
        //move the stage by 80 steps
        machine->TurnTableMotorDiff(80);
        //QThread::msleep(50);

        //grab new frame from camera
        machine->GrabFrame(filename);

        //send the grabbed frame to gui widget for display
        //emit set_image(machine->camera->get_frame(),machine->camera->getWidth(),machine->camera->getHeight());

        processing->generateVoxelsModelCV(machine->camera->get_frame(),machine->camera->getWidth(),machine->camera->getHeight(),t,stage_center);

    }

    machine->frameUpdateMutex.unlock();

    //delete imagefile;
}
