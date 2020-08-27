#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "IO/_hardwareserial.h"
#include <QDebug>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}
/*
 *
*/
MainWindow::~MainWindow()
{
    delete ui;
}
/*
 *
*/
void MainWindow::update_camera_image(char *img, unsigned int w, unsigned int h)
{
    //ui->widget->update_camera_image(img,w,h);//---------------Needs work!!!!!
}
