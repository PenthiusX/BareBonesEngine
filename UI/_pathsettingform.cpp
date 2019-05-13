#include "_pathsettingform.h"
#include "ui__pathsettingform.h"
#include <QDebug>

_PathSettingForm::_PathSettingForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::_PathSettingForm)
{
    ui->setupUi(this);

    //connect(ui->browse_button, &QPushButton::clicked,this,&_PathSettingForm::clicked);
    //connect(ui->path_line_edit,SIGNAL(textChanged(QString)),this,SLOT(validatePath(QString)));
    connect(ui->browse_button, &QPushButton::clicked,[this](){switch (type) {
        case QFileDialog::Directory :
        {
            setPath(getPath());
            break;
        }
        case QFileDialog::ExistingFile :
        {
            setPath(getFile());
            break;
        }
        }

    });
}

_PathSettingForm::~_PathSettingForm()
{
    delete ui;
}

QString _PathSettingForm::getPath()
{
    QString dir = QFileDialog::getExistingDirectory(this,ui->label->text(),_path,QFileDialog::ShowDirsOnly);
    return dir;
}

QString _PathSettingForm::getFile()
{
    QString dir = QFileDialog::getOpenFileName(this,ui->label->text(),_path,file_types);
    return dir;
}

void _PathSettingForm::setPath(QString text)
{
    _path = text;
    ui->path_line_edit->setText(text);
}

void _PathSettingForm::setText(QString text)
{
    ui->label->setText(text);
}

bool _PathSettingForm::validatePath(QString temp_path)
{
    //check validity of path
    return true;
}

void _PathSettingForm::setFileTypes(QString extensions)
{
    file_types = extensions;
}

QString _PathSettingForm::path()
{
    return _path;
}

void _PathSettingForm::setType(QFileDialog::FileMode typ)
{
    this->type = typ;
}
