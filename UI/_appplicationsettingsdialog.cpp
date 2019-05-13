#include "_appplicationsettingsdialog.h"
#include "ui__appplicationsettingsdialog.h"

_AppplicationSettingsDialog::_AppplicationSettingsDialog(_ConfigControlEntity& app_settings_config,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::_AppplicationSettingsDialog),
    config(app_settings_config)
{
    ui->setupUi(this);
    config = app_settings_config;
    ui->config_file_entity_widget->setText("Config File");
    ui->scan_folder_entity_widget->setText("Scan Folder");
    ui->scan_folder_entity_widget->setType(QFileDialog::Directory);
    ui->config_file_entity_widget->setFileTypes("Configuration Files (*.json)");
    ui->config_file_entity_widget->setPath(app_settings_config["Paths"].getStringEntity("MACHINE_CONFIG_FILE"));
    ui->scan_folder_entity_widget->setPath(app_settings_config["Paths"].getStringEntity("SCAN_STORE_PATH"));
}

_AppplicationSettingsDialog::~_AppplicationSettingsDialog()
{
    delete ui;
}

void _AppplicationSettingsDialog::accept()
{
    config["Paths"].getStringEntity("MACHINE_CONFIG_FILE") = ui->config_file_entity_widget->path();
    config["Paths"].getStringEntity("SCAN_STORE_PATH")= ui->scan_folder_entity_widget->path();
    close();
}
