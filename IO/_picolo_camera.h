#ifndef _PICOLO_CAMERA_H
#define _PICOLO_CAMERA_H

#include <IO/_hwdcamera.h>

typedef int BOOL_1;
typedef void *PVOID_1;
typedef unsigned int        UINT32_1;

typedef int MCSTATUS_1;
typedef UINT32_1 MCHANDLE_1, *PMCHANDLE_1;
typedef UINT32_1 MCPARAMID_1, *PMCPARAMID_1;
typedef int MCEVENT_1, MCSIGNAL_1;

// SIGNALING
typedef struct _MC_CALLBACK_INFO_1 {
    PVOID_1 Context;
    MCHANDLE_1 Instance;
    MCSIGNAL_1 Signal;
    UINT32_1 SignalInfo;
    UINT32_1 SignalContext;
} MCSIGNALINFO_1, *PMCSIGNALINFO_1, *PMCCALLBACKINFO_1, MCCALLBACKINFO_1;

typedef void (_stdcall *PMCCALLBACK_1)(PMCSIGNALINFO_1 CbInfo);
typedef void (_stdcall *PMCCALLBACKEX_1)(PVOID_1 Context);

class _Picolo_Camera : public _HWDCamera
{
public:
    _Picolo_Camera();
    ~_Picolo_Camera();

    int init(int v); //initialize camera
    char* get_frame(); //return frame
    int stop_camera(); //
    static void callbackfunc(PMCSIGNALINFO_1 SigInfo);

    int list_cameras();
    int grab_frame();
    int grab_frame(QString filename);

public:
    MCHANDLE_1 m_Channel;
    int m_BufferPitch;
    static void* Drv_Img_Ptr;
    static volatile int failed_grab;
    static volatile int m_bChannelActive;

    FILE* imagefile;

};

#endif // _PICOLO_CAMERA_H
