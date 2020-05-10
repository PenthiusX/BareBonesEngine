#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

};

#endif // MAINWINDOW_H
