#ifndef TOOLS_H
#define TOOLS_H

#include <QString>
#include <QJsonObject>

class _Tools
{
public:
    _Tools();
    static QString ReadStringFromQrc(QString Filename);
    static QJsonObject ReadJsonFromQrc(QString Filename);
};


#endif // TOOLS_H
