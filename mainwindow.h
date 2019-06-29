#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <IO/_machine.h>
#include <Modules/_marker.h>
#include <Modules/_scanner.h>
#include <Modules/_processing.h>
#include <QFile>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

public slots:

    void update_camera_image(char* img,uint w,uint h);
    void openSettingsDialog();
    void setConfigSettings();
    void msgBox(QString windowname, QString info);
    void showGeneratedModel(char *img, uint iwidth, uint iheight);

private:
    Ui::MainWindow *ui;
    QThread *hardwareInteractionThread;//all hardware IO operations done inside this thread
    //all the following objects should be moved and work in the hardwareInteractionThread
    _Machine *machine=nullptr;
    _Marker *marker=nullptr;
    _Scanner *scanner=nullptr;
    _Processing *processing=nullptr;


    _ConfigControlEntity *application_settings=nullptr;
};
#endif // MAINWINDOW_H
