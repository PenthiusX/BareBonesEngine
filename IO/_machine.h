#ifndef _MACHINE_H
#define _MACHINE_H

#include <IO/_hardwareserial.h>
#include <IO/_hwdcamera.h>
#include <QJsonObject>

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
public:
    _Machine(); //constructor initializes camera and hradware_serial
    _Machine(QString json_file);
    ~_Machine();

    void set_hardware_serial_defaults(); //setup commands
    void set_camera();
    void set_hardware_serial();
    void set_hardware_serial(QJsonObject serial_config);

signals:
    void serialReturned(QString responce);
    void cameraFrameReturned(char *img,unsigned int w,unsigned h);

public slots:
    void init();
    void TurnTableMotorDiff(int steps);  //Rotate Stage Motor by specified steps (differential input)
    void LaserFocusMotorDiff(int steps); //Slide Marking Laser Focus Motor by specified steps (differential input)
    void LaserHeightMotorDiff(int steps);//Slide Marking Laser Height Motor by specified steps (differential input)
    void MarkingLaserOut(int state);     //Marking Laser Shoot on(state=1) or off(state=0)
    void MarkingLaserDiode(int state);   //Marking Laser Diode on(state=1) or off(state=0)
    void Vaccum(int state);
    void MarkingLaser(float intensity);
    void LineLaser(int intensity);
    void BackLight(int intensity);
    void GrabFrame(QString filename);
    void set_image_dir(QString dir);

public:
    _HardwareSerial *hardware_serial = nullptr;
    _HWDCamera *camera = nullptr;
    QJsonObject config;
};

#endif // _MACHINE_H
