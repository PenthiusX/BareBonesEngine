#ifndef _MACHINESELECTIONSECTION_H
#define _MACHINESELECTIONSECTION_H

#include <QWidget>
#include <UI/_caliberationsection.h>

namespace Ui {
class _MachineSelectionSection;
}

class _MachineSelectionSection : public _CaliberationSection
{
    Q_OBJECT

public:
    explicit _MachineSelectionSection(QStackedWidget *parent = nullptr);
    ~_MachineSelectionSection();

public slots:
    void init();
    void save();
    void showImage(QImage img);
private:
    Ui::_MachineSelectionSection *ui;
};

#endif // _MACHINESELECTIONSECTION_H
