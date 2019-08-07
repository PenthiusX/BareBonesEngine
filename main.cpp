#include <QApplication>
#include <QMainWindow>
#include <QSurfaceFormat>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QOpenGLFunctions>
#include <QGLFormat>

#include <string>

#include "mainwindow.h"
#include "_glwidget.h"

#include <thread>


/* Debug Use For application output
//qInfo is qt5.5+ only.
qInfo() << "C++ Style Info Message";
qDebug() << "C++ Style Debug Message";
qWarning() << "C++ Style Warning Message";
qCritical() << "C++ Style Critical Error Message";
// qFatal does not have a C++ style method.
*/

void functionTtest(int i,QString s){
    qDebug() << "tfunction" << i << s;
}

void treadTest(){

    std::thread t1(functionTtest ,2,"whaut whut" );
    t1.detach();

    qDebug() << "fFunction";

//    if(t1.joinable())
//        t1.join();
}

int main(int argc, char *argv[])
{
    treadTest();

    QGLFormat glFormat;
    glFormat.setVersion( 4, 3);
    glFormat.setProfile( QGLFormat::CoreProfile ); // Requires >=Qt-4.8.0
    glFormat.setSampleBuffers( true );

    QApplication a(argc, argv);
    MainWindow w;
    //gui is shown when machine is initialised
    //w.show();

    return a.exec();
}
