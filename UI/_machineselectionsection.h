#ifndef _MACHINESELECTIONSECTION_H
#define _MACHINESELECTIONSECTION_H

#include <QWidget>

namespace Ui {
class _MachineSelectionSection;
}

class _MachineSelectionSection : public QWidget
{
    Q_OBJECT

public:
    explicit _MachineSelectionSection(QWidget *parent = nullptr);
    ~_MachineSelectionSection();

private:
    Ui::_MachineSelectionSection *ui;
};

#endif // _MACHINESELECTIONSECTION_H
