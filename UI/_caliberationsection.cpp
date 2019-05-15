#include "_caliberationsection.h"
#include <QMessageBox>
#include <QDebug>

//global static objects which will be required for operations
_ConfigControlEntity* _CaliberationSection::application_settings = nullptr;
_Machine* _CaliberationSection::machine=nullptr;
_Scanner* _CaliberationSection::scanner=nullptr;
_Marker* _CaliberationSection::marker=nullptr;
_Processing* _CaliberationSection::processing=nullptr;


/*
 * The _CaliberationSection class
 *
 *      ***** parent Widget of this class should be QStackedWidget *****
 *
 * parent class for differant caliberation section
 * inherits QStackedWidget and implements UI/_lightcaliberationsection.ui file gui
 * this class is base for all caliberation sections caliberation section
 * contains global objects,modules like machine , scanner , processing
 * Created: 15_05_2019
 * Author:Saurabh
*/

/* _CaliberationSection constructor
 * empty constructor
 * Created: 15_05_2019
*/
_CaliberationSection::_CaliberationSection(QStackedWidget *parent) :
    QStackedWidget(parent)
{
}

/* _LightCaliberationSection destructor
 * empty destructors
 * Created: 15_05_2019
*/
_CaliberationSection::~_CaliberationSection()
{
}



/* Function : setMachine(_Machine *mach)
 * this function sets the global machine inside this class
 * Created: 15_05_2019
*/
void _CaliberationSection::setMachine(_Machine *mach)
{
    //copies pointer to global machine
    machine = mach;
}

/* Function : setScanner(_Scanner *scann)
 * this function sets the global scanner inside this class
 * Created: 15_05_2019
*/
void _CaliberationSection::setScanner(_Scanner *scann)
{
    //copies pointer to global scanner
    scanner = scann;
}

/* Function : setProcessing(_Processing *proc)
 * this function sets the global processing inside this class
 * Created: 15_05_2019
*/
void _CaliberationSection::setProcessing(_Processing *proc)
{
    processing = proc;
}

/* Function : setMarker(_Marker *mark)
 * this function sets the global marker inside this class
 * Created: 15_05_2019
*/
void _CaliberationSection::setMarker(_Marker *mark)
{
    marker=mark;
}

/* Function : setApplicationSettings(_ConfigControlEntity* app_sett)
 * this function sets the global application_settings inside this class
 * Created: 15_05_2019
*/
void _CaliberationSection::setApplicationSettings(_ConfigControlEntity* app_sett)
{
    application_settings = app_sett;
}
/* Function : init()
 * this function is virtual and should override in the child class
 * this shows this widget in QStackedWidget
 * Created: 15_05_2019
*/
void _CaliberationSection::init()
{

    if(machine->isInitialised())
    {
        //this part sets the current page of stacked widget to this widget
        {
            if(dynamic_cast<QStackedWidget*>(parentWidget()))
            {
                //setting the stacked widget page to this widget
                dynamic_cast<QStackedWidget*>(parentWidget())->setCurrentWidget(this);
            }
            else {
                qWarning() << QString("parent pointer of %1 is not valid or is not of type QStackedWidget").arg(this->objectName());
            }
        }
    }
}


/* Function : saveConfig()
 * this function saves the caliberation config
 * Created: 15_05_2019
*/
void _CaliberationSection::saveConfig()
{
    if(machine->saveConfig(application_settings->getChildEntity("Paths").getStringEntity("MACHINE_CONFIG_FILE")))
        QMessageBox::information(0, "message", "configuration file saved : "+application_settings->getChildEntity("Paths").getStringEntity("MACHINE_CONFIG_FILE"));

}
