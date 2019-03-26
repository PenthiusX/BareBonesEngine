#include "_dc_1394_camera.h"
#include <QDebug>
#define IMAGE_FILE_NAME "image.pgm"

// **All qDebug Logging functionality should be removed afterwards**

#ifdef PLATFORM_LINUX

/*
 * The _DC_1394_Camera class
 * To interface firewire prosilica dc1394 camera with upper level functions
 * will be used as backend camera for higher level camera class
 * requires dependancies libdc1394 ,libraw1394 in linux
 * requires dependancies libdc1394 ,libraw1394, CMU dc1394 in windows
 * Created: 15_02_2019
 * Author: Saurabh
*/

_DC_1394_Camera::_DC_1394_Camera()
{

}

_DC_1394_Camera::~_DC_1394_Camera()
{
    stop_camera();
}

/* This function initializes camera from
 * attatched dc1394 cameras
 * parameters :
 *     int v : index of camera to be selested from listed cameras
 * returns 1 if camera selested succesfully else 0
 * Created: 15_02_2019
 */
int _DC_1394_Camera::init(int v)
{
    //select the camera
    if(!select_camera(v)) return 0;

    //setup camera
    if(!camera_setup_defaults()) return 0;

    err=dc1394_video_set_transmission(camera, DC1394_ON);
    DC1394_ERR_CLN_RTN(err,cleanup_and_exit(camera),"Could not start camera iso transmission");

    return success;
}


char *_DC_1394_Camera::get_frame()
{
    grab_frame();
    return (char*)frame;
}

//currently saving frame in image.pgm
int _DC_1394_Camera::grab_frame()
{
    return grab_frame(IMAGE_FILE_NAME);
}
int _DC_1394_Camera::grab_frame(QString filename)
{

    FILE* imagefile;

    //capture one frame
    err=dc1394_capture_dequeue(camera, DC1394_CAPTURE_POLICY_WAIT, &frame);
    DC1394_ERR_CLN_RTN(err,cleanup_and_exit(camera),"Could not capture a frame\n");


    //check if frame is corrupt
    if( dc1394_capture_is_frame_corrupt( camera, frame) )
    printf("\n[DEBUG] frame is corrupt!\n");

    //save image as 'Image.pgm'
    imagefile=fopen(filename.toLocal8Bit(), "wb");

    if( imagefile == NULL) {
        qDebug() << "Can't create:" << filename;
        cleanup_and_exit(camera);
    }

    dc1394_get_image_size_from_video_mode(camera, video_mode, &width, &height);
    fprintf(imagefile,"P5\n%u %u 255\n", width, height);
    fwrite(frame->image, 1, height*width, imagefile);
    fclose(imagefile);

    qDebug() << "wrote: " << filename;

    //Give back frame to queue
    dc1394_capture_enqueue(camera,frame);
    return 1;
}
int _DC_1394_Camera::stop_camera()
{
    err=dc1394_video_set_transmission(camera,DC1394_OFF);
    DC1394_ERR_CLN_RTN(err,cleanup_and_exit(camera),"Could not stop the camera?\n");
    dc1394_capture_stop(camera);
    dc1394_camera_free(camera);
    dc1394_free (d);
    return 1;
}

int _DC_1394_Camera::list_cameras()
{
    //device_library
    d = dc1394_new ();
    if (!d)
        return 0;
    //check for list of cameras
    err=dc1394_camera_enumerate (d, &list);
    DC1394_ERR_RTN(err,"Failed to enumerate cameras");

    //if mo camera found
    if (list->num == 0) {
        dc1394_log_error("No cameras found");
        return 0;
    }
    return  list->num;
}

/* This function used if error occurs in camera function
 * breaks further operation releases camera and exits
 * parameters :
 *     dc1394camera_t *camera : pointer t0 dc1394 library camera object
 * Created: 15_02_2019
 */
void _DC_1394_Camera::cleanup_and_exit(dc1394camera_t *camera)
{
    dc1394_video_set_transmission(camera, DC1394_OFF);
    dc1394_capture_stop(camera);
    dc1394_camera_free(camera);
    success = 0;
}



/* This function selects camera from
 * attatched dc1394 cameras
 * parameters :
 *     int v : index of camera to be selested from listed cameras
 * returns 1 if camera selested succesfully else 0
 * Created: 15_02_2019
 */
int _DC_1394_Camera::select_camera(int v)
{
    //device_library
    d = dc1394_new ();
    if (!d)
        return 0;

    //check for list of cameras
    err=dc1394_camera_enumerate (d, &list);
    DC1394_ERR_RTN(err,"Failed to enumerate cameras");

    //if mo camera found
    if (list->num == 0) {
        dc1394_log_error("No cameras found");
        return 0;
    }

    //select camera at index v from listed camera
    camera = dc1394_camera_new (d, list->ids[v].guid);
    if (!camera) {
        dc1394_log_error("Failed to initialize camera with guid %llx", list->ids[0].guid);
        return 0;
    }

    //clear list
    dc1394_camera_free_list (list);

    //show selected camera
    qDebug() << QString("Using camera with GUID %1 \n").arg(camera->guid);

    //return 1 if success else 0
    return 1;
}


/* This function set initialization defaults for camera
 * eg: mode , color encoding ,iso speed , flags
 * returns 1 if camera selested succesfully else 0
 * Created: 15_02_2019
 */
int _DC_1394_Camera::camera_setup_defaults()
{
    // get video modes: used if list of modes required
    //err=dc1394_video_get_supported_modes(camera,&video_modes);
    //DC1394_ERR_CLN_RTN(err,cleanup_and_exit(camera),"Can't get video modes");

    //select video mode
    video_mode=DC1394_VIDEO_MODE_FORMAT7_0;

    //select color coding for selected mode
    err=dc1394_get_color_coding_from_video_mode(camera, video_mode,&coding);
    DC1394_ERR_CLN_RTN(err,cleanup_and_exit(camera),"Could not get color coding");

    //setup capture iso speed
    err=dc1394_video_set_iso_speed(camera, DC1394_ISO_SPEED_400);
    DC1394_ERR_CLN_RTN(err,cleanup_and_exit(camera),"Could not set iso speed");

    //set mode
    err=dc1394_video_set_mode(camera, video_mode);
    DC1394_ERR_CLN_RTN(err,cleanup_and_exit(camera),"Could not set video mode");

    //set default flags
    err=dc1394_capture_setup(camera,4, DC1394_CAPTURE_FLAGS_DEFAULT);
    DC1394_ERR_CLN_RTN(err,cleanup_and_exit(camera),"Could not setup camera-\nmake sure that the video mode and framerate are\nsupported by your camera");

    dc1394_get_image_size_from_video_mode(camera, video_mode, &width, &height);

    /* Print camera Features

    err=dc1394_feature_get_all(camera,&features);
    if (err!=DC1394_SUCCESS) {
        dc1394_log_warning("Could not get feature set");
    }
    else {
        dc1394_feature_print_all(&features, stdout);
    }
    */
    return success;
}


#endif //PLATFORM_LINUX
