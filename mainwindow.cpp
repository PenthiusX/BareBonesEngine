#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "IO/_hardwareserial.h"
#include <QDebug>
#include <QPixmap>
#include <QImage>
#include <UI/_appplicationsettingsdialog.h>
#include <IO/_configcontrolentity.h>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setConfigSettings();

    connect(ui->actionApplication_Settings,SIGNAL(triggered()),this,SLOT(openSettingsDialog()));
    _CaliberationSection::setApplicationSettings(application_settings);
    qRegisterMetaType<ActionType>("ActionType");

    //machine,marker,scanner should be in same thread -
    hardwareInteractionThread = new QThread;

    //setup hardware interacting obejcts
    //machine to be intialised first

    machine = new _Machine(application_settings->getChildEntity("Paths").getStringEntity("MACHINE_CONFIG_FILE"));
    machine->moveToThread(hardwareInteractionThread);

    processing = new _Processing();
    processing->moveToThread(hardwareInteractionThread);

    marker = new _Marker(machine);
    marker->moveToThread(hardwareInteractionThread);

    scanner = new _Scanner(machine,processing);
    scanner->moveToThread(hardwareInteractionThread);

    //
    qDebug() << "created hardware objects in thread :" << QThread::currentThread();

    connect(hardwareInteractionThread, &QThread::finished,[this]() {
        machine->deleteLater();
        scanner->deleteLater();
        marker->deleteLater();
        processing->deleteLater();
    });    //initialize machine which will create hardware objects in new thread
    connect(hardwareInteractionThread,SIGNAL(started()),machine,SLOT(init()));
    connect(hardwareInteractionThread,SIGNAL(started()),processing,SLOT(init()));
    connect(hardwareInteractionThread,SIGNAL(started()),scanner,SLOT(init()));

    /* button connections
     *  slot function implemented in child object of MainWindow and used by lambda functions
     *  https://medium.com/genymobile/how-c-lambda-expressions-can-improve-your-qt-code-8cd524f4ed9f#8138
     * */

    //buttons to machine slots connections
    //since the machine object is working in differant thread we have to use invokeMethod function

    //clickable buttons

    //set the global static objects which will be required for operations
    _CaliberationSection::setMachine(machine);
    _CaliberationSection::setScanner(scanner);
    _CaliberationSection::setProcessing(processing);
    _CaliberationSection::setMarker(marker);

    connect(machine,SIGNAL(initMachine()),ui->machine_type_section,SLOT(init()));

    //
    connect(ui->light_calibration_button, SIGNAL(clicked()),ui->light_caliberation_section,SLOT(init()));
    connect(ui->machine_selection_button, SIGNAL(clicked()),ui->machine_type_section,SLOT(init()));
    connect(ui->scan_calibration_button, SIGNAL(clicked()),ui->scan_caliberation_section,SLOT(init()));
    connect(ui->stage_calibration_button, SIGNAL(clicked()),ui->stage_caliberation_section,SLOT(init()));

    connect(ui->stage_left, &QPushButton::clicked,[this]() {QMetaObject::invokeMethod(machine, "callCommandFunction", Qt::QueuedConnection,Q_ARG(QString, "StageMotor"),Q_ARG(int, 100));});
    connect(ui->stage_right, &QPushButton::clicked,[this]() {QMetaObject::invokeMethod(machine, "TurnTableMotorDiff", Qt::QueuedConnection,Q_ARG(int, -100));});
    connect(ui->laser_height_up, &QPushButton::clicked,[this]() {QMetaObject::invokeMethod(machine, "LaserHeightMotorDiff", Qt::QueuedConnection,Q_ARG(int, 100));});
    connect(ui->laser_height_down, &QPushButton::clicked,[this]() {QMetaObject::invokeMethod(machine, "LaserHeightMotorDiff", Qt::QueuedConnection,Q_ARG(int, -100));});
    connect(ui->laser_focus_in, &QPushButton::clicked,[this]() {QMetaObject::invokeMethod(machine, "LaserFocusMotorDiff", Qt::QueuedConnection,Q_ARG(int, -300));});
    connect(ui->laser_focus_out, &QPushButton::clicked,[this]() {QMetaObject::invokeMethod(machine, "LaserFocusMotorDiff", Qt::QueuedConnection,Q_ARG(int, 300));});
    //connect(ui->grab_frame, &QPushButton::clicked,[this]() {QMetaObject::invokeMethod(machine, "GrabFrame", Qt::QueuedConnection,Q_ARG(QString,"image_0.pgm"));});

    //checkable buttons
    connect(ui->backlight_button, &QPushButton::clicked,[this]() {
        QMetaObject::invokeMethod(machine, "BackLight", Qt::QueuedConnection,Q_ARG(int, 0),Q_ARG(ActionType,_STORE_VALUE_TOGGLE));
    });

    connect(ui->line_laser_button, &QPushButton::clicked,[this]() {
        QMetaObject::invokeMethod(machine, "LineLaser", Qt::QueuedConnection,Q_ARG(int, 0),Q_ARG(ActionType,_STORE_VALUE_TOGGLE));
    });

    connect(ui->marking_laser_button, &QPushButton::clicked,[this]() {
        QMetaObject::invokeMethod(machine, "MarkingLaser", Qt::QueuedConnection,Q_ARG(int, 0),Q_ARG(ActionType,_STORE_VALUE_TOGGLE));
    });
    connect(ui->vaccum_button,  &QPushButton::clicked,[this]() {
        QMetaObject::invokeMethod(machine, "Vaccum",  Qt::QueuedConnection,Q_ARG(int, 0),Q_ARG(ActionType,_STORE_VALUE_TOGGLE));
    });

    qRegisterMetaType<const char*>("const char*");//for Q_ARG to understand the char* datatype

    connect(ui->live_camera_button, &QPushButton::toggled,[this]() {
        if(ui->live_camera_button->isChecked())
            QMetaObject::invokeMethod(processing, "setActiveProcess", Qt::QueuedConnection,Q_ARG(const char*,SLOT(histogram(char* ,unsigned int,unsigned int)) ));
        else
            QMetaObject::invokeMethod(processing, "setActiveProcess", Qt::QueuedConnection,Q_ARG(const char*,nullptr));
    });

    //setActiveProcess(SLOT(passThroughFrame(char* ,unsigned int,unsigned int)));

    //slider connections

    connect(ui->contrast_slider,SIGNAL(valueChanged(int)),machine,SLOT(setContrast(int)));


    //buttons to marker slots connections
    connect(ui->motors_setup,SIGNAL(clicked()),marker,SLOT(motors_setup()));
    connect(ui->mark_sine_wave,SIGNAL(clicked()),marker,SLOT(mark_sine_wave()));


    //buttons to scanner slots connections
    //connect(ui->scan, SIGNAL(clicked()),scanner,SLOT(scan_generate_model()));

    connect(machine,SIGNAL(guiFrameOut(char*,unsigned int,unsigned int)),this,SLOT(update_camera_image(char*,unsigned int ,unsigned int)));
    connect(processing,SIGNAL(outputImage2(char*,unsigned int,unsigned int)),ui->light_caliberation_section,SLOT(updateHistogramImage(char*,unsigned int ,unsigned int)));
    connect(machine,SIGNAL(cameraFrameRecieved(char*,unsigned int,unsigned int)),processing,SLOT(inputImage(char*,unsigned int ,unsigned int)));

    connect(processing,SIGNAL(outputImage(char*,unsigned int,unsigned int)),machine,SLOT(updateFrameColor(char*,unsigned int ,unsigned int)));
    //connect(scanner,SIGNAL(set_image(char*,unsigned int,unsigned int)),this,SLOT(update_camera_image(char*,unsigned int ,unsigned int)));
    //connect(marker,SIGNAL(set_image(char*,unsigned int,unsigned int)),this,SLOT(update_camera_image(char*,unsigned int ,unsigned int)));

    //start the hardware thread
    hardwareInteractionThread->start();

    //gui is shown when machine is initialised
    connect(machine,SIGNAL(initMachine()),this,SLOT(show()));

    QMetaObject::invokeMethod(machine, "LineLaser", Qt::QueuedConnection,Q_ARG(int, 0),Q_ARG(ActionType,_DEFAULT_FROM_CONFIG));
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

