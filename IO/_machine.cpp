#include "_machine.h"
#include <QDebug>
#include <QTimer>
#include <QThread>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <_tools.h>
#include <IO/_avt_camera.h>
#include <IO/_dc_1394_camera.h>


/*
 * The Machine class
 * To create an interface for computer-machine communication
 *
 *  contains these main interfaces :
 *    Camera : Camera object based on type(dc1394,analog,others) and inherted from _Camera class
 *    HardwareSerial : HardwareSerial(serial rs232) object for commands interface between machine and computer
 *
 * Created: 21_02_2019
 * Author:Saurabh
*/

/* Constructor for the Machine class
 * list and select serial ports connected to machine
 * list and select camera connected to machine
 * Created: 21_02_2019
 */
_Machine::_Machine()
{

}

/* Constructor for the Machine class
 * Read all defualts and configuration from given json file
 *
 * list and select serial ports connected to machine
 * list and select camera connected to machine
 * Created: 27_03_2019
 */
_Machine::_Machine(QString json_file) : config(_Tools::ReadJsonFromQrc(json_file))
{

}

// ------------ command functions ----------

/* commands are sent as Strings with command dependent prefixes,suffixes and numeric values if required
 * for example for stage motor has to be rotated by 100 steps the String command will be
 * command : XM100G
 * format :XM<steps>G
 *  XM : operation specific preffix
 *  100(<steps>) : numeric value
 *  G : operation specific suffix
 * similar to G-codes in CNC or 3D printer
 * command format: <val> specifies numerical value val replaced in the command string
 *
 * */

/* Rotate Stage Motor by specified steps
 * differential steps from current position
 * number of steps for full rotation: 16000
 * Created: 21_02_2019
 * */
void _Machine::TurnTableMotorDiff(int steps)
{
    //command : XM<steps>G
    hardware_serial->waitAndWriteData(QString("XM%1G").arg(steps));
}

/* Slide Marking Laser Focus Motor by specified steps
 * differential steps from current position
 * mm per step : unknown
 * Created: 21_02_2019
 * */
void _Machine::LaserFocusMotorDiff(int steps)
{
    //command : FM<steps>G
    hardware_serial->waitAndWriteData(QString("FM%1G").arg(steps));
}

/* Slide Marking Laser Height Motor by specified steps
 * differential steps from current position
 * mm per step : unknown
 * Created: 21_02_2019
 * */
void _Machine::LaserHeightMotorDiff(int steps)
{
    //command : ZM<steps>G
    hardware_serial->waitAndWriteData(QString("ZM%1G").arg(steps));
}


/* Set Line Laser intensity and switch on/off
 * zero intensity specifies off condition
 * intensity varies from 0 to 255
 * */
void _Machine::Vaccum(int state)
{
    //command for on:Q
    //command for off:R
    hardware_serial->waitAndWriteData((state) ? "N":"Y");
}

/* Set Line Laser intensity and switch on/off
 * zero intensity specifies off condition
 * intensity varies from 0 to 255
 * */
void _Machine::LineLaser(int intensity)
{
    hardware_serial->waitAndWriteData(QString("L%1").arg(intensity));
}

/* Set BackLight intensity and switch on/off
 * zero intensity specifies off condition
 * intensity varies from 0 to 255
 * */
void _Machine::BackLight(int intensity)
{
    hardware_serial->waitAndWriteData(QString("O%1").arg(intensity));
}

void _Machine::GrabFrame(QString filename)
{
    camera->grab_frame(filename);
    emit cameraFrameReturned(camera->get_frame(),1360,1024);
}

/* Set Marking Laser intensity and switch on/off
 * @ - for machine intensity starts from zero but for command intensity
 * recieved from 1 above hence intensity is subtracted by 1
 * zero intensity specifies off condition
 * */
void _Machine::MarkingLaser(float intensity)
{
    if (intensity > 0)
    {
        // command : E<intensity>
        hardware_serial->waitAndWriteData(QString("E%1").arg(intensity-1));//set the intesity @
        MarkingLaserDiode(1);//switch on the laser diode
        QThread::sleep(5);
        MarkingLaserOut(1);
        //QTimer::singleShot(3000,[this]() {MarkingLaserOut(1);});//switch on the laser out after 2 seconds
    }
    else {
        MarkingLaserOut(0);//switch off the laser out
        QThread::sleep(1);
        MarkingLaserDiode(0);
        //QTimer::singleShot(1000,[this]() {MarkingLaserDiode(0);});//switch off the laser diode after 2 seconds
    }
}

/* Switch Marking Laser Out on or off
 * on | state = 1
 * off | state = 0
 * */
void _Machine::MarkingLaserOut(int state)
{
    //command for on:Q
    //command for off:R
    hardware_serial->waitAndWriteData((state) ? "Q":"R");
}

