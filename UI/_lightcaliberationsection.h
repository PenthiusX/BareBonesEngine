#ifndef _LIGHTCALIBERATIONSECTION_H
#define _LIGHTCALIBERATIONSECTION_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "ui__lightcaliberationsection.h"
#include <UI/_caliberationsection.h>
#include <IO/_machine.h>

class _LightCaliberationSection : public _CaliberationSection
{
    Q_OBJECT
public:
    explicit _LightCaliberationSection(QStackedWidget *parent = nullptr);
    ~_LightCaliberationSection();

    bool setupConnections();
    bool deleteConnections();

signals:

public slots:

    void init();
    void updateHistogramImage(char *img, unsigned int w, unsigned int h);
    void save();

private:
    Ui::LightCaliberationSection *ui;
};

#endif // _LIGHTCALIBERATIONSECTION_H