/*
 *
*/
void MainWindow::setConfigSettings()
{
    QString filename = QCoreApplication::applicationDirPath()+"/../application_settings_.json";

    if(!QFileInfo(QCoreApplication::applicationDirPath()+"/../application_settings_.json").exists())
        QFile::copy(":/Config/application_setting.json",QCoreApplication::applicationDirPath()+ "/../application_settings_.json");

    application_settings = new _ConfigControlEntity(_Tools::ReadJsonFromQrc(filename));

    if(!QFileInfo(application_settings->getChildEntity("Paths").getStringEntity("MACHINE_CONFIG_FILE")).exists())
    {
        application_settings->getChildEntity("Paths").getStringEntity("MACHINE_CONFIG_FILE") = QCoreApplication::applicationDirPath() + "/../configuration.json";
        QFile::copy(":/Config/configuration.json",application_settings->getChildEntity("Paths").getStringEntity("MACHINE_CONFIG_FILE"));
        application_settings->getChildEntity("Paths").getStringEntity("SCAN_STORE_PATH") = QCoreApplication::applicationDirPath();
        _Tools::WriteJsonToFile(filename,application_settings->toJson());
    }

    _CaliberationSection::setApplicationSettings(application_settings);
}

/*
 *
*/
void MainWindow::openSettingsDialog()
{
    QString filename = QCoreApplication::applicationDirPath()+"/../application_settings_.json";
    //QString filename = "C:/Users/Developer/Documents/build-DiamondPal-Desktop_Qt_5_12_2_MSVC2017_64bit-Debug/application_settings_.json";
    delete application_settings;
    application_settings= new _ConfigControlEntity(_Tools::ReadJsonFromQrc(filename));
    _AppplicationSettingsDialog app_settings_dialog(*application_settings);
    app_settings_dialog.setModal(true);
    app_settings_dialog.exec();

    _Tools::WriteJsonToFile(filename,application_settings->toJson());

    _CaliberationSection::setApplicationSettings(application_settings);
}
