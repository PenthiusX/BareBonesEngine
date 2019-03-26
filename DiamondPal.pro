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

HEADERS  += mainwindow.h \
            _glwidget.h \
            _renderer.h \
            tools.h \
            _shader.h \
            _sceneentity.h \
            _scene.h \
            _camera.h \
            _assetloader.h \
            _texture.h\
           IO/_hwdcamera.h \
           IO/_hardwareserial.h \
           IO/_machine.h \
           IO/_dc_1394_camera.h \
           IO/_avt_camera.h \
           Modules/_marker.h \
           Modules/_scanner.h

SOURCES += main.cpp\
            mainwindow.cpp \
            _glwidget.cpp \
            _renderer.cpp \
            tools.cpp \
            _shader.cpp \
            _sceneentity.cpp \
            _scene.cpp \
            _camera.cpp \
            _assetloader.cpp \
            _texture.cpp\
           IO/_hwdcamera.cpp \
           IO/_hardwareserial.cpp \
           IO/_machine.cpp \
           IO/_dc_1394_camera.cpp \
           IO/_avt_camera.cpp \
           Modules/_marker.cpp \
           Modules/_scanner.cpp

FORMS    += mainwindow.ui

RESOURCES += \
    shaders.qrc \
    textures.qrc \
    models.qrc


linux-g++ { #check if platform is linux
message(Linux)
DEFINES += PLATFORM_LINUX=true
INCLUDEPATH += /usr/local/include/dc1394
INCLUDEPATH += $$PWD/deps/linux/glm-0.9.9.3/glm/
DEPENDPATH += /usr/local/include/dc1394
DEPENDPATH += $$PWD/deps/linux/glm-0.9.9.3/glm/
LIBS += -L/usr/local/lib -ldc1394
LIBS += -lOpenGL32
}

win32 { #check if platform is windows
DEFINES += PLATFORM_WIN=true
INCLUDEPATH += $$PWD/Dependancies/win/FireGrab/Lib $$PWD/deps/windows/glm-0.9.9.3/glm/
DEPENDPATH += $$PWD/Dependancies/win/FireGrab/Lib $$PWD/deps/windows/glm-0.9.9.3/glm/
LIBS +=
}

DISTFILES +=
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/Dependancies/win/FireGrab/Lib/ -lFGCamera
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/Dependancies/win/FireGrab/Lib/ -lFGCamera

