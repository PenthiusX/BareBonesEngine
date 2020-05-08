#ifndef _AVT_CAMERA_H
#define _AVT_CAMERA_H

#include <IO/_hwdcamera.h>
#include <QString>

#   ifdef PLATFORM_WIN

#include "FGCamera.h"


/*
 * The _AVT_Camera class
 * To interface firewire prosilica camera using Allied Vision Technologies driver
 * driver installer situated in Dependancies/win/AVTFirePackage3_1_1.exe
 * Library inside "Dependancies/win/Lib" for 64 bit windows
 * with upper level functions
 * will be used as backend camera for higher level camera class
 * Author: Aditya
*/
class _AVT_Camera :public _HWDCamera
{
//public:
//    CFGCamera  Camera;
//    UINT32     Result;
//    FGNODEINFO NodeInfo[3];
//    UINT32     NodeCnt;
//    FGFRAME    Frame;
//    FILE* imagefile;
//    char *byteframe = nullptr;


//    _AVT_Camera();

//    int list_cameras();

//    int select_camera(int v);
//    int init(int v);
//    ~_AVT_Camera();

//    int stop_camera();


//    char* get_frame();

//    int grab_frame();

//    int grab_frame(QString filename);
};
#   endif //PLATFORM_WIN

#endif // _AVT_CAMERA_H
