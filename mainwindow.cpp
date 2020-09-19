#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QThread>



MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    isToolBox = false;
    ui->groupBox->setHidden(true);


//    ui->x->geometry().center().setX(ui->groupBox->width() * 0.5);ui->x->geometry().center().setY(ui->groupBox->height() * 0.5);
    ui->pushButton_2->setStyleSheet("QPushButton {color:red;}");
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{

}

void MainWindow::on_pushButton_2_clicked()
{
    isToolBox = !isToolBox;
    ui->groupBox->setHidden(isToolBox);
}

void MainWindow::on_x_stateChanged(int arg1)
{
    if(arg1 == 0){x = false;}
    else if(arg1 == 2){x= true;}
}

void MainWindow::on_y_stateChanged(int arg1)
{
    if(arg1 == 0){y = false;}
    else if(arg1 == 2){y= true;}
}

void MainWindow::on_z_stateChanged(int arg1)
{
    if(arg1 == 0){z = false;}
    else if(arg1 == 2){z= true;}
}

void MainWindow::on_Pos_stateChanged(int arg1)
{
    if(arg1 == 0){Pos = false;}
    else if(arg1 == 2){Pos= true;}
}

void MainWindow::on_Rot_stateChanged(int arg1)
{
    if(arg1 == 0){Rot = false;}
    else if(arg1 == 2){Rot= true;}
}

void MainWindow::on_Scale_stateChanged(int arg1)
{
    if(arg1 == 0){Scl = false;}
    else if(arg1 == 2){Scl= true;}
}

void MainWindow::on_hs2_valueChanged(int value)
{

}
