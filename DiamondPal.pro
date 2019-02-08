

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
           mainwindow.cpp \
           _glwidget.cpp

HEADERS  += mainwindow.h \
            _glwidget.h

FORMS    += mainwindow.ui


linux-g++ { #check if platform is linux
message(Linux)
INCLUDEPATH +=
DEPENDPATH += 
LIBS += 
}

win32 { #check if platform is windows
INCLUDEPATH += 
DEPENDPATH += 
LIBS += 
}