/* Switch Marking Laser Diode on or off
 * on | state = 1
 * off | state = 0
 * */
void _Machine::MarkingLaserDiode(int state)
{
    //command for on:V
    //command for off:W
    hardware_serial->waitAndWriteData((state) ? "V":"W");
}

// ------------/command functions ----------



/* setup cammands
 * Created: 21_02_2019
 * */
void _Machine::set_hardware_serial_defaults()
{
    hardware_serial->writeDataAndWait("?");
    hardware_serial->writeDataAndWait("?");
    hardware_serial->writeDataAndWait("XH8000B4000A32000M400D15");
    hardware_serial->writeDataAndWait("T");
    hardware_serial->writeDataAndWait("N");
}


/* Select Camera object depending on Platform
 * and camera type
 * Fallback to default virtual Camera if
 * no camera found or failed to initialize
 *
 * ** required update: This Function Should set Camera based on configuration file afterwards**
 *
 * */
void _Machine::set_camera()
{

    //QJsonObject camera_config = config["Camera"].toObject();

    camera = new _HWDCamera();

#ifdef PLATFORM_LINUX
    camera = new _DC_1394_Camera();

    if(camera->list_cameras()==0)
    {
        //base camera if no dc1394 camera found
        camera = new _Camera();
    }
#endif //PLATFORM_LINUX

#ifdef PLATFORM_WIN

    //
    camera = new _AVT_Camera();

    if(camera->init(0)==0)
    {
        //base camera if no special camera found
        camera = new _HWDCamera();
    }

    /* camera->init(0) function of camera will reopen camera module hence it
     * should be stopped first else thows already opened error
     * */
    camera->stop_camera();

#endif //PLATFORM_WIN
}

/* Function : set_hardware_serial()
 * this function finds the hardware serial port which is connected to machine
 *
*/
void _Machine::set_hardware_serial()
{
    //list serial ports and check if returns machine
    QStringList ports = _HardwareSerial::list_serial_ports();

    QStringList machine_ports;

    for (int i = 0; i < ports.size(); ++i){

        _HardwareSerial port(ports.at(i));

        port.openSerialPort();

        port.writeDataAndWait("?");

        //if acknowledgement("=" character) recieved then machine is connected
        if(port.writeDataAndWait("?")=="=\n")
            machine_ports << ports.at(i);
    }
    switch (machine_ports.size())
    {
        case 0: // if no Serial ports found;
            qDebug() << "machine not detected functionality will be limited";
            hardware_serial = new _HardwareSerial();
            break;
        case 1: //if one machine found;
            hardware_serial = new _HardwareSerial(machine_ports.at(0));
            break;
        default:
            qDebug() << "more than one machine detected select one";
            // code to be executed if more than one machine found
    }
}
/* Function : set_hardware_serial(QJsonObject hardware_config)
 * this function finds the machine connected to computer
 * and sets the defualts as given in the config
*/
void _Machine::set_hardware_serial(QJsonObject hardware_config)
{
    //send the nested config objects to necessary hardware objects (should be used by nested objects afterwards)
    QJsonObject serial_config = hardware_config["Communication"].toObject()["RS232"].toObject();
    QJsonObject command_config = hardware_config["Commands"].toObject();


    //list serial ports and check if returns machine

    QStringList ports = _HardwareSerial::list_serial_ports();

    QStringList machine_ports;

    for (int i = 0; i < ports.size(); ++i){

        _HardwareSerial port(ports.at(i));

        port.openSerialPort();

        port.writeDataAndWait("?");

        //if acknowledgement("=" character) recieved then machine is connected
        if(port.writeDataAndWait("?")=="=\n")
            machine_ports << ports.at(i);
    }
    switch (machine_ports.size())
    {
        case 0: // if no Serial ports found;
            qDebug() << "machine not detected functionality will be limited";
            hardware_serial = new _HardwareSerial();
            break;
        case 1: //if one machine found;
            hardware_serial = new _HardwareSerial(machine_ports.at(0));
            break;
        default:
            qDebug() << "more than one machine detected select one";
            // code to be executed if more than one machine found
    }
}

/*
 * Function : init()
 * should be called when thread starts
 * sets the hardware serial and camera objects according to configuration
 * sets defaults for hardware interation
*/
void _Machine::init()
{
    set_hardware_serial();

    hardware_serial->openSerialPort();

    //list cameras and select by properties here-

    // init selected camera - currently hardcoded

    set_camera();

    camera->init(0);

    camera->get_frame();

    //setup commands
    set_hardware_serial_defaults();
}

/* Function : set_image_dir(QString dir)
 * this function sets folder to
 * save / load images when connected to hardware camera / no camera found respectively
*/
void _Machine::set_image_dir(QString dir)
{
    camera->set_image_dir(dir);
}
