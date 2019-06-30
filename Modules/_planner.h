#ifndef _PLANNER_H
#define _PLANNER_H

#include <IO/_machine.h>
#include <QObject>
#include <QOpenGLExtraFunctions>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QWidget>
#include <Compute/_cpu_compute.h>
#include <_sceneentity.h>

class _Planner : public QObject, protected QOpenGLExtraFunctions
{
    Q_OBJECT

    public:
        explicit _Planner(QObject *parent = nullptr);
        //explicit _Planner(_Machine* global_machine,* proc,QObject *parent = nullptr);

    public slots:
        void init();
        void startPlanning(_Tools::ModelData stone_model,_Tools::ModelData diamond_model,glm::mat4x4 relative_transform);

    signals:
        void diamondRelativeTransformOut(glm::mat4x4 relative_transform);

    public:
        //_GPU_Compute *gpu_compute = nullptr;
        _Cpu_Compute *cpu_compute = nullptr;
};

#endif // _PLANNER_H
