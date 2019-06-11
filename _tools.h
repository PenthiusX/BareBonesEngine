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
#include <QJsonObject>


#include "_sceneentity.h"
/*
 * Class: _Tools
 * This class holds funtions that have no specific
 * class utility but are standalone tools in maths and physics for use
 * throughout the project.
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
    static void Debugmatrix4x4(glm::mat4x4 mat4);
    static float getRandomNumberfromRange(float min , float max);
    static void SaveImageToPgm(char *img, unsigned int rttWidth, unsigned int rttHeight, QString filename);
    static QJsonObject ReadJsonFromQrc(QString Filename);
    static bool WriteJsonToFile(QString Filename,QJsonObject jsonObject);
    //Debug use
    static void printFrameRate(int intervalValue);
    static void printAtFixedIntevals(int secs,QString s);

    static std::vector<int> GenerateFactors(int n);
    static glm::vec3 NormalizeSum(glm::vec3 in);
    static int argmax(glm::vec3 in);
    static glm::ivec3 GetGroupSize(int n, glm::vec3 ratio = glm::vec3(3,3,1));

    static QString getFolderFromDialog(QString aplicationRelativePath);
    static QJsonObject ReadJsonFromSystem(QString Filename);
    static bool WriteJsonToFileSystem(QString filename, QJsonObject config);
    static int indexFromPixelCordinates(glm::vec2 pixel_cord, glm::ivec2 resolution);
    static glm::ivec3 vertIndexesFromElementIndex(int i);
    static glm::vec3 vertsFromIndex(glm::ivec3 indexes);

};


#endif // TOOLS_H
