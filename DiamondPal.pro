

#-------------------------------------------------
#
# Project DiamondPal
#
#-------------------------------------------------

QT       += core gui
QT       += opengl
QT       += widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DiamondPal
TEMPLATE = app

SOURCES += main.cpp\
            mainwindow.cpp \
            _glwidget.cpp \
            _renderer.cpp \
            tools.cpp \
            _shader.cpp \
            _sceneentity.cpp \
    _scene.cpp \
    _camera.cpp

HEADERS  += mainwindow.h \
            _glwidget.h \
            _renderer.h \
            tools.h \
            _shader.h \
            _sceneentity.h \
    _scene.h \
    _camera.h

FORMS    += mainwindow.ui

RESOURCES += \
    shaders.qrc \
    textures.qrc

linux-g++ { #check if platform is linux
message(Linux)
INCLUDEPATH += $$PWD/../glm-0.9.9.3/glm
DEPENDPATH += $$PWD/../glm-0.9.9.3/glm
LIBS += 
}

win32 { #check if platform is windows
INCLUDEPATH += $$PWD/../glm-0.9.9.3/glm
DEPENDPATH += $$PWD/../glm-0.9.9.3/glm
LIBS +=
}

