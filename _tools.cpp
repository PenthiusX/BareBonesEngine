#include "_tools.h"
#include <QFile>
#include <QString>
#include <QDebug>

//Commonly used Functions

_Tools::_Tools()
{

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
/*
 * Function : translateToPoint(QVector3D pointLocation, float stepDistance)
 * this function takes in two points and generates the next point to translate
 * towards the point to focus on.
 * Created: 11_04_2019
*/
QVector3D _Tools::translateToPoint(QVector3D pointLocation, float stepDistance)
{
    //implementation pending

    QVector3D points;
    return points;
}

