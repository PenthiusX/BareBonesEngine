#include "_hardwareserial.h"
#include <QDebug>
#include <QSerialPortInfo>
#include <vector>

/*
 * The HardwareSerial class
 * To create an serial port interface for computer-machine communication
 *
 *    ** Yet to create non blocking preemptive object for threaded operation **
 *
 * Extends the QObject class for slots connections to work
 * Created: 21_02_2019
 * Author:Saurabh
*/

/* Constructor for the HardwareSerial class
 * set the default settings for serial port
 * Created: 21_02_2019
 */
_HardwareSerial::_HardwareSerial()
{

    virtual_serial = true;

    settings.baudRate = QSerialPort::Baud115200;
    settings.dataBits = QSerialPort::Data8;
    settings.parity = QSerialPort::NoParity;
    settings.stopBits = QSerialPort::OneStop;
    settings.flowControl = QSerialPort::NoFlowControl;
    settings.timeout = 1000;
}

_HardwareSerial::_HardwareSerial(QString port_name)
{
    settings.name = port_name;
    settings.baudRate = QSerialPort::Baud115200;
    settings.dataBits = QSerialPort::Data8;
    settings.parity = QSerialPort::NoParity;
    settings.stopBits = QSerialPort::OneStop;
    settings.flowControl = QSerialPort::NoFlowControl;
    settings.timeout = 1000;
}

/* Destructor
 * closes serial port when object is deleted
 * Created: 21_02_2019
 * */
_HardwareSerial::~_HardwareSerial()
{
    if(!virtual_serial)
        closeSerialPort();
}

/* initialize QSerial object with default setttings
 * Created: 21_02_2019
 * */
void _HardwareSerial::openSerialPort()
{
    m_serial = new QSerialPort();

    if(!virtual_serial){
    //default settings
    m_serial->setPortName(settings.name);
    m_serial->setBaudRate(settings.baudRate);
    m_serial->setDataBits(settings.dataBits);
    m_serial->setParity(settings.parity);
    m_serial->setStopBits(settings.stopBits);
    m_serial->setFlowControl(settings.flowControl);

    //check if serial opened successfully
    if (m_serial->open(QIODevice::ReadWrite)) {
        qDebug() << "Serial port opened";
    } else {
        qDebug() << "Serial port could not be opened: " << m_serial->errorString();
    }
}
    //setup slot connection for reading recieved data
    //run _HardwareSerial::readData() function when QSerialPort::readyRead() signal is recieved
    //connect(m_serial, &QSerialPort::readyRead, this, &_HardwareSerial::readData);
}

/* releases QSerial object and closes connection
 * Created: 21_02_2019
 * */
void _HardwareSerial::closeSerialPort()
{
    if (m_serial->isOpen())
        m_serial->close();
    qDebug() << "Disconnected";
}
QStringList _HardwareSerial::list_serial_ports()
{
    QStringList list;
    const auto infos = QSerialPortInfo::availablePorts();

    for (const QSerialPortInfo &info : infos) {
        list << info.portName();
    }
    return list;
}

/* non blocking/asychronous data write
 * returns immediately after writing data to send buffer
 * Created: 21_02_2019
 * */
void _HardwareSerial::writeData(const QString data)
{

    QByteArray d = data.toLocal8Bit();
    d.append(QByteArray::fromHex("0A"));

    if(!virtual_serial){
        m_serial->write(d);
        qDebug() << "written" << d << QThread::currentThread();
    }
    else {
         qDebug() << "written virtually" << d << QThread::currentThread();
    }


}

/* blocking/sychronous data write and wait until responce is recieved
 * returns responce as QString after recieving by reading recieve buffer
 * Created: 21_02_2019
 * */
QString _HardwareSerial::writeDataAndWait(const QString data)
{
    if(!virtual_serial) {
    QByteArray d = data.toLocal8Bit();
    d.append(QByteArray::fromHex("0A"));
    m_serial->write(d);
    qDebug() << "written" << d << QThread::currentThread();
    if(m_serial->waitForReadyRead(settings.timeout)){
        QByteArray responseData = m_serial->readAll();
        while (m_serial->waitForReadyRead(10))
            responseData += m_serial->readAll();

        const QString response = QString::fromUtf8(responseData);
        qDebug() << response;
        return response;
    }
    else {
        qDebug() << QString("Wait read response timeout %1").arg(1);
        const QString response;
        return response;
    }
    }
    else {
        qDebug() << "written virtually" << data << QThread::currentThread();
        return "=\n";
    }
}

/*  *** Yet to implement correctly ***
 * blocking/sychronous wait until last responce is recieved
 * write data after waiting is done
 * Created: 21_02_2019
 * */
void _HardwareSerial::waitAndWriteData(const QString data)
{
    writeDataAndWait(data);
}

void _HardwareSerial::run()
{
    qDebug() << "runnung" ;//<<exec();
}

/* reads data when signal to read is triggered
 * Created: 21_02_2019
*/
void _HardwareSerial::readData()
{
    if(!virtual_serial) {const QByteArray data = m_serial->readAll();
    qDebug() << data;
    }
}
