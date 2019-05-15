#ifndef _MACHINESELECTIONSECTION_H
#define _MACHINESELECTIONSECTION_H

#include <QWidget>
#include <IO/_machine.h>
#include <UI/_caliberationsection.h>

namespace Ui {
class _MachineSelectionSection;
}

class _MachineSelectionSection : public _CaliberationSection
{
    Q_OBJECT

public:
    explicit _MachineSelectionSection(QWidget *parent = nullptr);
    ~_MachineSelectionSection();

public slots:
    void save();
private:
    Ui::_MachineSelectionSection *ui;
};

#endif // _MACHINESELECTIONSECTION_H
