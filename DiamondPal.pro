

#-------------------------------------------------
#
# Project DiamondPal
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DiamondPal
TEMPLATE = app

SOURCES += main.cpp\
                mainwindow.cpp

HEADERS  += window.h \
    mainwindow.h

FORMS    += mainwindow.ui

linux-g++ { #check if platform is linux
message(Linux)
LIBS += -lGLEW
}

win32 { #check if platform is linux
message(Win)
}
