#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "IO/_hardwareserial.h"
#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //machine,marker,scanner should be in same thread -
    hardwareInteractionThread = new QThread;

    //setup hardware interacting obejcts
    //machine to be intialised first

    machine = new _Machine();
    machine->moveToThread(hardwareInteractionThread);

    marker = new _Marker(machine);
    marker->moveToThread(hardwareInteractionThread);

    scanner = new _Scanner(machine);
    scanner->moveToThread(hardwareInteractionThread);

    //
    qDebug() << "created hardware objects in thread :" << QThread::currentThread();

    connect(hardwareInteractionThread, &QThread::finished,[this]() {
        machine->deleteLater();
        scanner->deleteLater();
        marker->deleteLater();
    });    //initialize machine which will create hardware objects in new thread
    connect(hardwareInteractionThread,SIGNAL(started()),machine,SLOT(init()));
    connect(hardwareInteractionThread,SIGNAL(started()),scanner,SLOT(init()));

    /* button connections
     *  slot function implemented in child object of MainWindow and used by lambda functions
     *  https://medium.com/genymobile/how-c-lambda-expressions-can-improve-your-qt-code-8cd524f4ed9f#8138
     * */

    //buttons to machine slots connections
    //since the machine object is working in differant thread we have to use invokeMethod function
    connect(ui->stage_left, &QPushButton::clicked,[this]() {QMetaObject::invokeMethod(machine, "TurnTableMotorDiff", Qt::QueuedConnection,Q_ARG(int, 100));});
    connect(ui->stage_right, &QPushButton::clicked,[this]() {QMetaObject::invokeMethod(machine, "TurnTableMotorDiff", Qt::QueuedConnection,Q_ARG(int, -100));});
    connect(ui->backlight_on, &QPushButton::clicked,[this]() {QMetaObject::invokeMethod(machine, "BackLight", Qt::QueuedConnection,Q_ARG(int, 136));});
    connect(ui->backlight_off, &QPushButton::clicked,[this]() {QMetaObject::invokeMethod(machine, "BackLight", Qt::QueuedConnection,Q_ARG(int, 0));});
    connect(ui->line_laser_on, &QPushButton::clicked,[this]() {QMetaObject::invokeMethod(machine, "LineLaser", Qt::QueuedConnection,Q_ARG(int, 112));});
    connect(ui->line_laser_off, &QPushButton::clicked,[this]() {QMetaObject::invokeMethod(machine, "LineLaser", Qt::QueuedConnection,Q_ARG(int, 0));});
    connect(ui->marking_laser_on, &QPushButton::clicked,[this]() {QMetaObject::invokeMethod(machine, "MarkingLaser", Qt::QueuedConnection,Q_ARG(float, 4));});
    connect(ui->marking_laser_off, &QPushButton::clicked,[this]() {QMetaObject::invokeMethod(machine, "MarkingLaser", Qt::QueuedConnection,Q_ARG(float, 0));});
    connect(ui->laser_height_up, &QPushButton::clicked,[this]() {QMetaObject::invokeMethod(machine, "LaserHeightMotorDiff", Qt::QueuedConnection,Q_ARG(int, 100));});
    connect(ui->laser_height_down, &QPushButton::clicked,[this]() {QMetaObject::invokeMethod(machine, "LaserHeightMotorDiff", Qt::QueuedConnection,Q_ARG(int, -100));});
    connect(ui->laser_focus_in, &QPushButton::clicked,[this]() {QMetaObject::invokeMethod(machine, "LaserFocusMotorDiff", Qt::QueuedConnection,Q_ARG(int, -300));});
    connect(ui->laser_focus_out, &QPushButton::clicked,[this]() {QMetaObject::invokeMethod(machine, "LaserFocusMotorDiff", Qt::QueuedConnection,Q_ARG(int, 300));});
    connect(ui->vaccum_on, &QPushButton::clicked,[this]() {QMetaObject::invokeMethod(machine, "Vaccum", Qt::QueuedConnection,Q_ARG(int, 1));});
    connect(ui->vaccum_off, &QPushButton::clicked,[this]() {QMetaObject::invokeMethod(machine, "Vaccum", Qt::QueuedConnection,Q_ARG(int, 0));});

    //
    connect(ui->grab_frame, &QPushButton::clicked,[this]() {QMetaObject::invokeMethod(machine, "GrabFrame", Qt::QueuedConnection,Q_ARG(QString,"image_0.pgm"));});
    //connect(machine,SIGNAL(cameraFrameReturned(char*,unsigned int,unsigned int)),this,SLOT(update_camera_image(char*,unsigned int ,unsigned int)));

    //buttons to marker slots connections
    connect(ui->motors_setup,SIGNAL(clicked()),marker,SLOT(motors_setup()));
    connect(ui->mark_sine_wave,SIGNAL(clicked()),marker,SLOT(mark_sine_wave()));

    //buttons to scanner slots connections
    //connect(ui->scan, SIGNAL(clicked()),scanner,SLOT(scan_generate_model()));
    connect(ui->scan, &QPushButton::clicked,[this]() {
        QString dir = QFileDialog::getExistingDirectory(this, tr("Open Images Directory"),
                                                    QCoreApplication::applicationDirPath()+"/../scan_images",
                                                    QFileDialog::ShowDirsOnly);

        QMetaObject::invokeMethod(machine, "set_image_dir", Qt::QueuedConnection,Q_ARG(QString, dir));
        QMetaObject::invokeMethod(scanner, "scan_generate_model", Qt::QueuedConnection);
    });

    connect(scanner,SIGNAL(set_image(char*,unsigned int,unsigned int)),this,SLOT(update_camera_image(char*,unsigned int ,unsigned int)));
    //connect(marker,SIGNAL(set_image(char*,unsigned int,unsigned int)),this,SLOT(update_camera_image(char*,unsigned int ,unsigned int)));


    //start the hardware thread
    hardwareInteractionThread->start();
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
  ui->widget->update_background_image(img,w,h);//------------------------Needs work!!!!!
}
