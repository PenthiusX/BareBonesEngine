#include "_avt_camera.h"
#include <QDebug>
#define filename2 "image.pgm"
#include <FGCamera.h>
#include <_tools.h>

// **All qDebug Logging functionality should be removed afterwards**

#ifdef PLATFORM_WIN

/*
 * The _AVT_Camera class
 * To interface firewire prosilica camera using Allied Vision Technologies driver
 * driver installer situated in Dependancies/win/AVTFirePackage3_1_1.exe
 * Library inside "Dependancies/win/Lib" for 64 bit windows
 * with upper level functions
 * will be used as backend camera for higher level camera class
 * Created: 02_03_2019
 * Author: Saurabh
*/
_AVT_Camera::_AVT_Camera() : _HWDCamera()
{
    Width = MAX_FRAME_WIDTH;
    Height = MAX_FRAME_HEIGHT;
}

/* find and return number of cameras connected
 * Created: 02_03_2019
 */
int _AVT_Camera::list_cameras(){
    // Init module
    Result = FGInitModule(NULL);

    // Get list of connected nodes
    if (Result == FCE_NOERROR)
        Result = FGGetNodeList(NodeInfo, 3, &NodeCnt);
    else qDebug() <<"camera list not found";
    qDebug() <<"found camera list :" << NodeCnt << "  :" << Result;

    return  NodeCnt;
}

/* This function selects camera from
 * attatched firewire avt cameras
 * parameters :
 *     int v : index of camera to be selested from listed cameras
 * returns 1 if camera selested succesfully else 0
 * Created: 02_03_2019
 */
int _AVT_Camera::select_camera(int v){
    // Connect with node
    if (Result == FCE_NOERROR && NodeCnt){
        qDebug() <<"failed: camera select:"<< Result;
        Result = Camera.Connect(&NodeInfo[v].Guid);}
    else {
        qDebug() <<"failed: camera select:"<< Result;
        return 0;
    }
    return 1;
}

/* This function initializes camera from
 * attatched firewire cameras
 * parameters :
 *     int v : index of camera to be selested from listed cameras
 * returns 1 if camera selested succesfully else 0
 * Created: 03_02_2019
 */
int _AVT_Camera::init(int v){

    if(list_cameras()==0) return 0;

    select_camera(v);

    // Start DMA logic
    if (Result == FCE_NOERROR)
        Result = Camera.OpenCapture();
    else {
        qDebug() <<"failed: open capture";
        return 0;
    }

    // Start image device
    if (Result == FCE_NOERROR)
        Result = Camera.StartDevice();
    else {
        qDebug() <<"failed: open capture";
        return 0;
    }
    byteframe = new char[1392640]; //Frame.Length
    return 1;
}

/* Destructor stops camera
 * Created: 03_02_2019
 */
_AVT_Camera::~_AVT_Camera(){
 stop_camera();
}

/* Stop camera and exit module/library
 * Created: 03_02_2019
 */
int _AVT_Camera::stop_camera(){

// Stop the device
Camera.StopDevice();

// Close capture
Camera.CloseCapture();

// Disconnect before ExitModule
Camera.Disconnect();

// Exit module
FGExitModule();
return 1;
}


/* This Function returns pointer to start of image data
 * used to retrieve image grabbed by camera
 * Created: 03_02_2019
 */
char* _AVT_Camera::get_frame(){

    //grab_frame();
//    memcpy(byteframe,&Frame.pData[0],1392640);
    //qDebug() << "camera frame ptr" << (long int)byteframe;
    return byteframe;
    //return (char*)&Frame.pData[0];
}

//currently saving frame in image.pgm
int _AVT_Camera::grab_frame(){
    Result = Camera.GetFrame(&Frame, 5000);

    // ... Here you can processs image data in Frame.pData
    if (Result == FCE_NOERROR){
        //printf("Frame received (%02X %02X %02X %02X ...)\n",Frame.pData[0], Frame.pData[1], Frame.pData[2], Frame.pData[3]);
        memcpy(byteframe,&Frame.pData[0],Width*Height);
    }
    else {
        qDebug() <<"failed: grab frame";
        return 0;
    }

    //return byteframe;
    // Return frame to module
    if (Result == FCE_NOERROR)
        Result = Camera.PutFrame(&Frame);
    else {
        qDebug() <<"failed: return frame";
        return 0;
    }
    return 1;
}

/* Capture frame from camera and store inside given file name
 * Created: 03_02_2019
 */
int _AVT_Camera::grab_frame(QString filename){

    Result = Camera.GetFrame(&Frame, 5000);

    filename = images_dir + filename;

    // ... Here you can processs image data in Frame.pData
    if (Result == FCE_NOERROR){
        //printf("Frame received (%02X %02X %02X %02X ...)\n",Frame.pData[0], Frame.pData[1], Frame.pData[2], Frame.pData[3]);
        memcpy(byteframe,&Frame.pData[0],Width*Height);

        _Tools::SaveImageToPgm(byteframe,Width,Height,filename);
    }
    else {
        qDebug() <<"failed: grab frame";
        return 0;
    }

    //return byteframe;
    // Return frame to module
    if (Result == FCE_NOERROR)
        Result = Camera.PutFrame(&Frame);
    else {
        qDebug() <<"failed: return frame";
        return 0;
    }
    return 1;
}

#endif // PLATFORM_WIN
