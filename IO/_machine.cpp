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
#include <IO/_picolo_camera.h>

#define NEWLINE "\n"
#define NULL_STR ""



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
    //mandatory
    set_function_map();
}

/* Constructor for the Machine class
 * Read all defualts and configuration from given json file
 *
 * list and select serial ports connected to machine
 * list and select camera connected to machine
 * Created: 27_03_2019
 */
_Machine::_Machine(QString json_file) : config(_ConfigControlEntity(_Tools::ReadJsonFromQrc(json_file)))
{
    json_file_name=json_file;
    //mandatory
    set_function_map();

}

_Machine::~_Machine()
{
    delete camera;
    delete hardware_serial;
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
void _Machine::TurnTableMotorDiff(int steps,ActionType action)
{
    static int last_steps = steps;
    switch (action) {
    case _CALIBERATION_CONFIG_UPDATE:
    {
        //save steps to config
        config["Hardware"]["Controls"]["StageMotor"]["Data"]["Caliberation"].getFloatEntity("VALUE")=steps;
        break;
    }
    case _DEFAULT_FROM_CONFIG:
    {
        steps = config["Hardware"]["Controls"]["StageMotor"]["Data"]["Caliberation"].getFloatEntity("VALUE");
        break;
    }
    }
    //command : XM<steps>G
    static QString prefix = config["Hardware"]["Controls"]["StageMotor"]["Commands"]["RS232"].getStringEntity("SET")+config["Hardware"]["Controls"]["SetDistance"]["Commands"]["RS232"].getStringEntity("SET");
    static QString postfix = config["Hardware"]["Controls"]["MoveAcceleration"]["Commands"]["RS232"].getStringEntity("SET");
    hardware_serial->waitAndWriteData(QString(prefix+"%1"+postfix).arg(steps));
}

/* Slide Marking Laser Focus Motor by specified steps
 * differential steps from current position
 * mm per step : unknown
 * Created: 21_02_2019
 * */
void _Machine::LaserFocusMotorDiff(int steps,ActionType action)
{
    static int last_steps = steps;
    switch (action) {
    case _CALIBERATION_CONFIG_UPDATE:
    {
        //save steps to config
        config["Hardware"]["Controls"]["LaserFocusMotor"]["Data"]["Caliberation"].getFloatEntity("VALUE")=steps;
        break;
    }
    case _DEFAULT_FROM_CONFIG:
    {
        steps = config["Hardware"]["Controls"]["LaserFocusMotor"]["Data"]["Caliberation"].getFloatEntity("VALUE");
        break;
    }
    }
    //command : FM<steps>G
    static QString prefix = config["Hardware"]["Controls"]["LaserFocusMotor"]["Commands"]["RS232"].getStringEntity("SET")+config["Hardware"]["Controls"]["SetDistance"]["Commands"]["RS232"].getStringEntity("SET");
    static QString postfix = config["Hardware"]["Controls"]["MoveAcceleration"]["Commands"]["RS232"].getStringEntity("SET");
    hardware_serial->waitAndWriteData(QString(prefix+"%1"+postfix).arg(steps));
}

/* Slide Marking Laser Height Motor by specified steps
 * differential steps from current position
 * mm per step : unknown
 * Created: 21_02_2019
 * */
void _Machine::LaserHeightMotorDiff(int steps,ActionType action)
{
    static int last_steps = steps;
    switch (action) {
    case _CALIBERATION_CONFIG_UPDATE:
    {
        //save steps to config
        config["Hardware"]["Controls"]["LaserHeightMotor"]["Data"]["Caliberation"].getFloatEntity("VALUE")=steps;
        break;
    }
    case _DEFAULT_FROM_CONFIG:
    {
        steps = config["Hardware"]["Controls"]["LaserHeightMotor"]["Data"]["Caliberation"].getFloatEntity("VALUE");
        break;
    }
    }
    //command : ZM<steps>G
    static QString prefix = config["Hardware"]["Controls"]["LaserHeightMotor"]["Commands"]["RS232"].getStringEntity("SET")+config["Hardware"]["Controls"]["SetDistance"]["Commands"]["RS232"].getStringEntity("SET");
    static QString postfix = config["Hardware"]["Controls"]["MoveAcceleration"]["Commands"]["RS232"].getStringEntity("SET");
    hardware_serial->waitAndWriteData(QString(prefix+"%1"+postfix).arg(steps));
}


/* Set Line Laser intensity and switch on/off
 * zero intensity specifies off condition
 * intensity varies from 0 to 255
 * */
void _Machine::Vaccum(int state,ActionType action)
{
    static int last_state = state;
    switch (action) {
    case _CALIBERATION_CONFIG_UPDATE:
    {
        //save state to config
        config["Hardware"]["Controls"]["Vaccum"]["Data"]["Caliberation"].getFloatEntity("VALUE")=state;
        break;
    }
    case _DEFAULT_FROM_CONFIG:
    {
        state = config["Hardware"]["Controls"]["Vaccum"]["Data"]["Caliberation"].getFloatEntity("VALUE");
        break;
    }
    case _STORE_VALUE_TOGGLE:
    {
        //exchange variables state without temporary variables
        state = state + last_state;
        last_state = state - last_state;
        state = state - last_state;
        break;
    }
    }
    //command for on:N
    //command for off:Y
    static QString on = config["Hardware"]["Controls"]["Vaccum"]["Commands"]["RS232"].getStringEntity("ON");
    static QString off = config["Hardware"]["Controls"]["Vaccum"]["Commands"]["RS232"].getStringEntity("OFF");
    hardware_serial->waitAndWriteData((state) ? on:off);
}

/* Set Line Laser intensity and switch on/off
 * zero intensity specifies off condition
 * intensity varies from 0 to 255
 * */
void _Machine::LineLaser(int intensity,ActionType action)
{
    static int last_value = intensity;
    switch (action) {
    case _CALIBERATION_CONFIG_UPDATE:
    {
        //save value to config
        config["Hardware"]["Controls"]["LineLaser"]["Data"]["Caliberation"].getFloatEntity("VALUE")=intensity;
        break;
    }
    case _DEFAULT_FROM_CONFIG:
    {
        intensity = config["Hardware"]["Controls"]["LineLaser"]["Data"]["Caliberation"].getFloatEntity("VALUE");
        break;
    }
    case _STORE_VALUE_TOGGLE:
    {
        //exchange variables value without temporary variables
        intensity = intensity + last_value;
        last_value = intensity - last_value;
        intensity = intensity - last_value;
        break;
    }
    }
    static QString cmd = config["Hardware"]["Controls"]["LineLaser"]["Commands"]["RS232"].getStringEntity("SET");
    hardware_serial->waitAndWriteData(QString(cmd+"%1").arg(intensity));
}

/* Set BackLight intensity and switch on/off
 * zero intensity specifies off condition
 * intensity varies from 0 to 255
 * */
void _Machine::BackLight(int intensity,ActionType action)
{
    static int last_value = intensity;
    switch (action) {
    case _CALIBERATION_CONFIG_UPDATE:
    {
        //save value to config
        config["Hardware"]["Controls"]["BackLight"]["Data"]["Caliberation"].getFloatEntity("VALUE")=intensity;
        break;
    }
    case _DEFAULT_FROM_CONFIG:
    {
        intensity = config["Hardware"]["Controls"]["BackLight"]["Data"]["Caliberation"].getFloatEntity("VALUE");
        break;
    }
    case _STORE_VALUE_TOGGLE:
    {
        //exchange variables value without temporary variables
        intensity = intensity + last_value;
        last_value = intensity - last_value;
        intensity = intensity - last_value;
        break;
    }
    }

    static QString cmd = config["Hardware"]["Controls"]["BackLight"]["Commands"]["RS232"].getStringEntity("SET");

    //write value to serial
    hardware_serial->waitAndWriteData(QString(cmd+"%1").arg(intensity));
}


/* Set Marking Laser intensity and switch on/off
 * @ - for machine intensity starts from zero but for command intensity
 * recieved from 1 above hence intensity is subtracted by 1
 * zero intensity specifies off condition
 * */
void _Machine::MarkingLaser(int intensity,ActionType action)
{
    static int last_value = intensity;
    switch (action) {
    case _CALIBERATION_CONFIG_UPDATE:
    {
        //save value to config
        config["Hardware"]["Controls"]["MarkingLaser"]["Data"]["Caliberation"].getFloatEntity("VALUE")=intensity;
        break;
    }
    case _DEFAULT_FROM_CONFIG:
    {
        intensity = config["Hardware"]["Controls"]["MarkingLaser"]["Data"]["Caliberation"].getFloatEntity("VALUE");
        break;
    }
    case _STORE_VALUE_TOGGLE:
    {
        //exchange variables value without temporary variables
        intensity = intensity + last_value;
        last_value = intensity - last_value;
        intensity = intensity - last_value;
        break;
    }
    }

    static QString laser_power_cmd = config["Hardware"]["Controls"]["MarkingLaser"]["Commands"]["RS232"].getStringEntity("SET");
    if (intensity > 0)
    {
        // command : E<intensity>

        hardware_serial->waitAndWriteData(QString(laser_power_cmd+"%1").arg(intensity-1));//set the intesity @
        MarkingLaserDiode(1); //switch on the laser diode
        QThread::sleep(5);    //wait 5 seconds
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

/* Function :getMachineType()
 * "?"
 * created: 12_04_2019
 * */
QString _Machine::getMachineVersion()
{
    //command : "?"
    static QString info_cmd = config["Hardware"]["Controls"]["GetVersion"]["Commands"]["RS232"].getStringEntity("SET");
    return hardware_serial->writeDataAndWait(info_cmd);
}

void _Machine::setBrightness(int value,ActionType action)
{
    static int last_value = value;
    switch (action) {
    case _CALIBERATION_CONFIG_UPDATE:
    {
        //save value to config
        config["Camera"]["Brightness"]["Data"]["Caliberation"].getFloatEntity("VALUE")=value;
        break;
    }
    case _DEFAULT_FROM_CONFIG:
    {
        value = config["Camera"]["Brightness"]["Data"]["Caliberation"].getFloatEntity("VALUE");
        break;
    }
    case _STORE_VALUE_TOGGLE:
    {
        //exchange variables value without temporary variables
        value = value + last_value;
        last_value = value - last_value;
        value = value - last_value;
        break;
    }
    }
    camera->setBrightness(value);
}

void _Machine::setGain(int value,ActionType action)
{
    static int last_value = value;
    switch (action) {
    case _CALIBERATION_CONFIG_UPDATE:
    {
        //save value to config
        config["Camera"]["Gain"]["Data"]["Caliberation"].getFloatEntity("VALUE")=value;
        break;
    }
    case _DEFAULT_FROM_CONFIG:
    {
        value = config["Camera"]["Gain"]["Data"]["Caliberation"].getFloatEntity("VALUE");
        break;
    }
    case _STORE_VALUE_TOGGLE:
    {
        //exchange variables value without temporary variables
        value = value + last_value;
        last_value = value - last_value;
        value = value - last_value;
        break;
    }
    }
    camera->setGain(value);
}

void _Machine::setExposure(int value,ActionType action)
{
    static int last_value = value;
    switch (action) {
    case _CALIBERATION_CONFIG_UPDATE:
    {
        //save value to config
        config["Camera"]["Exposure"]["Data"]["Caliberation"].getFloatEntity("VALUE")=value;
        break;
    }
    case _DEFAULT_FROM_CONFIG:
    {
        value = config["Camera"]["Exposure"]["Data"]["Caliberation"].getFloatEntity("VALUE");
        break;
    }
    case _STORE_VALUE_TOGGLE:
    {
        //exchange variables value without temporary variables
        value = value + last_value;
        last_value = value - last_value;
        value = value - last_value;
        break;
    }
    }
    camera->setExposure(value);
}

void _Machine::setContrast(int value,ActionType action)
{
    static int last_value = value;
    switch (action) {
    case _CALIBERATION_CONFIG_UPDATE:
    {
        //save value to config
        config["Camera"]["Contrast"]["Data"]["Caliberation"].getFloatEntity("VALUE")=value;
        break;
    }
    case _DEFAULT_FROM_CONFIG:
    {
        value = config["Camera"]["Contrast"]["Data"]["Caliberation"].getFloatEntity("VALUE");
        break;
    }
    case _STORE_VALUE_TOGGLE:
    {
        //exchange variables value without temporary variables
        value = value + last_value;
        last_value = value - last_value;
        value = value - last_value;
        break;
    }
    }
    camera->setContrast(value);
}

void _Machine::setOffset(int value,ActionType action)
{
    static int last_value = value;
    switch (action) {
    case _CALIBERATION_CONFIG_UPDATE:
    {
        //save value to config
        config["Camera"]["Offset"]["Data"]["Caliberation"].getFloatEntity("VALUE")=value;
        break;
    }
    case _DEFAULT_FROM_CONFIG:
    {
        value = config["Camera"]["Offset"]["Data"]["Caliberation"].getFloatEntity("VALUE");
        break;
    }
    case _STORE_VALUE_TOGGLE:
    {
        //exchange variables value without temporary variables
        value = value + last_value;
        last_value = value - last_value;
        value = value - last_value;
        break;
    }
    }
    camera->setOffset(value);
}


/* Function :InfoCmd()
 * Sends the info command
 * "?"
 * created: 12_04_2019
 * */
QString _Machine::InfoCmd()
{
    //command : "?"
    static QString info_cmd = config["Hardware"]["Controls"]["Info"]["Commands"]["RS232"].getStringEntity("SET");
    return hardware_serial->writeDataAndWait(info_cmd);
}

/* Function : InfoCmd(_HardwareSerial& port)
 * Sends the info command on the given port
 * "?"
 * created: 12_04_2019
 * */
QString _Machine::InfoCmd(_HardwareSerial& port)
{

    //command : "?"
    static QString info_cmd = config["Hardware"]["Controls"]["Info"]["Commands"]["RS232"].getStringEntity("SET");
    return port.writeDataAndWait(info_cmd);
}

/* Switch Marking Laser Out on or off
 * on | state = 1
 * off | state = 0
 * */
void _Machine::MarkingLaserOut(int state,ActionType action)
{
    static int last_state = state;
    switch (action) {
    case _STORE_VALUE_TOGGLE:
    {
        //exchange variables state without temporary variables
        state = state + last_state;
        last_state = state - last_state;
        state = state - last_state;
        break;
    }
    }
    //command for on:Q
    //command for off:R
    static QString on = config["Hardware"]["Controls"]["MarkingLaser"]["Commands"]["RS232"].getStringEntity("ON");
    static QString off = config["Hardware"]["Controls"]["MarkingLaser"]["Commands"]["RS232"].getStringEntity("OFF");
    hardware_serial->waitAndWriteData((state) ? on:off);
}

/* Switch Marking Laser Diode on or off
 * on | state = 1
 * off | state = 0
 * */
void _Machine::MarkingLaserDiode(int state,ActionType action)
{
    static int last_state = state;
    switch (action) {
    case _CALIBERATION_CONFIG_UPDATE:
    {
        //save state to config
        config["Hardware"]["Controls"]["MarkingLaserDiode"]["Data"]["Caliberation"].getFloatEntity("VALUE")=state;
        break;
    }
    case _DEFAULT_FROM_CONFIG:
    {
        state = config["Hardware"]["Controls"]["MarkingLaserDiode"]["Data"]["Caliberation"].getFloatEntity("VALUE");
        break;
    }
    case _STORE_VALUE_TOGGLE:
    {
        //exchange variables state without temporary variables
        state = state + last_state;
        last_state = state - last_state;
        state = state - last_state;
        break;
    }
    }
    //command for on:V
    //command for off:W
    static QString on = config["Hardware"]["Controls"]["MarkingLaserDiode"]["Commands"]["RS232"].getStringEntity("ON");
    static QString off = config["Hardware"]["Controls"]["MarkingLaserDiode"]["Commands"]["RS232"].getStringEntity("OFF");
    hardware_serial->waitAndWriteData((state) ? on:off);
}

// ------------/command functions ----------



/* setup cammands
 * Created: 21_02_2019
 * */
void _Machine::set_hardware_serial_defaults()
{
    InfoCmd();
    InfoCmd();
    hardware_serial->writeDataAndWait("XH8000B4000A32000M400D15");
    getMachineVersion();
    Vaccum(1);

}


/* Select Camera object depending on Platform
 * and camera type
 * Fallback to default virtual Camera if
 * no camera found or failed to initialize
 *
 * ** required update: This Function Should set Camera based on configuration file afterwards**
 *
 * */
void _Machine::set_camera(_ConfigControlEntity& camera_config)
{

    //QJsonObject camera_config = config["Camera"].toObject();

    camera = new _HWDCamera();

#ifdef PLATFORM_LINUX
    delete camera;
    camera = new _DC_1394_Camera();

    if(camera->list_cameras()==0)
    {
        //base camera if no dc1394 camera found
        camera = new _HWDCamera();
    }
#endif //PLATFORM_LINUX

#ifdef PLATFORM_WIN

    //
    delete camera;
    camera = new _AVT_Camera();

    if(camera->init(0)==0)
    {
        //base camera if no special camera found
        camera = new _HWDCamera();
    }

    delete camera;
    camera = new _Picolo_Camera();

    if(camera->list_cameras()==0)
    {
        //base camera if no dc1394 camera found
        camera = new _HWDCamera();
    }

    /* camera->init(0) function of camera will reopen camera module hence it
     * should be stopped first else thows already opened error
     * */
    camera->stop_camera();

#endif //PLATFORM_WIN
}


/* Function : set_hardware_serial(_ConfigControlEntity& hardware_config)
 * this function finds the machine connected to computer
 * and sets the defualts as given in the config
*/
void _Machine::set_hardware_serial(_ConfigControlEntity& hardware_config)
{
    //send the nested config objects to necessary hardware objects (should be used by nested objects afterwards)

    //list serial ports and check if returns machine

    QStringList ports = _HardwareSerial::list_serial_ports();

    static QString ack_cmd = hardware_config["Controls"]["Ack"]["Commands"]["RS232"].getStringEntity("SET")+NEWLINE;

    QStringList machine_ports;

    for (int i = 0; i < ports.size(); ++i){

        _HardwareSerial port(ports.at(i),hardware_config["Communication"]["RS232"]);

        port.openSerialPort();

        InfoCmd(port);//send info command to given port

        //if acknowledgement("=" character) recieved then machine is connected
        if(InfoCmd(port)==ack_cmd)
            machine_ports << ports.at(i);
    }
    switch (machine_ports.size())
    {
        case 0: // if no Serial ports found;
            qDebug() << "machine not detected functionality will be limited";
            hardware_serial = new _HardwareSerial();
            break;
        case 1: //if one machine found;
            hardware_serial = new _HardwareSerial(machine_ports.at(0),hardware_config["Communication"]["RS232"]);
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
    set_hardware_serial(config["Hardware"]);

    hardware_serial->openSerialPort();

    //list cameras and select by properties here-

    // init selected camera - currently hardcoded

    set_camera(config["Camera"]);

    camera->init(0);

    camera->get_frame();

    //setup commands
    set_hardware_serial_defaults();


    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateImageCamera()));
    timer->start(40);

    callCommandFunction("StageMotor",10000);

    initialised = true;

    emit initMachine();
}

void _Machine::callCommandFunction(QString function_name,int value,ActionType action)
{
    //calling function pointer
    (*this.*function_map[function_name].cmdFunctionPtr)(value,action);
}

void _Machine::saveConfig()
{
    _Tools::WriteJsonToQrc("configuration.json",config.toJson());
}

void _Machine::set_function_map()
{
    //assigning function pointers
    function_map["StageMotor"].cmdFunctionPtr = &_Machine::TurnTableMotorDiff;
    function_map["LaserFocusMotor"].cmdFunctionPtr = &_Machine::LaserFocusMotorDiff;
    function_map["LaserHeightMotor"].cmdFunctionPtr = &_Machine::LaserHeightMotorDiff;
    function_map["MarkingLaserOut"].cmdFunctionPtr = &_Machine::MarkingLaserOut;
    function_map["MarkingLaserDiode"].cmdFunctionPtr = &_Machine::MarkingLaserDiode;
    function_map["Vaccum"].cmdFunctionPtr = &_Machine::Vaccum;
    function_map["MarkingLaser"].cmdFunctionPtr = &_Machine::MarkingLaser;
    function_map["LineLaser"].cmdFunctionPtr = &_Machine::LineLaser;
    function_map["BackLight"].cmdFunctionPtr = &_Machine::BackLight;
    function_map["Brightness"].cmdFunctionPtr = &_Machine::setBrightness;
    function_map["Gain"].cmdFunctionPtr = &_Machine::setGain;
    function_map["Exposure"].cmdFunctionPtr = &_Machine::setExposure;
    function_map["Contrast"].cmdFunctionPtr = &_Machine::setContrast;
    function_map["Offset"].cmdFunctionPtr = &_Machine::setOffset;
}

/* Function : set_image_dir(QString dir)
 * this function sets folder to
 * save / load images when connected to hardware camera / no camera found respectively
*/
void _Machine::set_image_dir(QString dir)
{
    camera->set_image_dir(dir);
}

void _Machine::GrabFrame()
{
    camera->grab_frame();
}

void _Machine::updateImageCamera()
{
    if(frameUpdateMutex.tryLock(0))
    {
        //lock is aquired
        camera->grab_frame();
        if(camera->get_frame())
            emit cameraFrameRecieved(camera->get_frame(),camera->getWidth(),camera->getHeight());
        frameUpdateMutex.unlock();
    }
}

void _Machine::GrabFrame(QString filename)
{
    camera->grab_frame(filename);
}

void _Machine::updateFrameGrayscale(char* img,unsigned int iwidth,unsigned int iheight)
{
    for (int index = 0; index < (iwidth*iheight); index++) {
        colorFrame[index*4] = img[index];
        colorFrame[index*4+1] = img[index];
        colorFrame[index*4+2] = img[index];
        colorFrame[index*4+3] = 255;
    }
    emit guiFrameOut(colorFrame,iwidth,iheight);
}

void _Machine::updateFrameColor(char* img,unsigned int iwidth,unsigned int iheight)
{
    emit guiFrameOut(img,iwidth,iheight);
}

bool _Machine::isInitialised() const
{
    return initialised;
}

