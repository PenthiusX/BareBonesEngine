#include "_machineselectionsection.h"
#include "ui__machineselectionsection.h"
#include <_tools.h>
#include <QMessageBox>
#include <QDebug>

_MachineSelectionSection::_MachineSelectionSection(QStackedWidget *parent) :
    _CaliberationSection(parent),
    ui(new Ui::_MachineSelectionSection)
{
    delete layout(); //overwriting layout in setupUi
    ui->setupUi(this);
    connect(ui->save_button,SIGNAL(clicked()),this,SLOT(save()));
}

_MachineSelectionSection::~_MachineSelectionSection()
{
    delete ui;
}

/* Function : init()
 * this function initialises the default values
 * default commands to hardware are also sent
 * when section button is clicked this function is called
 * this also shows the this widget in QStackedWidget
 * Created: 06_05_2019
*/
void _MachineSelectionSection::init()
{

    if(machine->isInitialised())
    {
        int n = ui->machine_type_dropdown->count();
        for (int index = 0; index < n; ++index) {
            QString l = machine->config["Configuration"].getStringEntity("MACHINE_TYPE") ;
            QString r = ui->machine_type_dropdown->itemText(index);
            if(l==r)
                ui->machine_type_dropdown->setCurrentIndex(index);
        }

        QString infoString; // Declare taskId.
        QMetaObject::invokeMethod(machine,
                              "getMachineVersion",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(QString, infoString));
        ui->machine_info_text_browser->setText(infoString);

        QString dir = application_settings->getChildEntity("Paths").getStringEntity("SCAN_STORE_PATH");

        machine->camera->setImageDir(dir);

        QMetaObject::invokeMethod(processing, "setActiveProcess", Qt::QueuedConnection,Q_ARG(const char*,SLOT(passThroughFrame(char* ,unsigned int,unsigned int))));

        //this part sets the current page of stacked widget to this widget
        {
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
}

void _MachineSelectionSection::save()
{
    machine->config["Configuration"].getStringEntity("MACHINE_TYPE") = ui->machine_type_dropdown->currentText();
    saveConfig();
}
