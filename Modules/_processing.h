#ifndef _PROCESSING_H
#define _PROCESSING_H

#include <QObject>
#include <QOpenGLExtraFunctions>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QWidget>
#include <Compute/_gpu_compute.h>
//22_04_2019
class _Processing : public QObject , protected QOpenGLExtraFunctions
{
    Q_OBJECT
public:
    explicit _Processing(QObject *parent = nullptr);

    bool makeCurrent();
    void doneCurrent();
    _GPU_Compute *gpu_compute = nullptr;

    void markLineLaser(char *img, unsigned int iwidth, unsigned int iheight);

signals:
    void outputImage(char *img, unsigned int iwidth, unsigned int iheight);
    void outputImage2(char *img, unsigned int iwidth, unsigned int iheight);
    void inputImageRecived(char *img, unsigned int iwidth, unsigned int iheight);

public slots:
    void inputImage(char *img, unsigned int iwidth, unsigned int iheight);
    void passThroughFrame(char *img, unsigned int iwidth, unsigned int iheight);
    void histogram(char *img, unsigned int iwidth, unsigned int iheight);
    void init();
    void setActiveProcess(const char *slot);

protected:

    QOpenGLContext *context = nullptr;
    QOffscreenSurface *surface = nullptr;
    char * colorFrame = nullptr;
    const char* active_slot = nullptr;
};

#endif // _PROCESSING_H
