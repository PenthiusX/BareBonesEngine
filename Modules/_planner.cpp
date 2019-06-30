#include "_planner.h"

_Planner::_Planner(QObject *parent) :QObject(parent) , QOpenGLExtraFunctions()
{

}

/*
 * Function : init
 * should be called when thread starts inside hardware interaction thread
 * creates context and initializes _GPU_Compute object to d compute operations
 * sets defaults for hardware interation
 * created 29/06/2019
*/
void _Planner::init()
{
    cpu_compute = new _Cpu_Compute;

    initializeOpenGLFunctions();
}

void _Planner::startPlanning(_Tools::ModelData stone_model,_Tools::ModelData diamond_model,glm::mat4x4 relative_transform)
{
    //vertices of stone and planes of diomond
    //create planner interface
}
