#include "_caliberationsection.h"
#include <QMessageBox>

_CaliberationSection::_CaliberationSection(QWidget *parent) :
    QWidget(parent)
{
}

_CaliberationSection::~_CaliberationSection()
{
}



/* Function : setMachine(_Machine *mach)
 * this function sets the global machine inside this class
 * and defaults related to caliberation entities
 * Created: 03_05_2019
*/
void _CaliberationSection::setMachine(_Machine *mach)
{
    //copies pointer to global machine
    machine = mach;

//    ui->gain_slider_box->setMinimum(machine->config["Camera"]["Gain"]["Data"]["Default"].getFloatEntity("MIN"));
//    ui->gain_slider_box->setMaximum(machine->config["Camera"]["Gain"]["Data"]["Default"].getFloatEntity("MAX"));

//    ui->offset_slider_box->setMinimum(machine->config["Camera"]["Offset"]["Data"]["Default"].getFloatEntity("MIN"));
//    ui->offset_slider_box->setMaximum(machine->config["Camera"]["Offset"]["Data"]["Default"].getFloatEntity("MAX"));

//    ui->backlight_slider_box->setMinimum(machine->config["Hardware"]["Controls"]["BackLight"]["Data"]["Default"].getFloatEntity("MIN"));
//    ui->backlight_slider_box->setMaximum(machine->config["Hardware"]["Controls"]["BackLight"]["Data"]["Default"].getFloatEntity("MAX"));

}
void _CaliberationSection::setApplicationSettings(_ConfigControlEntity* app_sett)
{
    application_settings = app_sett;
}
/* Function : init()
 * this function initialises the default values for machine commands
 * default commands to hardware are also sent
 * when machine is initialised this function is called
 * Created: 06_05_2019
*/
void _CaliberationSection::init()
{

    if(machine->isInitialised())
    {
        //send default commands here
//        ui->lens_number_dropdown->setCurrentIndex(machine->config["Camera"]["Lens"].getFloatEntity("LENS_NUMBER"));
//        ui->gain_slider_box->setValue(machine->config["Camera"]["Gain"]["Data"]["Caliberation"].getFloatEntity("VALUE"));
//        ui->offset_slider_box->setValue(machine->config["Camera"]["Offset"]["Data"]["Caliberation"].getFloatEntity("VALUE"));
//        ui->backlight_slider_box->setValue(machine->config["Hardware"]["Controls"]["BackLight"]["Data"]["Caliberation"].getFloatEntity("VALUE"));

    }
}

///* Function : setupConnections()
// * this function sets the necessary connections between ui and machine
// * and defaults related to caliberation entities
// * Created: 03_05_2019
//*/
//bool _CaliberationSection::setupConnections()
//{
//    if(machine){

//        //setup connections here

////        connect(ui->backlight_slider_box,SIGNAL(valueChanged(int)),machine,SLOT(BackLight(int)));
////        connect(ui->offset_slider_box,SIGNAL(valueChanged(int)),machine,SLOT(setOffset(int)));
////        connect(ui->gain_slider_box,SIGNAL(valueChanged(int)),machine,SLOT(setGain(int)));

////        connect(machine,SIGNAL(initMachine()),this,SLOT(init()));
////        connect(ui->save_button,SIGNAL(clicked()),this,SLOT(save()));

////        qDebug() << "light caliberation section connections set";

//        return true;
//    }
//    //qDebug() << "light caliberation section connections failed to setup";
//    return false;
//}


///* Function : deleteConnections()
// * this function deletes the necessary connections between ui and machine
// * and defaults related to caliberation entities
// * Created: 03_05_2019
//*/
//bool _CaliberationSection::deleteConnections()
//{
//    if(machine){

//        //delete connections here
//        disconnect(ui->backlight_slider_box,SIGNAL(valueChanged(int)),machine,SLOT(BackLight(int)));
//        disconnect(ui->offset_slider_box,SIGNAL(valueChanged(int)),machine,SLOT(setOffset(int)));
//        disconnect(ui->gain_slider_box,SIGNAL(valueChanged(int)),machine,SLOT(setGain(int)));

//        qDebug() << "light caliberation section connections deleted";

//        return true;
//    }
//    qDebug() << "light caliberation section connections failed to delete";
//    return false;
//}

/* Function : saveCoonfig()
 * this function saves the necessary values in this caliberation section
 * Created: 06_05_2019
*/
void _CaliberationSection::saveConfig()
{
    if(machine->saveConfig(application_settings->getChildEntity("Paths").getStringEntity("MACHINE_CONFIG_FILE")))
        QMessageBox::information(0, "message", "configuration file saved : "+application_settings->getChildEntity("Paths").getStringEntity("MACHINE_CONFIG_FILE"));

}
