#ifndef _SCANNER_H
#define _SCANNER_H

#include <IO/_machine.h>
#include <QObject>
#include <QOpenGLExtraFunctions>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QWidget>


class _Scanner : public QObject , protected QOpenGLExtraFunctions
{
    Q_OBJECT
public:
    explicit _Scanner(QObject *parent = nullptr);
    explicit _Scanner(_Machine* global_machine,QObject *parent = nullptr);

public slots:
    void init();
    void scan_save_images();
    void scan_generate_model();
signals:
    void set_image(char* img,unsigned int w,unsigned int h);


public:
    _Machine *machine;
    QOpenGLContext *context = nullptr;
    QOpenGLExtraFunctions *gl = nullptr;
    QOffscreenSurface *surface = nullptr;

    unsigned int framebuffer;
    unsigned int renderbuffer;
    unsigned int framebuffer_texture;
    unsigned int framebuffer_Width=1360,framebuffer_Height=1024;
};

#endif // _SCANNER_H
