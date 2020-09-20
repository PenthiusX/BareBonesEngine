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
    void resizeEvent(QResizeEvent* event);

public slots:

private slots:
    void on_pushButton_2_clicked();
    void on_x_stateChanged(int arg1);
    void on_y_stateChanged(int arg1);
    void on_z_stateChanged(int arg1);
    void on_Pos_stateChanged(int arg1);
    void on_Rot_stateChanged(int arg1);
    void on_Scale_stateChanged(int arg1);

    void on_hs2_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    bool isToolBox;
    bool x,y,z,Pos,Rot,Scl,Amb,Spec,Diff;
};

#endif // MAINWINDOW_H
