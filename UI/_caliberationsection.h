#ifndef _CALIBERATIONSECTION_H
#define _CALIBERATIONSECTION_H

#include <QWidget>
#include <IO/_machine.h>
#include <Modules/_marker.h>
#include <Modules/_scanner.h>
#include <QStackedWidget>

class _CaliberationSection : public QStackedWidget
{
    Q_OBJECT

public:
    explicit _CaliberationSection(QStackedWidget *parent = nullptr);
    ~_CaliberationSection();

    static void setMachine(_Machine* mach);
    static void setScanner(_Scanner *scann);
    static void setProcessing(_Processing* proc);
    static void setMarker(_Marker *mark);
    static void setApplicationSettings(_ConfigControlEntity *app_sett);

    //virtual bool setupConnections();
    //virtual bool deleteConnections();

signals:

public slots:

    virtual void init();
    void saveConfig();

protected:

    //global static objects which will be required for operations
    static _Machine *machine;
    static _Scanner *scanner;
    static _Marker *marker;
    static _Processing *processing;
    static _ConfigControlEntity *application_settings;

};

#endif // _CALIBERATIONSECTION_H
