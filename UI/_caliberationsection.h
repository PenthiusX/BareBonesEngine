#ifndef _CALIBERATIONSECTION_H
#define _CALIBERATIONSECTION_H

#include <QWidget>
#include <IO/_machine.h>

class _CaliberationSection : public QWidget
{
    Q_OBJECT

public:
    explicit _CaliberationSection(QWidget *parent = nullptr);
    ~_CaliberationSection();

    static void setMachine(_Machine* mach);
    static void setApplicationSettings(_ConfigControlEntity *app_sett);

    //virtual bool setupConnections();
    //virtual bool deleteConnections();

signals:

public slots:

    virtual void init();
    void saveConfig();

protected:
    static _Machine *machine;
    static _ConfigControlEntity *application_settings;

};

#endif // _CALIBERATIONSECTION_H
