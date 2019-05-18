#ifndef TOOLS_H
#define TOOLS_H

#include <QString>
#include <QDebug>
#include <QVector3D>
#include <QVector2D>
#include <cstdlib>  // For srand() and rand()
#include <vector>
#include <QElapsedTimer>
#include <glm/glm.hpp>

/*
 * Class: _Tools
 * This class holds funtions that have no specific
 * class utility but are standalone tools in maths and physics for use
 * throughout the project.
 * Created: 26_02_2019
 * Author: Saurabh , Aditya
 *
 * (Note!: Phsyis and math implementations could be move out to a seperate class depending how this scales.
 * This can be used as a test base to test out structs and datastructure which can then be later implemented
 * as a sperate class).
*/
class _Tools
{
public:
    _Tools();
    static QString ReadStringFromQrc(QString Filename);
    static QVector2D retunrnMaxPoint(QVector2D mousepos);
    static QVector3D translateToPoint(QVector3D pointLocation,float stepDistance);
    static void Debugmatrix4x4(glm::mat4x4 mat4);
    static void printFrameRate();
    static float getRandomNumberfromRange(float min , float max);

private:

};


#endif // TOOLS_H
