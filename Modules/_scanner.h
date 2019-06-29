#ifndef _SCANNER_H
#define _SCANNER_H

#include <IO/_machine.h>
#include <QObject>
#include <QOpenGLExtraFunctions>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QWidget>
#include <Compute/_cpu_compute.h>
#include <Modules/_processing.h>


class _Scanner : public QObject , protected QOpenGLExtraFunctions
{
    Q_OBJECT
public:
    explicit _Scanner(QObject *parent = nullptr);
    explicit _Scanner(_Machine* global_machine,_Processing* proc,QObject *parent = nullptr);

public slots:
    void init();
    void scanImages();
    void scanGenerateModel();
    void scanGenerateModelEdge();
    void scanGenerateModelWrap();
signals:
    void set_image(char* img,unsigned int w,unsigned int h);


public:
    _Machine *machine;
    QOpenGLContext *context = nullptr;
    QOffscreenSurface *surface = nullptr;
    _Cpu_Compute *cpu_compute = nullptr;
    _Processing *processing=nullptr;

};

#endif // _SCANNER_H
