#include "_machineselectionsection.h"
#include "ui__machineselectionsection.h"
#include <_tools.h>
#include <QMessageBox>

_MachineSelectionSection::_MachineSelectionSection(QWidget *parent) :
    _CaliberationSection(parent),
    ui(new Ui::_MachineSelectionSection)
{
    ui->setupUi(this);
    connect(ui->save_button,SIGNAL(clicked()),this,SLOT(save()));
}

_MachineSelectionSection::~_MachineSelectionSection()
{
    delete ui;
}

void _MachineSelectionSection::save()
{
    saveConfig();
}
