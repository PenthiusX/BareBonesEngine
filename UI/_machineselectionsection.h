#ifndef _MACHINESELECTIONSECTION_H
#define _MACHINESELECTIONSECTION_H

#include <QWidget>
#include <IO/_machine.h>

namespace Ui {
class _MachineSelectionSection;
}

class _MachineSelectionSection : public QWidget
{
    Q_OBJECT

public:
    explicit _MachineSelectionSection(QWidget *parent = nullptr);
    ~_MachineSelectionSection();

public slots:
    void save();
    void setApplicationSettings(_ConfigControlEntity *app_sett);
private:
    Ui::_MachineSelectionSection *ui;
};

#endif // _MACHINESELECTIONSECTION_H
