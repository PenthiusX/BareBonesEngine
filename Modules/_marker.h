#ifndef _MARKER_H
#define _MARKER_H

#include <IO/_machine.h>
#include <QtMath>

class _Marker : public QObject
{
    Q_OBJECT
public:
    _Marker();
    _Marker(_Machine* global_machine);

public slots:
    void mark_sine_wave();
    void motors_setup();


private:
    _Machine *machine;
};

#endif // _MARKER_H
