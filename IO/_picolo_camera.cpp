#include "_picolo_camera.h"
#include <QDebug>
#include <QThread>

#define EURESYS_SURFACE_COUNT 3
#define filename2 "image.pgm"
#include <multicam.h>
#include <_tools.h>

PVOID _Picolo_Camera::Drv_Img_Ptr = nullptr;
volatile BOOL _Picolo_Camera::failed_grab = FALSE;
volatile BOOL _Picolo_Camera::m_bChannelActive = FALSE;

/*class:  _Picolo_Camera
 * to interface analog camera connected to picolo card
 * this class contains static pointers to image data
 * hence is to be used for only one camera currently
 * created 10_04_2019
 * Author : Saurabh
*/

/* _Picolo_Camera contstructor
 *
*/
_Picolo_Camera::_Picolo_Camera()
{
    m_Channel = 0;
    m_BufferPitch = Width;
}

/* Destructor stops camera
 * Created: 10_04_2019
 */
_Picolo_Camera::~_Picolo_Camera()
{
    qDebug() << "inside destructor";
    stop_camera();
    delete Drv_Img_Ptr;
}

int _Picolo_Camera::init(int v)
{
    McOpenDriver(NULL);

    // Activate message box error handling and generate an error log file
    McSetParamInt (MC_CONFIGURATION, MC_ErrorHandling, MC_ErrorHandling_MSGBOX);
    McSetParamStr (MC_CONFIGURATION, MC_ErrorLog, "error.log");

    // Create a channel and associate it with the first connector on the first board
    McCreate(MC_CHANNEL, &m_Channel);
    McSetParamInt(m_Channel, MC_DriverIndex, v);
    McSetParamStr(m_Channel, MC_Connector, "VID1");
    // Choose the video standard
    McSetParamStr(m_Channel, MC_CamFile, "PAL");
    // Choose the pixel color format
    McSetParamInt(m_Channel, MC_ColorFormat, MC_ColorFormat_Y8);

    // Choose the acquisition mode
    McSetParamInt(m_Channel, MC_AcquisitionMode, MC_AcquisitionMode_VIDEO);
    // Choose the way the first acquisition is triggered
    McSetParamInt(m_Channel, MC_TrigMode, MC_TrigMode_IMMEDIATE);
    // Choose the triggering mode for subsequent acquisitions
    McSetParamInt(m_Channel, MC_NextTrigMode, MC_NextTrigMode_REPEAT);
    // Choose the number of images to acquire
    McSetParamInt(m_Channel, MC_SeqLength_Fr, MC_INDETERMINATE);

    // Retrieve image dimensions
    McGetParamInt(m_Channel, MC_ImageSizeX, &Width);
    McGetParamInt(m_Channel, MC_ImageSizeY, &Height);
    McGetParamInt(m_Channel, MC_BufferPitch, &m_BufferPitch);

    // The memory allocation for the images is automatically done by Multicam when activating the channel.
    // We only set the number of surfaces to be created by MultiCam.
    McSetParamInt(m_Channel, MC_SurfaceCount, EURESYS_SURFACE_COUNT);

    // Enable MultiCam signals
    McSetParamInt(m_Channel, MC_SignalEnable + MC_SIG_SURFACE_PROCESSING, MC_SignalEnable_ON);
    McSetParamInt(m_Channel, MC_SignalEnable + MC_SIG_ACQUISITION_FAILURE, MC_SignalEnable_ON);
    McSetParamInt(m_Channel, MC_SignalEnable + MC_SIG_END_CHANNEL_ACTIVITY, MC_SignalEnable_ON);

    // Register the callback function

    //void (_Picolo_Camera::*pt2ConstMember)(PMCSIGNALINFO) = NULL;

    //PMCCALLBACK pt2ConstMember = static_cast<PMCCALLBACK>(_Picolo_Camera::callbackfunc);

    McRegisterCallback(m_Channel, (PMCCALLBACK)callbackfunc, this);

    McSetParamInt(m_Channel, MC_ChannelState, MC_ChannelState_ACTIVE);

    //qDebug() << "img resolution" << Width <<" " << Height;

    byteframe = new char[Width*Height];

    return 1;
}

