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

    ui->gain_slider_box->setMinimum(machine->config["Camera"]["Gain"]["Data"]["Default"].getFloatEntity("MIN"));
    ui->gain_slider_box->setMaximum(machine->config["Camera"]["Gain"]["Data"]["Default"].getFloatEntity("MAX"));

    ui->offset_slider_box->setMinimum(machine->config["Camera"]["Offset"]["Data"]["Default"].getFloatEntity("MIN"));
    ui->offset_slider_box->setMaximum(machine->config["Camera"]["Offset"]["Data"]["Default"].getFloatEntity("MAX"));

    ui->backlight_slider_box->setMinimum(machine->config["Hardware"]["Controls"]["BackLight"]["Data"]["Default"].getFloatEntity("MIN"));
    ui->backlight_slider_box->setMaximum(machine->config["Hardware"]["Controls"]["BackLight"]["Data"]["Default"].getFloatEntity("MAX"));

    ui->line_laser_slider_box->setMinimum(machine->config["Hardware"]["Controls"]["LineLaser"]["Data"]["Default"].getFloatEntity("MIN"));
    ui->line_laser_slider_box->setMaximum(machine->config["Hardware"]["Controls"]["LineLaser"]["Data"]["Default"].getFloatEntity("MAX"));

//    ui->gain_slider_box->setMinimum(machine->config["Camera"]["Gain"]["Data"]["Default"].getFloatEntity("MIN"));
//    ui->gain_slider_box->setMaximum(machine->config["Camera"]["Gain"]["Data"]["Default"].getFloatEntity("MAX"));

}

void _LightCaliberationSection::init()
{

    if(machine->isInitialised())
    {
        //send default commands here
        ui->gain_slider_box->setValue(machine->config["Camera"]["Gain"]["Data"]["Caliberation"].getFloatEntity("VALUE"));
        ui->offset_slider_box->setValue(machine->config["Camera"]["Offset"]["Data"]["Caliberation"].getFloatEntity("VALUE"));
        ui->backlight_slider_box->setValue(machine->config["Hardware"]["Controls"]["BackLight"]["Data"]["Caliberation"].getFloatEntity("VALUE"));
    }
}

bool _LightCaliberationSection::setupConnections()
{
    if(machine){

        //setup connections here

        connect(ui->backlight_slider_box,SIGNAL(valueChanged(int)),machine,SLOT(BackLight(int)));
        connect(ui->line_laser_slider_box,SIGNAL(valueChanged(int)),machine,SLOT(LineLaser(int)));
        connect(ui->offset_slider_box,SIGNAL(valueChanged(int)),machine,SLOT(setOffset(int)));
        connect(ui->gain_slider_box,SIGNAL(valueChanged(int)),machine,SLOT(setGain(int)));

        connect(machine,SIGNAL(initMachine()),this,SLOT(init()));

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
