#include "_machineselectionsection.h"
#include "ui__machineselectionsection.h"
#include <_tools.h>
#include <QMessageBox>

_MachineSelectionSection::_MachineSelectionSection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::_MachineSelectionSection)
{
    ui->setupUi(this);
}

_MachineSelectionSection::~_MachineSelectionSection()
{
    delete ui;
}

void _MachineSelectionSection::save()
{
    if(machine->saveConfig(application_settings->getChildEntity("Paths").getStringEntity("MACHINE_CONFIG_FILE")))
        QMessageBox::information(0, "message", "configuration file saved : "+application_settings->getChildEntity("Paths").getStringEntity("MACHINE_CONFIG_FILE"));
}

void _MachineSelectionSection::setApplicationSettings(_ConfigControlEntity* app_sett)
{
    application_settings = app_sett;
}