/* This Function returns pointer to start of image data
 * used to retrieve image grabbed by camera
 * Created: 10_04_2019
 */
char *_Picolo_Camera::get_frame()
{
    return byteframe;
}


/* Stop camera and exit module/library/driver
 * Created: 10_04_2019
 */
int _Picolo_Camera::stop_camera()
{
    qDebug() << "before state idle";
    McSetParamInt(m_Channel, MC_ChannelState, MC_ChannelState_IDLE);
    // Terminate driver
    qDebug() << "before close driver";
    McCloseDriver();
    qDebug() << "after close driver";
    return 1;
}


/* Function : callbackfunc(PMCSIGNALINFO SigInfo)
 * Static callback functions which is called by camera driver when new camera frame is grabbed
 * new frame is grabbed 25 times per second in loop by driver
 * this function stores the pointer to image data in static variable Drv_Img_Ptr when new frame grabbed
 * image data is in fromat R8 bits as a 1 byte per pixel
*/
void _Picolo_Camera::callbackfunc(PMCSIGNALINFO_1 SigInfo)
{
    switch(SigInfo->Signal)
    {
        case MC_SIG_SURFACE_PROCESSING:
            // Update "current" surface address pointer
            McGetParamInt (SigInfo->SignalInfo, MC_SurfaceAddr, (PINT32) &Drv_Img_Ptr);

            //qDebug() << "pointer" << Drv_Img_Ptr;

            //_Tools::SaveImage(Drv_Img_Ptr,768,576,3,"picolo_grabbed_frame.pgm");

            break;
        case MC_SIG_END_CHANNEL_ACTIVITY:
                m_bChannelActive = FALSE;
                qDebug() << "MC_SIG_END_CHANNEL_ACTIVITY";
            break;
        case MC_SIG_ACQUISITION_FAILURE:
            qDebug() << "Acquisition Failure ! PicoloVideo";
            qDebug() << "MC_SIG_ACQUISITION_FAILURE";
            failed_grab = true;
            break;
        default:
            break;
    }
}

/* Function : list_cameras()
 * since the picolo driver library does not provide a way to count number of cameras
 * we have to connect to a camera and check if a frame is recieved succesfully
 * Created : 10_04_2019
*/
int _Picolo_Camera::list_cameras()
{
    //check if first frame recieved succesfully
    init(0);
    QThread::msleep(200);
    if(failed_grab){
        return 0;
    }
    else return 1;

}

/* Function grab_frame()
 * Capture frame from camera and but do not save it
 * Created: 10_04_2019
 */
int _Picolo_Camera::grab_frame(){
    if(!Drv_Img_Ptr) return 0; //no frame grabbed uptil now

    // ... Here you can processs image data in Frame.pData
    if (!failed_grab){

        memcpy(byteframe,Drv_Img_Ptr,Width*Height);
    }
    else {
        qDebug() <<"failed: grab frame";
        return 0;
    }
    return 1;
}

/* Function grab_frame(QString filename)
 * Capture frame from camera and store inside given file name
 *
 * Created: 10_04_2019
 */
int _Picolo_Camera::grab_frame(QString filename)
{
    if(!Drv_Img_Ptr) return 0; //no frame grabbed uptil now
    filename = images_dir+filename;

    // ... Here you can processs image data in Frame.pData
    if (!failed_grab){

        memcpy(byteframe,Drv_Img_Ptr,Width*Height);
        _Tools::SaveImageToPgm(byteframe,Width,Height,filename);
    }
    else {
        qDebug() <<"failed: grab frame";
        return 0;
    }

    return 1;
}

void _Picolo_Camera::setBrightness(int value)
{
    McSetParamInt(m_Channel, MC_Brightness, value);
}

void _Picolo_Camera::setGain(int value)
{
    McSetParamInt(m_Channel, MC_Gain, value);
}

void _Picolo_Camera::setExposure(int value)
{
    McSetParamInt(m_Channel, MC_Exposure, value);
}

void _Picolo_Camera::setContrast(int value)
{
    McSetParamInt(m_Channel, MC_Contrast, value);
}

void _Picolo_Camera::setOffset(int value)
{
    McSetParamInt(m_Channel, MC_Offset, value);
}

