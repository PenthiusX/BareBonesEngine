#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include <QString>
#include <QDebug>
#include <QVector3D>
#include <QVector2D>
#include <QVector4D>
#include <cstdlib>  // For srand() and rand()
#include <vector>
#include <QElapsedTimer>
#include <glm/glm.hpp>


#include "_sceneentity.h"
/*
 * Class: _Tools
 * This class holds funtions that have no specific
 * class utility but are standalone tools in maths and physics for use
 * throughout the project.
 * Created: 26_02_2019
 * Author: Saurabh , Aditya
 *
 * (Note!: This whole class needs to be restructured Math implementations could be move out to a seperate class depending how this scales.
 * This can be used as a test base to test out structs and datastructure which can then be later implemented
 * as a sperate class).
*/
class _Tools
{
public:
    _Tools();
    static QString ReadStringFromQrc(QString Filename);
    static QVector2D retunrnMaxPoint(QVector2D mousepos);
    static void Debugmatrix4x4(glm::mat4x4 mat4);
    static float getRandomNumberfromRangeF(float min , float max);
    static QVector3D interpolateBetweenPoints(QVector3D p1, QVector3D p2,float factor);
    //Debug use
    static bool printFrameRate();
    static bool setIntervals(float intervalgap);
private:
//    static QElapsedTimer timer;
//    static uint frameCounter;
};


#endif // TOOLS_H
