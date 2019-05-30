#include "_tools.h"
#include <QFile>
#include <QString>


//Commonly used Functions
QElapsedTimer timer;
unsigned int frameCounter;
_Tools::_Tools()
{
    timer.start();
    frameCounter = 0;
}
/*
 * Function:ReadStringFromQrc(QString Filename)
 * returns the string litral for a qrc file path
 * Created: 2_03_2019
*/
QString _Tools::ReadStringFromQrc(QString Filename)
{
    // Read text files from .qrc file
    // return as QString
    QString mText = "";
    QFile mFile(Filename);

    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "could not open file for read";
    }
    QTextStream in(&mFile);
    mText = in.readAll();

    mFile.close();
    return mText;
}
/*
 * Function : retunrnMaxPoint(QVector2D mousepos)
 * this funtion takes in the updated mousposition and
 * returns the max point wrt to the first point passed
 * into this function.
 * Created: 11_04_2019
*/
QVector2D _Tools::retunrnMaxPoint(QVector2D mousepos)
{
    QVector2D max;
    if(mousepos.x() > max.x())
    {
        max.setX(max.x());
    }
    if(mousepos.y() > max.y())
    {
        max.setY(max.y());
    }
    return max;
}

void _Tools::Debugmatrix4x4(glm::mat4x4 mat4)
{
    for(int i = 0 ; i < 4 ; i++)
    {
        qInfo() << mat4[i][0] <<" "<< mat4[i][1] <<" "<< mat4[i][2] <<" "<< mat4[i][3];
    }
    qInfo() << "----------------------------------------------------";
}


void _Tools::printFrameRate()
{
    frameCounter++;
    int t = timer.elapsed()/1000;
    int interval = 1;
//    qDebug() << t;
    if(t > interval)
    {
        qInfo() <<"Framerate:"<< frameCounter / interval;
        frameCounter = 0;
        timer.restart();
    }
}

float _Tools::getRandomNumberfromRange(float min, float max)
{
//    std::srand(2);
    return min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max-min)));
}
