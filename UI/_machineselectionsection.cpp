#include "_machineselectionsection.h"
#include "ui__machineselectionsection.h"

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
