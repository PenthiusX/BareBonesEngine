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

    virtual void setMachine(_Machine* mach);
    void setApplicationSettings(_ConfigControlEntity *app_sett);

    //virtual bool setupConnections();
    //virtual bool deleteConnections();

signals:

public slots:

    virtual void init();
    void saveConfig();

protected:
    _Machine *machine=nullptr;
    _ConfigControlEntity *application_settings=nullptr;
};

#endif // _CALIBERATIONSECTION_H
