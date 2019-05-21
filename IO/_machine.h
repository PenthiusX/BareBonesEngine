#ifndef _MACHINE_H
#define _MACHINE_H

#include <IO/_hardwareserial.h>
#include <IO/_hwdcamera.h>
#include <IO/_configcontrolentity.h>
#include <QJsonObject>
#include <QMutex>
#include <QTimer>

enum ActionType{
    _DEFAULT=0,
    _CALIBERATION_CONFIG_UPDATE,
    _DEFAULT_FROM_CONFIG,
    _STORE_VALUE_TOGGLE
};


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

    typedef void (_Machine::*CommandFunction)(int,ActionType); // function pointer with (int ,ActionType) parameters

    struct ControlEntity
    {
        _Machine::CommandFunction cmdFunctionPtr;
        _ConfigControlEntity default_config;
        _ConfigControlEntity temp_config;
    };

public:
    _Machine(); //constructor initializes camera and hardware_serial
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
    void initMachine();
    void stageAngleChanged(float angle);

public slots:
    void init();
    void GrabFrame(QString filename);
    void GrabFrame();
    void setImageDir(QString dir);
    QString InfoCmd();
    QString InfoCmd(_HardwareSerial &port);
    QString getMachineVersion();
    void callCommandFunction(QString function_name, int value, ActionType action = _DEFAULT);
    bool saveConfig(QString filename = "configuration.json");

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
    void setApplicationSettings(_ConfigControlEntity *app_sett);

public:
    _HardwareSerial *hardware_serial = nullptr;
    _HWDCamera *camera = nullptr;
    char * colorFrame = nullptr;
    QTimer *timer = nullptr;
    std::map<QString, ControlEntity> function_map;
    QMutex frameUpdateMutex;//acquired by modules when they need to display processed or masked image else it is acquired by machine to show camera image
    bool initialised = false;
    QString json_file_name;
    bool isInitialised() const;
    void ActionTypeLogicHandler(ActionType action,int& val ,int& last_val ,float&config_val ,bool& toggle_state);
    _ConfigControlEntity *application_settings=nullptr;

protected slots:
    void updateFrameGrayscale(char *img, unsigned int iwidth, unsigned int iheight);
    void updateImageCamera();
};

#endif // _MACHINE_H
