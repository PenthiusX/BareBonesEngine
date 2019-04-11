#ifndef TOOLS_H
#define TOOLS_H

#include <QString>
#include <QVector3D>
#include <QVector2D>

/*
 * Class: _Tools
 * This class holds funtions that have no specific
 * class utility but are standalone tools in maths and physics for use
 * throughout the project.
 * (Note!: Phsyis and math implementations could be move out to a seperate class depending how this scales)
 * Created: 26_02_2019
 * Author: Saurabh , Aditya
*/
class _Tools
{
public:
    _Tools();
    static QString ReadStringFromQrc(QString Filename);
    static QVector2D retunrnMaxPoint(QVector2D mousepos);
    static QVector3D translateToPoint(QVector3D pointLocation,float stepDistance);
};


#endif // TOOLS_H
