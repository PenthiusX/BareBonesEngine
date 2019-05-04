#ifndef _LIGHTCALIBERATIONSECTION_H
#define _LIGHTCALIBERATIONSECTION_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "ui__lightcaliberationsection.h"
#include <IO/_machine.h>

class _LightCaliberationSection : public QWidget
{
    Q_OBJECT
public:
    explicit _LightCaliberationSection(QWidget *parent = nullptr);
    ~_LightCaliberationSection();

    void setMachine(_Machine* mach);
    bool setupConnections();
    bool deleteConnections();

signals:

public slots:

private:
    Ui::LightCaliberationSection *ui;
    _Machine *machine=nullptr;
};

#endif // _LIGHTCALIBERATIONSECTION_H
