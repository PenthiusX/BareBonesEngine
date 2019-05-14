#include "_machineselectionsection.h"
#include "ui__machineselectionsection.h"

_MachineSelectionSection::_MachineSelectionSection(QWidget *parent) :
    _CaliberationSection(parent),
    ui(new Ui::_MachineSelectionSection)
{
    ui->setupUi(this);
}

_MachineSelectionSection::~_MachineSelectionSection()
{
    delete ui;
}
