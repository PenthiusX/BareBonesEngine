#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QThread>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    hardwareInteractionThread = new QThread;

    qDebug() << "created hardware objects in thread :" << QThread::currentThread();
    connect(hardwareInteractionThread, &QThread::finished,[this](){

    });

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
void MainWindow::update_camera_image(char *img, uint w, uint h)
{
    //ui->widget->update_camera_image(img,w,h);
}
