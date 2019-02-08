

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
           _glwidget.cpp \
    _renderer.cpp

HEADERS  += mainwindow.h \
            _glwidget.h \
    _renderer.h

FORMS    += mainwindow.ui

RESOURCES += \
    shaders.qrc \
    textures.qrc
