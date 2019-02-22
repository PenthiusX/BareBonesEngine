
#include <QApplication>
#include <QMainWindow>
#include <QSurfaceFormat>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QOpenGLFunctions>

#include "mainwindow.h"
#include "_glwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
	w.show();
	

    return a.exec();
}
