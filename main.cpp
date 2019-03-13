
#include <QApplication>
#include <QMainWindow>
#include <QSurfaceFormat>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QOpenGLFunctions>

#include <QGLFormat>

#include "mainwindow.h"
#include "_glwidget.h"

int main(int argc, char *argv[])
{
    QGLFormat glFormat;
    glFormat.setVersion( 4, 0 );
    glFormat.setProfile( QGLFormat::CoreProfile ); // Requires >=Qt-4.8.0
    glFormat.setSampleBuffers( true );

    QApplication a(argc, argv);
    MainWindow w;
	w.show();

    return a.exec();
}
