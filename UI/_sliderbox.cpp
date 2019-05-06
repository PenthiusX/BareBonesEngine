#include "_sliderbox.h"
#include "ui__sliderbox.h"


/* _SliderBox Class
 * QGroupBox inherited
 * has a linked pair of QSlider and QSpinBox
 * implements gui of UI/_sliderbox.ui
 * Created: 03_05_2019
 * Author:Saurabh
*/

/*_SliderBox constructor
 * sets up the ui
 * and connections
 * Created: 03_05_2019
*/
_SliderBox::_SliderBox(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::_SliderBox)
{
    ui->setupUi(this);
    connect(ui->slider,SIGNAL(valueChanged(int)),this,SLOT(emitValue(int)));
}

/*_SliderBox destructor
 * deletes the ui
 * Created: 03_05_2019
*/
_SliderBox::~_SliderBox()
{
    delete ui;
}

/* Function: setValue(int value)
 * sets the slider to given value
 * also emits the value changed signal to send amchine command
 * Created: 03_05_2019
*/
void _SliderBox::setValue(int value)
{
    ui->slider->setValue(value);
}


/* Function: emitValue(int value)
 * emits the valueChanged signal to send machine command
 * Created: 03_05_2019
*/
void _SliderBox::emitValue(int value)
{
    emit valueChanged(value);
}

/* Function: setMinimum(int value)
 * sets the Minimum slider bound
 * Created: 03_05_2019
*/
void _SliderBox::setMinimum(int value)
{
    ui->slider->setMinimum(value);
    ui->spinBox->setMinimum(value);
}

/* Function: setMaximum(int value)
 * sets the Maximum slider bound
 * Created: 03_05_2019
*/
void _SliderBox::setMaximum(int value)
{
    ui->slider->setMaximum(value);
    ui->spinBox->setMaximum(value);
}

int _SliderBox::value()
{
    return ui->slider->value();
}
