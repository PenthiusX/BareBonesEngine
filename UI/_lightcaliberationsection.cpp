#include "_lightcaliberationsection.h"
#include <QDebug>

_LightCaliberationSection::_LightCaliberationSection(QWidget *parent) : QWidget(parent),
    ui(new Ui::LightCaliberationSection)
{
    ui->setupUi(this);


}

_LightCaliberationSection::~_LightCaliberationSection()
{
    delete ui;
}

void _LightCaliberationSection::setMachine(_Machine *mach)
{
    machine = mach;

    //set the defaults here (min/max)

}

bool _LightCaliberationSection::setupConnections()
{
    if(machine){

        //setup connections here

        connect(ui->backlight_slider_box,SIGNAL(valueChanged(int)),machine,SLOT(BackLight(int)));
        connect(ui->line_laser_slider_box,SIGNAL(valueChanged(int)),machine,SLOT(LineLaser(int)));
        connect(ui->offset_slider_box,SIGNAL(valueChanged(int)),machine,SLOT(setOffset(int)));
        connect(ui->gain_slider_box,SIGNAL(valueChanged(int)),machine,SLOT(setGain(int)));

        qDebug() << "light caliberation section connections set";
        return true;
    }
    qDebug() << "light caliberation section connections failed to setup";
    return false;
}

bool _LightCaliberationSection::deleteConnections()
{
    if(machine){

        //delete connections here
        disconnect(ui->backlight_slider_box,SIGNAL(valueChanged(int)),machine,SLOT(BackLight(int)));
        disconnect(ui->line_laser_slider_box,SIGNAL(valueChanged(int)),machine,SLOT(LineLaser(int)));
        disconnect(ui->offset_slider_box,SIGNAL(valueChanged(int)),machine,SLOT(setOffset(int)));
        disconnect(ui->gain_slider_box,SIGNAL(valueChanged(int)),machine,SLOT(setGain(int)));

        qDebug() << "light caliberation section connections deleted";

        return true;
    }
    qDebug() << "light caliberation section connections failed to delete";
    return false;
}
