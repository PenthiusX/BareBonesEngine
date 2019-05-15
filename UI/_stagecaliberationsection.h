#ifndef _STAGECALIBERATIONSECTION_H
#define _STAGECALIBERATIONSECTION_H

#include <QWidget>
#include "ui__lightcaliberationsection.h"
#include <UI/_caliberationsection.h>
#include <IO/_machine.h>

namespace Ui {
class _StageCaliberationSection;
}

class _StageCaliberationSection : public _CaliberationSection
{
    Q_OBJECT

public:
    explicit _StageCaliberationSection(QStackedWidget *parent = nullptr);
    ~_StageCaliberationSection();

    bool setupConnections();
    bool deleteConnections();
public slots:
    void init();
    void save();
private:
    Ui::_StageCaliberationSection *ui;
};

#endif // _STAGECALIBERATIONSECTION_H
