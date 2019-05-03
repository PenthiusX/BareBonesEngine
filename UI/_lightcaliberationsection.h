#ifndef _LIGHTCALIBERATIONSECTION_H
#define _LIGHTCALIBERATIONSECTION_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "ui_LightCaliberationSection.h"

class _LightCaliberationSection : public QWidget
{
    Q_OBJECT
public:
    explicit _LightCaliberationSection(QWidget *parent = nullptr);
    ~_LightCaliberationSection();

signals:

public slots:

private:
    Ui::LightCaliberationSection *ui;
};

#endif // _LIGHTCALIBERATIONSECTION_H
