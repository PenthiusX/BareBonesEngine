#include "_stagecaliberationsection.h"
#include "ui__stagecaliberationsection.h"
#include <QDebug>
#include <_tools.h>
#include <QMessageBox>
#include <UI/_caliberationsection.h>
/*
 * The _StageCaliberationSection class
 *
 *      ***** parent Widget of this class should be QStackedWidget *****
 *
 * inherites QWidget and implements UI/_lightcaliberationsection.ui file gui
 * this class contain all function inside the light caliberation section
 * Created: 03_05_2019
 * Author:Saurabh
*/

/* _StageCaliberationSection constructor
 * this constructor sets up the ui
 * Created: 03_05_2019
*/
_StageCaliberationSection::_StageCaliberationSection(QStackedWidget *parent) : _CaliberationSection(parent),
    ui(new Ui::_StageCaliberationSection)
{

    delete layout(); //overwriting layout in setupUi
    ui->setupUi(this);
}

/* _StageCaliberationSection destructor
 * this destructor deletes the ui
 * Created: 03_05_2019
*/
_StageCaliberationSection::~_StageCaliberationSection()
{
    delete ui;
}

/* Function : init()
 * this function initialises the default values for machine commands
 * default commands to hardware are also sent
 * when machine is initialised this function is called
 * Created: 06_05_2019
*/
void _StageCaliberationSection::init()
{
    setupConnections();

    if(machine->isInitialised())
    {
        //set the defaults here (min/max)

        QMetaObject::invokeMethod(processing, "setActiveProcess", Qt::QueuedConnection,Q_ARG(const char*,SLOT(markStageEdge(char* ,unsigned int,unsigned int))));


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

/* Function : setupConnections()
 * this function sets the necessary connections between ui and machine
 * and defaults related to caliberation entities
 * Created: 03_05_2019
*/
bool _StageCaliberationSection::setupConnections()
{
    if(machine){

        //setup connections here

        connect(ui->save_button,SIGNAL(clicked()),this,SLOT(save()));
        connect(processing,SIGNAL(outputImage2(char*,unsigned int,unsigned int)),this,SLOT(updateHistogramImage(char*,unsigned int ,unsigned int)));
        connect(processing,SIGNAL(stageCenterAngleOut(float,float,float)),this,SLOT(stageCenterAngleIn(float,float,float)));
        return true;
    }
    return false;
}


/* Function : deleteConnections()
 * this function deletes the necessary connections between ui and machine
 * and defaults related to caliberation entities
 * Created: 03_05_2019
*/
bool _StageCaliberationSection::deleteConnections()
{
    if(machine){

        //delete connections here
        disconnect(ui->save_button,SIGNAL(clicked()),this,SLOT(save()));
        disconnect(processing,SIGNAL(outputImage2(char*,unsigned int,unsigned int)),this,SLOT(updateHistogramImage(char*,unsigned int ,unsigned int)));
        disconnect(processing,SIGNAL(stageCenterAngleOut(float,float,float)),this,SLOT(stageCenterAngleIn(float,float,float)));
        return true;
    }
    return false;
}

/* Function : save()
 * this function saves the necessary values in this caliberation section
 * Created: 06_05_2019
*/
void _StageCaliberationSection::save()
{
    //save caliberated values
//    machine->config["Camera"]["Lens"].getFloatEntity("LENS_NUMBER") = ui->lens_number_dropdown->currentIndex() ;
//    QMetaObject::invokeMethod(machine, "BackLight", Qt::QueuedConnection,Q_ARG(int, ui->backlight_slider_box->value()),Q_ARG(ActionType,_CALIBERATION_CONFIG_UPDATE));
//    QMetaObject::invokeMethod(machine, "setGain", Qt::QueuedConnection,Q_ARG(int, ui->gain_slider_box->value()),Q_ARG(ActionType,_CALIBERATION_CONFIG_UPDATE));
//    QMetaObject::invokeMethod(machine, "setOffset", Qt::QueuedConnection,Q_ARG(int, ui->offset_slider_box->value()),Q_ARG(ActionType,_CALIBERATION_CONFIG_UPDATE));

    saveConfig();
}

///* Function : update_histogram_image(char *img, unsigned int w, unsigned int h)
// * this function updates the histogram label with histogram image
// * Created: 03_05_2019
//*/
//void _StageCaliberationSection::updateHistogramImage(char *img, unsigned int w, unsigned int h)
//{
//    //ui->histogram_image_label->setPixmap(pixmap);
//    int lw = ui->histogram_image_label->width();
//    int lh = (lw*3)/4;//ui->histogram_image_label->height();

//    QImage Img((uchar*)img, w, h, QImage::Format_Grayscale8);

//    QPixmap pixmap;

//    pixmap.convertFromImage(Img);

//    // set a scaled pixmap to a w x h window keeping its aspect ratio
//    ui->histogram_image_label->setPixmap(pixmap.scaled(lw,lh));
//}
/* Function : update_histogram_image(char *img, unsigned int w, unsigned int h)
 * this function updates the histogram label with histogram image
 * Created: 03_05_2019
*/
void _StageCaliberationSection::updateHistogramImage(char *img, unsigned int w, unsigned int h)
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

void _StageCaliberationSection::stageCenterAngleIn(float center_x, float center_y, float angle)
{
    ui->stage_angle_out_browser->setText(QString::number(angle));
    ui->stage_angle_out_browser_X->setText(QString::number(center_x));
    ui->stage_angle_out_browser_Y->setText(QString::number(center_y));
}


