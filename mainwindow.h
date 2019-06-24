#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <IO/_machine.h>
#include <Modules/_marker.h>
#include <Modules/_scanner.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void update_camera_image(char* img,uint w,uint h);

private:
    Ui::MainWindow *ui;

    QThread *hardwareInteractionThread;//all hardware IO operations done inside this thread

    //all the following objects should be moved and work in the hardwareInteractionThread
    _Machine *machine;
    _Marker *marker;
    _Scanner *scanner;
};

#endif // MAINWINDOW_H
