

#-------------------------------------------------
#
# Project DiamondPal
#
#-------------------------------------------------

QT       += core
QT       += gui
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
    _camera.cpp \
    _assetloader.cpp

HEADERS  += mainwindow.h \
            _glwidget.h \
            _renderer.h \
            tools.h \
            _shader.h \
            _sceneentity.h \
    _scene.h \
    _camera.h \
    _assetloader.h

FORMS    += mainwindow.ui

RESOURCES += \
    shaders.qrc \
    textures.qrc

linux-g++ { #check if platform is linux
message(Linux)
INCLUDEPATH += $$PWD/deps/linux/glm-0.9.9.3/glm
DEPENDPATH += $$PWD/deps/linux/glm-0.9.9.3/glm
LIBS += -lOpenGL32
}

win32 { #check if platform is windows
INCLUDEPATH += $$PWD/deps/windows/glm-0.9.9.3/glm \
                $$PWD/deps/windows/assimp-4.0.1/include
DEPENDPATH += $$PWD/deps/windows/glm-0.9.9.3/glm
LIBS +=
}

