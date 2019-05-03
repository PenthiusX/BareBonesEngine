#include "_lightcaliberationsection.h"


_LightCaliberationSection::_LightCaliberationSection(QWidget *parent) : QWidget(parent),
    ui(new Ui::LightCaliberationSection)
{
    ui->setupUi(this);
}

_LightCaliberationSection::~_LightCaliberationSection()
{
    delete ui;
}
