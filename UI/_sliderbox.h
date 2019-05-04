#ifndef _SLIDERBOX_H
#define _SLIDERBOX_H

#include <QGroupBox>

namespace Ui {
class _SliderBox;
}

class _SliderBox : public QGroupBox
{
    Q_OBJECT

public:
    explicit _SliderBox(QWidget *parent = nullptr);
    ~_SliderBox();

signals:
    void valueChanged(int value);

public slots:
    void setValue(int value);
    void emitValue(int value);

    void setMinimum(int value);
    void setMaximum(int value);

private:
    Ui::_SliderBox *ui;
};

#endif // _SLIDERBOX_H
