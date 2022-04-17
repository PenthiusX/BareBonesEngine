#-------------------------------------------------
#
# Project DiamondPal
#
#-------------------------------------------------

QT += core gui multimedia serialport
QT += opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
requires(qtConfig(combobox))

TARGET = DiamondPal
TEMPLATE = app

HEADERS  += \
    _bufferobjects.h \
    _extobjLoader.h \
    _material.h \
    _skboxtest.h \
    a_animation.h \
    a_bone.h \
    mainwindow.h \
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
    _physics.h \
    _modelinfo.h \
    _text.h

SOURCES +=  \
    _bufferobjects.cpp \
    _material.cpp \
    a_animation.cpp \
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

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/Dependancies/common/assimp-5.2.3/buildMingw/lib/ -llibassimp.dll
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/Dependancies/common/assimp-5.2.3/buildMingw/lib/ -llibassimp.dll

INCLUDEPATH += $$PWD/Dependancies/common/assimp-5.2.3/buildMingw/include
DEPENDPATH += $$PWD/Dependancies/common/assimp-5.2.3/buildMingw/bin
