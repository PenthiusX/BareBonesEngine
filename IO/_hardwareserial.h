#ifndef _HARDWARESERIAL_H
#define _HARDWARESERIAL_H

#include <QSerialPort>
#include <QObject>
#include <QThread>
#include <IO/_configcontrolentity.h>

/*
 * The HardwareSerial class
 * To create an serial port interface for computer-machine communication
 * Extends the QObject class for slots connections to work
 *  * Created: 21_02_2019
 * Author:Saurabh
*/

class _HardwareSerial : public QThread
{
    Q_OBJECT
public:
    _HardwareSerial();
    _HardwareSerial(QString port_name);//constructor set the settings
    _HardwareSerial(QString port_name, _ConfigControlEntity& serial_config);
    ~_HardwareSerial();

    void openSerialPort();  //initialise serial port with settings
    void closeSerialPort(); //release serial port
    static QStringList list_serial_ports();

    void writeData(const QString data); //non blocking data write
    QString writeDataAndWait(const QString data); //write data and wait for responce and return responce
    void waitAndWriteData(const QString data); //wait until last command gives acknowledgement and then write

    void run();

    struct Settings { //struct to save defaults settings
        QString name;
        qint32 baudRate;
        QString stringBaudRate;
        QSerialPort::DataBits dataBits;
        QString stringDataBits;
        QSerialPort::Parity parity;
        QString stringParity;
        QSerialPort::StopBits stopBits;
        QString stringStopBits;
        QSerialPort::FlowControl flowControl;
        QString stringFlowControl;
        qint64 readBufferSize;
        int timeout;
        bool localEchoEnabled;
    } settings;

public slots:
    void readData(); //slot to read data when receive buffer is non-empty

private:
    bool virtual_serial= false;
    QSerialPort *m_serial = nullptr; //QSerial object
    //QThread thread;
};

#endif // _HARDWARESERIAL_H
