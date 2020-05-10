//#include "_marker.h"
//#include <QDebug>
//#include <QThread>

//_Marker::_Marker()
//{

//}

//_Marker::_Marker(_Machine *global_machine)
//{
//    machine = global_machine;
//}

//void _Marker::mark_sine_wave()
//{
//    int last_height = 0,absolute_height=0;

//    //switch on the laser
//    machine->MarkingLaser(2);
//    qDebug() << "laser inside sine";
//    for(int t = 0;t<320;t++)
//    {
//        absolute_height = (int)(1000*sin((10*M_PI*t/320)));
//        machine->LaserHeightMotorDiff(absolute_height-last_height);
//        machine->TurnTableMotorDiff(50);
//        QThread::msleep(1);
//        last_height = absolute_height;
//    }
//    //switch off laser
//    machine->MarkingLaser(0);
//}

//void _Marker::motors_setup()
//{
//    //_Marker and _Machine is in same thread so we can call its slots directly
//    machine->TurnTableMotorDiff(1000);
//    machine->TurnTableMotorDiff(-1000);
//    machine->LaserFocusMotorDiff(1000);
//    machine->LaserFocusMotorDiff(-1000);
//    machine->LaserHeightMotorDiff(1000);
//    machine->LaserHeightMotorDiff(-1000);
//}
