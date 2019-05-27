#ifndef _PATHSETTINGFORM_H
#define _PATHSETTINGFORM_H


#include <QWidget>
#include <QFileDialog>

namespace Ui {
class _PathSettingForm;
}

class _PathSettingForm : public QWidget
{
    Q_OBJECT

public:
    explicit _PathSettingForm(QWidget *parent = nullptr);
    ~_PathSettingForm();

signals:
    void clicked();

public slots:
    QString getPath();
    QString getFile();
    void setPath(QString);
    void setText(QString text);
    bool validatePath(QString temp_path);
    void setFileTypes(QString extensions);

    QString path();
    void setType(QFileDialog::FileMode typ);
private:
    Ui::_PathSettingForm *ui;
    QString _path;
    QFileDialog::FileMode type = QFileDialog::ExistingFile;
    QString file_types = "All Files (*)";


};

#endif // _PATHSETTINGFORM_H
