#ifndef _APPPLICATIONSETTINGSDIALOG_H
#define _APPPLICATIONSETTINGSDIALOG_H

#include <QDialog>
#include <IO/_configcontrolentity.h>
namespace Ui {
class _AppplicationSettingsDialog;
}

class _AppplicationSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit _AppplicationSettingsDialog(_ConfigControlEntity& app_settings_config,QWidget *parent = nullptr);
    ~_AppplicationSettingsDialog();

public slots:
    void accept();

private:
    Ui::_AppplicationSettingsDialog *ui;
    _ConfigControlEntity& config;

};

#endif // _APPPLICATIONSETTINGSDIALOG_H
