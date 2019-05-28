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
            UI/_caliberationsection.h \
            _glwidget.h \
            _renderer.h \
            _shader.h \
            _sceneentity.h \
            _scene.h \
            _camera.h \
            _assetloader.h \
            _texture.h\
            _framebuffer.h \
            _physics.h \
            _tools.h \
            IO/_hwdcamera.h \
            IO/_hardwareserial.h \
            IO/_machine.h \
            IO/_dc_1394_camera.h \
            IO/_avt_camera.h \
            IO/_configcontrolentity.h \
            IO/_picolo_camera.h \
            Modules/_marker.h \
            Modules/_scanner.h \
            Modules/_processing.h \
            Compute/_gpu_compute.h \
            Compute/_cpu_compute.h \
            UI/_tabwidget.h \
            UI/_tabbar.h \
            UI/_lightcaliberationsection.h \
            UI/_sliderbox.h \
            UI/_machineselectionsection.h \
            UI/_scancaliberationsection.h \
            UI/_appplicationsettingsdialog.h \
            UI/_pathsettingform.h \
            UI/_stagecaliberationsection.h


SOURCES += main.cpp\
           UI/_caliberationsection.cpp \
           mainwindow.cpp \
           _glwidget.cpp \
           _renderer.cpp \
           _shader.cpp \
           _sceneentity.cpp \
           _scene.cpp \
           _camera.cpp \
           _assetloader.cpp \
           _texture.cpp\
           _framebuffer.cpp \
           _physics.cpp \
           _tools.cpp \
           IO/_hwdcamera.cpp \
           IO/_hardwareserial.cpp \
           IO/_machine.cpp \
           IO/_dc_1394_camera.cpp \
           IO/_avt_camera.cpp \
           IO/_configcontrolentity.cpp \
           IO/_picolo_camera.cpp \
           Modules/_marker.cpp \
           Modules/_scanner.cpp \
           Modules/_processing.cpp \
           Compute/_gpu_compute.cpp \
           Compute/_cpu_compute.cpp \
           UI/_tabwidget.cpp \
           UI/_tabbar.cpp \
           UI/_lightcaliberationsection.cpp \
           UI/_sliderbox.cpp \
           UI/_machineselectionsection.cpp \
           UI/_scancaliberationsection.cpp \
           UI/_appplicationsettingsdialog.cpp \
           UI/_pathsettingform.cpp \
           UI/_stagecaliberationsection.cpp

FORMS    += mainwindow.ui \
            UI/_sliderbox.ui \
            UI/_lightcaliberationsection.ui \
            UI/_machineselectionsection.ui \
            UI/_scancaliberationsection.ui \
            UI/_appplicationsettingsdialog.ui \
            UI/_pathsettingform.ui \
            UI/_stagecaliberationsection.ui

RESOURCES += shaders.qrc \
             textures.qrc \
             models.qrc \
             data.qrc

INCLUDEPATH += $$PWD/Dependancies/common/glm-0.9.9.3/glm/
DEPENDPATH += $$PWD/Dependancies/common/glm-0.9.9.3/glm/

DEFINES += MAX_FRAME_WIDTH=1360
DEFINES += MAX_FRAME_HEIGHT=1024

#DEFINES += PICOLO_CAMERA=true
#DEFINES += AVT_CAMERA=true
DEFINES += NO_CAMERA=true

linux-g++ { #check if platform is linux
message(Linux)
DEFINES += PLATFORM_LINUX=true
INCLUDEPATH += /usr/local/include/dc1394
DEPENDPATH += /usr/local/include/dc1394
LIBS += -L/usr/local/lib -ldc1394
LIBS += -lOpenGL32
}

win32 { #check if platform is windows
DEFINES += PLATFORM_WIN=true
INCLUDEPATH += $$PWD/Dependancies/win/FireGrab/Lib
DEPENDPATH += $$PWD/Dependancies/win/FireGrab/Lib
LIBS +=
}

DISTFILES +=
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/Dependancies/win/FireGrab/Lib/ -lFGCamera
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/Dependancies/win/FireGrab/Lib/ -lFGCamera


win32: LIBS += -L$$PWD/Dependancies/win/Picolo/Lib/amd64/ -lMultiCam

INCLUDEPATH += $$PWD/Dependancies/win/Picolo/Include
DEPENDPATH += $$PWD/Dependancies/win/Picolo/Include
