#include "_sliderbox.h"
#include "ui__sliderbox.h"

_SliderBox::_SliderBox(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::_SliderBox)
{
    ui->setupUi(this);
    connect(ui->slider,SIGNAL(valueChanged(int)),this,SLOT(emitValue(int)));
}

_SliderBox::~_SliderBox()
{
    delete ui;
}

void _SliderBox::setValue(int value)
{
    ui->slider->setValue(value);
}

void _SliderBox::emitValue(int value)
{
    emit valueChanged(value);
}

void _SliderBox::setMinimum(int value)
{
    ui->slider->setMinimum(value);
    ui->spinBox->setMinimum(value);
}

void _SliderBox::setMaximum(int value)
{
    ui->slider->setMaximum(value);
    ui->spinBox->setMaximum(value);
}
