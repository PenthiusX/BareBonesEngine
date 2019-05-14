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

    void init();
    void updateHistogramImage(char *img, unsigned int w, unsigned int h);
    void save();

    void setApplicationSettings(_ConfigControlEntity *app_sett);
private:
    Ui::LightCaliberationSection *ui;
    _Machine *machine=nullptr;
    _ConfigControlEntity *application_settings=nullptr;
};

#endif // _LIGHTCALIBERATIONSECTION_H
