#include "_scancaliberationsection.h"
#include "ui__scancaliberationsection.h"
#include <QFileDialog>

_ScanCaliberationSection::_ScanCaliberationSection(QStackedWidget *parent) :
    _CaliberationSection(parent),
    ui(new Ui::_ScanCaliberationSection)
{
    delete layout(); //overwriting layout in setupUi
    ui->setupUi(this);
}

_ScanCaliberationSection::~_ScanCaliberationSection()
{
    delete ui;
}


/* Function : init()
 * this function initialises the default values for machine commands
 * default commands to hardware are also sent
 * when machine is initialised this function is called
 * Created: 09_05_2019
*/
void _ScanCaliberationSection::init()
{
    setupConnections();

    if(machine->isInitialised())
    {
        ui->gain_slider_box->setMinimum(machine->config["Camera"]["Gain"]["Data"]["Default"].getFloatEntity("MIN"));
        ui->gain_slider_box->setMaximum(machine->config["Camera"]["Gain"]["Data"]["Default"].getFloatEntity("MAX"));

        ui->offset_slider_box->setMinimum(machine->config["Camera"]["Offset"]["Data"]["Default"].getFloatEntity("MIN"));
        ui->offset_slider_box->setMaximum(machine->config["Camera"]["Offset"]["Data"]["Default"].getFloatEntity("MAX"));

        ui->line_laser_slider_box->setMinimum(machine->config["Hardware"]["Controls"]["LineLaser"]["Data"]["Default"].getFloatEntity("MIN"));
        ui->line_laser_slider_box->setMaximum(machine->config["Hardware"]["Controls"]["LineLaser"]["Data"]["Default"].getFloatEntity("MAX"));

        //send default commands here
        ui->gain_slider_box->setValue(machine->config["Camera"]["Gain"]["Data"]["Caliberation"].getFloatEntity("VALUE"));
        ui->offset_slider_box->setValue(machine->config["Camera"]["Offset"]["Data"]["Caliberation"].getFloatEntity("VALUE"));
        ui->line_laser_slider_box->setValue(machine->config["Hardware"]["Controls"]["LineLaser"]["Data"]["Caliberation"].getFloatEntity("VALUE"));

        //parentWidget()->  setCurrentWidget(ui->light_caliberation_section);
        QMetaObject::invokeMethod(processing, "setActiveProcess", Qt::QueuedConnection,Q_ARG(const char*,SLOT(passThroughFrame(char* ,unsigned int,unsigned int))));

        if(dynamic_cast<QStackedWidget*>(parentWidget()))
        {
            //setting the stacked widget page to this widget
            dynamic_cast<QStackedWidget*>(parentWidget())->setCurrentWidget(this);
        }
        else {
            qWarning() << QString("parent pointer of %1 is not valid or is not of type QStackedWidget").arg(this->objectName());
        }
    }
}

/* Function : setupConnections()
 * this function sets the necessary connections between ui and machine
 * and defaults related to caliberation entities
 * Created: 09_05_2019
*/
bool _ScanCaliberationSection::setupConnections()
{
    if(machine){

        //setup connections here

        connect(ui->scan_laser_button, &QPushButton::clicked,[this]() {
            qDebug() << application_settings->toJson();
            QString dir = application_settings->getChildEntity("Paths").getStringEntity("SCAN_STORE_PATH");
            QMetaObject::invokeMethod(machine, "setImageDir", Qt::QueuedConnection,Q_ARG(QString, dir));
            QMetaObject::invokeMethod(scanner, "scanGenerateModel", Qt::QueuedConnection);
        });

        connect(ui->scan_button, &QPushButton::clicked,[this]() {
            qDebug() << application_settings->toJson();
            QString dir = application_settings->getChildEntity("Paths").getStringEntity("SCAN_STORE_PATH");
            QMetaObject::invokeMethod(machine, "setImageDir", Qt::QueuedConnection,Q_ARG(QString, dir));
            QMetaObject::invokeMethod(scanner, "scanImages", Qt::QueuedConnection);
        });

        connect(ui->line_laser_slider_box,SIGNAL(valueChanged(int)),machine,SLOT(LineLaser(int)));
        connect(ui->offset_slider_box,SIGNAL(valueChanged(int)),machine,SLOT(setOffset(int)));
        connect(ui->gain_slider_box,SIGNAL(valueChanged(int)),machine,SLOT(setGain(int)));

        qDebug() << "light caliberation section connections set";
        return true;
    }
    qDebug() << "light caliberation section connections failed to setup";
    return false;
}


/* Function : deleteConnections()
 * this function deletes the necessary connections between ui and machine
 * and defaults related to caliberation entities
 * Created: 09_05_2019
*/
bool _ScanCaliberationSection::deleteConnections()
{
    if(machine){

        //delete connections here
        disconnect(ui->line_laser_slider_box,SIGNAL(valueChanged(int)),machine,SLOT(LineLaser(int)));
        disconnect(ui->offset_slider_box,SIGNAL(valueChanged(int)),machine,SLOT(setOffset(int)));
        disconnect(ui->gain_slider_box,SIGNAL(valueChanged(int)),machine,SLOT(setGain(int)));

        qDebug() << "light caliberation section connections deleted";

        return true;
    }
    qDebug() << "light caliberation section connections failed to delete";
    return false;
}

/* Function : scan()
 * this function scan the stone and compares it with referance stone
 * Created: 09_05_2019
*/
void _ScanCaliberationSection::scan()
{
    //save caliberated values
    qDebug() << "scanning";
}
