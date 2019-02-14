

#-------------------------------------------------
#
# Project DiamondPal
#
#-------------------------------------------------

QT       += core gui
QT       +=opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DiamondPal
TEMPLATE = app

SOURCES += main.cpp\
           mainwindow.cpp \
           _glwidget.cpp \
    _renderer.cpp \
    tools.cpp

HEADERS  += mainwindow.h \
            _glwidget.h \
    _renderer.h \
    tools.h

FORMS    += mainwindow.ui

RESOURCES += \
    shaders.qrc \
    textures.qrc

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

