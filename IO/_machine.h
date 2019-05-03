#ifndef _MACHINE_H
#define _MACHINE_H

#include <IO/_hardwareserial.h>
#include <IO/_hwdcamera.h>
#include <IO/_configcontrolentity.h>
#include <QJsonObject>
#include <QMutex>
#include <QTimer>

/*
 * The Machine class
 * To create an interface for computer-machine communication
 *
 *  contains these main interfaces :
 *    Camera : Camera object based on type(dc1394,avt,analog,others) and inherted from _Camera class
 *    HardwareSerial : HardwareSerial(serial rs232) object for commands interface between machine and computer
 *
 * Created: 21_02_2019
 * Author:Saurabh
*/
class _Machine :public QObject
{
    Q_OBJECT

    enum ActionType{
        _DEFAULT=0,
        _CALIBERATION_CONFIG_UPDATE,
        _DEFAULT_FROM_CONFIG,
        _STORE_VALUE_TOGGLE
    };

    typedef void (_Machine::*CommandFunction)(int,ActionType); // function pointer with (int ,ActionType) parameters

public:
    _Machine(); //constructor initializes camera and hradware_serial
    _Machine(QString json_file);
    ~_Machine();

    void set_hardware_serial_defaults(); //setup commands
    void set_camera(_ConfigControlEntity &camera_config);
    void set_hardware_serial(_ConfigControlEntity &hardware_config);
    void set_function_map();

    _ConfigControlEntity config;


signals:
    void serialReturned(QString responce);
    void guiFrameOut(char *img,unsigned int w,unsigned h);//goes from machine to ui(gl_widget) for displaying
    void cameraFrameRecieved(char *img,unsigned int w,unsigned h);

public slots:
    void init();
    void GrabFrame(QString filename);
    void GrabFrame();
    void set_image_dir(QString dir);
    QString InfoCmd();
    QString InfoCmd(_HardwareSerial &port);
    QString getMachineVersion();
    void callCommandFunction(QString function_name, int value, ActionType action = _DEFAULT);

    //machine control commands
    void TurnTableMotorDiff(  int steps,ActionType action = _DEFAULT); //Rotate Stage Motor by specified steps (differential input)
    void LaserFocusMotorDiff( int steps,ActionType action = _DEFAULT); //Slide Marking Laser Focus Motor by specified steps (differential input)
    void LaserHeightMotorDiff(int steps,ActionType action = _DEFAULT); //Slide Marking Laser Height Motor by specified steps (differential input)
    void MarkingLaserOut(     int state,ActionType action = _DEFAULT); //Marking Laser Shoot on(state=1) or off(state=0)
    void MarkingLaserDiode(   int state,ActionType action = _DEFAULT); //Marking Laser Diode on(state=1) or off(state=0)
    void Vaccum(              int state,ActionType action = _DEFAULT);
    void MarkingLaser(        int intensity,ActionType action = _DEFAULT);
    void LineLaser(           int intensity,ActionType action = _DEFAULT);
    void BackLight(           int intensity,ActionType action = _DEFAULT);
    void setBrightness(       int value,ActionType action = _DEFAULT);
    void setGain(             int value,ActionType action = _DEFAULT);
    void setExposure(         int value,ActionType action = _DEFAULT);
    void setContrast(         int value,ActionType action = _DEFAULT);
    void setOffset(           int value,ActionType action = _DEFAULT);

    void updateFrameColor(char *img, unsigned int iwidth, unsigned int iheight);

public:
    _HardwareSerial *hardware_serial = nullptr;
    _HWDCamera *camera = nullptr;
    char * colorFrame = nullptr;
    QTimer *timer = nullptr;
    std::map<QString, _Machine::CommandFunction> function_map;
    QMutex frameUpdateMutex;//acquired by modules when they need to display processed or masked image else it is acquired by machine to show camera image

protected slots:
    void updateFrameGrayscale(char *img, unsigned int iwidth, unsigned int iheight);
    void updateImageCamera();
};

#endif // _MACHINE_H
