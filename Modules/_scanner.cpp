#include "_scanner.h"
#include <QDebug>
#include <_texture.h>
#include <_shader.h>
#include <Compute/_cpu_compute.h>
#include <Compute/_gpu_compute.h>

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
    if (!context){
        context = new QOpenGLContext(this);
        context->setFormat(format);
    }

    if(!context->isValid()){
        qDebug() << "GL context not Valid";
    }

    surface = new QOffscreenSurface();
    surface->setFormat(context->format());
    //if glWidget's context need to be shared with scanner context, make glwidget's context current first and uncomment following line
    //context->setShareContext(QOpenGLContext::currentContext());
    surface->create();
    qDebug() << "offscreen render init";
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

    cpu_compute = new _Cpu_Compute;

    initializeOpenGLFunctions();

    connect(this,SIGNAL(set_image(char*,unsigned int,unsigned int)),machine,SLOT(updateFrameColor(char*,unsigned int ,unsigned int)));
}


/* Function : scan(_Scanner::ScanType scan_type)
 * this function scans the stone by rotating stage by 360/n degrees every step
 * where n :number_of_rotation_steps_per_scan_epoch
 */
void _Scanner::scan(_Scanner::ScanType scan_type)
{

    machine->frameUpdateMutex.lock();// display will not be updated by camera frame directly

    //read scan defaults from config
    int number_of_rotation_steps_per_scan_epoch = machine->config["Hardware"]["Scan"]["Default"].getFloatEntity("ROTATION_STEP"); //200
    int number_of_stepper_steps_per_rotaion = machine->config["Hardware"]["Controls"]["StageMotor"]["Data"]["Default"].getFloatEntity("STEPS_PER_ROTAION");//16000
    glm::vec2 stage_center;
    stage_center.x = machine->config["Hardware"]["Scan"]["Caliberation"].getFloatEntity("STAGE_CENTER_X");
    stage_center.y = machine->config["Hardware"]["Scan"]["Caliberation"].getFloatEntity("STAGE_CENTER_Y");


    //set default parameters for scanning
    //machine->LineLaser(0,_DEFAULT_FROM_CONFIG);
    //machine->BackLight(0,_DEFAULT);//toggle force off

    //scan loop
    for(int t = 0;t<number_of_rotation_steps_per_scan_epoch;t++)
    {
        QString filename = QString("scan_image_stage_%1").arg(t);

        //move the stage by calculated steps
        machine->TurnTableMotorDiff(number_of_stepper_steps_per_rotaion/number_of_rotation_steps_per_scan_epoch);

        //grab new frame from camera
        machine->GrabFrame(filename);

        //send the grabbed frame to gui widget for display
        //emit set_image(machine->camera->get_frame(),machine->camera->getWidth(),machine->camera->getHeight());


        //process the frame according to scan type
        switch (scan_type) {
        case ScanImagesOnly:
        {
            //pass frame as it is by converting from gray to rgba
            processing->passThroughFrame(machine->camera->get_frame(),machine->camera->getWidth(),machine->camera->getHeight());
            break;
        }
        case ScanLaserLine:
        {
            //scan line laser for concave mapping
            processing->markLineLaser(machine->camera->get_frame(),machine->camera->getWidth(),machine->camera->getHeight(),t);
            break;
        }
        case ScanModelWrapGPU:
        {
            //gpu compute scan wrap mesh
            processing->generateEdgeModel(machine->camera->get_frame(),machine->camera->getWidth(),machine->camera->getHeight(),t,stage_center);
            break;
        }
        case ScanModelWrapCVGPU:
        {
            //
            //processing->generateEdgeModel(machine->camera->get_frame(),machine->camera->getWidth(),machine->camera->getHeight(),t,stage_center);
            break;
        }
        case ScanModelEdgeCV:
        {
            //
            //processing->generateEdgeModel(machine->camera->get_frame(),machine->camera->getWidth(),machine->camera->getHeight(),t,stage_center);
            break;
        }
        case ScanModelEdgeCVVoxels:
        {
            //
            //processing->generateEdgeModel(machine->camera->get_frame(),machine->camera->getWidth(),machine->camera->getHeight(),t,stage_center);
            break;
        }
        }

    }

    machine->frameUpdateMutex.unlock();

}

