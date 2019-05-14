#ifndef _SCANCALIBERATIONSECTION_H
#define _SCANCALIBERATIONSECTION_H

#include <QWidget>
#include <_tools.h>
#include <IO/_machine.h>
#include <Modules/_scanner.h>
#include <QDebug>

namespace Ui {
class _ScanCaliberationSection;
}

class _ScanCaliberationSection : public QWidget
{
    Q_OBJECT

public:
    explicit _ScanCaliberationSection(QWidget *parent = nullptr);
    ~_ScanCaliberationSection();
    void setMachineScanner(_Machine *mach,_Scanner *scann);
public slots:
    void init();
    bool setupConnections();
    bool deleteConnections();
    void scan();
    void setApplicationSettings(_ConfigControlEntity *app_sett);
private:
    Ui::_ScanCaliberationSection *ui;
    _Machine *machine=nullptr;
    _Scanner *scanner=nullptr;
    _ConfigControlEntity *application_settings=nullptr;
};

#endif // _SCANCALIBERATIONSECTION_H
