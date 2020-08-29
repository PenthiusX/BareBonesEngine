#-------------------------------------------------
#
# Project DiamondPal
#
#-------------------------------------------------

QT += core gui multimedia serialport opengl
QT += opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
requires(qtConfig(combobox))

TARGET = DiamondPal
TEMPLATE = app

HEADERS  += \
    _material.h \
    mainwindow.h \
    _ObjLoader.h \
    _glwidget.h \
    _light.h \
    _renderer.h \
    _shader.h \
    _sceneentity.h \
    _scene.h \
    _camera.h \
    _assetloader.h \
    _texture.h \
    _tools.h \
    _framebuffer.h \
    _physics.h \
    _modelinfo.h \
    _text.h

SOURCES +=  \
    _material.cpp \
    main.cpp \
    mainwindow.cpp \
    _light.cpp \
    _glwidget.cpp \
    _renderer.cpp \
    _shader.cpp \
    _sceneentity.cpp \
    _scene.cpp \
    _camera.cpp \
    _assetloader.cpp \
    _texture.cpp \
    _tools.cpp \
    _framebuffer.cpp \
    _physics.cpp \
    _modelinfo.cpp \
    _text.cpp

FORMS    += mainwindow.ui

RESOURCES += \
    shaders.qrc \
    textures.qrc \
    models.qrc

win32 { #check if platform is windows
DEFINES += PLATFORM_WIN=true
INCLUDEPATH += $$PWD/Dependancies/common/glm-0.9.9.3/glm/
DEPENDPATH += $$PWD/Dependancies/common/glm-0.9.9.3/glm/

LIBS += -lOpengl32

}
