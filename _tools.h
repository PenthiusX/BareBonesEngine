#ifndef TOOLS_H
#define TOOLS_H

#include <QString>
#include <QVector2D>


class _Tools
{
public:
    _Tools();
    static QString ReadStringFromQrc(QString Filename);
    static QVector2D retunrnMaxPoint(QVector2D mousepos);
};


#endif // TOOLS_H
