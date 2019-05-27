#ifndef _SCANCALIBERATIONSECTION_H
#define _SCANCALIBERATIONSECTION_H

#include <QWidget>
#include <_tools.h>
#include <UI/_caliberationsection.h>
#include <IO/_machine.h>
#include <Modules/_scanner.h>
#include <QDebug>

namespace Ui {
class _ScanCaliberationSection;
}

class _ScanCaliberationSection : public _CaliberationSection
{
    Q_OBJECT

public:
    explicit _ScanCaliberationSection(QStackedWidget *parent = nullptr);
    ~_ScanCaliberationSection();

public slots:
    void init();
    bool setupConnections();
    bool deleteConnections();
    void scan();
private:
    Ui::_ScanCaliberationSection *ui;
};

#endif // _SCANCALIBERATIONSECTION_H
