#include "_lightcaliberationsection.h"
#include <QDebug>


/*
 * The _LightCaliberationSection class
 * inherites QWidget and implements UI/_lightcaliberationsection.ui file gui
 * this class contain all function inside the light caliberation section
 * Created: 03_05_2019
 * Author:Saurabh
*/

/* _LightCaliberationSection constructor
 * this constructor sets up the ui
 * Created: 03_05_2019
*/
_LightCaliberationSection::_LightCaliberationSection(QWidget *parent) : QWidget(parent),
    ui(new Ui::LightCaliberationSection)
{
    ui->setupUi(this);
}

/* _LightCaliberationSection destructor
 * this constructor deletes the ui
 * Created: 03_05_2019
*/
_LightCaliberationSection::~_LightCaliberationSection()
{
    delete ui;
}


/* Function : setMachine(_Machine *mach)
 * this function sets the global machine inside this class
 * and defaults related to caliberation entities
 * Created: 03_05_2019
*/
void _LightCaliberationSection::setMachine(_Machine *mach)
{
    //copies pointer to global machine
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

}

/* Function : init()
 * this function initialises the default values for machine commands
 * default commands to hardware are also sent
 * when machine is initialised this function is called
 * Created: 06_05_2019
*/
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


/* Function : setupConnections()
 * this function sets the necessary connections between ui and machine
 * and defaults related to caliberation entities
 * Created: 03_05_2019
*/
bool _LightCaliberationSection::setupConnections()
{
    if(machine){

        //setup connections here

        connect(ui->backlight_slider_box,SIGNAL(valueChanged(int)),machine,SLOT(BackLight(int)));
        connect(ui->line_laser_slider_box,SIGNAL(valueChanged(int)),machine,SLOT(LineLaser(int)));
        connect(ui->offset_slider_box,SIGNAL(valueChanged(int)),machine,SLOT(setOffset(int)));
        connect(ui->gain_slider_box,SIGNAL(valueChanged(int)),machine,SLOT(setGain(int)));

        connect(machine,SIGNAL(initMachine()),this,SLOT(init()));
        connect(ui->save_button,SIGNAL(clicked()),this,SLOT(save()));

        qDebug() << "light caliberation section connections set";
        return true;
    }
    qDebug() << "light caliberation section connections failed to setup";
    return false;
}


/* Function : deleteConnections()
 * this function deletes the necessary connections between ui and machine
 * and defaults related to caliberation entities
 * Created: 03_05_2019
*/
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
void _LightCaliberationSection::save()
{
    //save caliberated values
    machine->BackLight(ui->backlight_slider_box->value(),_CALIBERATION_CONFIG_UPDATE);
    machine->setGain(ui->gain_slider_box->value(),_CALIBERATION_CONFIG_UPDATE);
    machine->setOffset(ui->offset_slider_box->value(),_CALIBERATION_CONFIG_UPDATE);

    machine->saveConfig();
}

/* Function : update_histogram_image(char *img, unsigned int w, unsigned int h)
 * this function updates the histogram label with histogram image
 * Created: 03_05_2019
*/
void _LightCaliberationSection::updateHistogramImage(char *img, unsigned int w, unsigned int h)
{
    //ui->histogram_image_label->setPixmap(pixmap);
    int lw = ui->histogram_image_label->width();
    int lh = (lw*3)/4;//ui->histogram_image_label->height();

    QImage Img((uchar*)img, w, h, QImage::Format_Grayscale8);

    QPixmap pixmap;

    pixmap.convertFromImage(Img);

    // set a scaled pixmap to a w x h window keeping its aspect ratio
    ui->histogram_image_label->setPixmap(pixmap.scaled(lw,lh));
}
