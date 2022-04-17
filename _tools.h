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
#include<glm/gtc/quaternion.hpp>

#include <assimp/quaternion.h>
#include <assimp/vector3.h>
#include <assimp/matrix4x4.h>

/*
 * Class: _Tools
 * This class holds funtions that have no specific
 * class utility but are standalone tools in maths and physics for use
 * throughout the project.
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
//    static QElapsedTimer timer;
//    static uint frameCounter;

    static inline glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from){
        glm::mat4 to;
        //the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
        to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
        to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
        to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
        to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
        return to;
    }
    static inline glm::vec3 GetGLMVec(const aiVector3D& vec){
        return glm::vec3(vec.x, vec.y, vec.z);
    }
    static inline glm::quat GetGLMQuat(const aiQuaternion& pOrientation){
        return glm::quat(pOrientation.w, pOrientation.x, pOrientation.y, pOrientation.z);
    }
};

#endif // TOOLS_H
