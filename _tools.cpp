#include "_tools.h"
#include <QFile>
#include <QString>
#include <QDebug>

//Commonly used Functions

_Tools::_Tools()
{

}

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
