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
    static void SaveImageToPgm(char *img, unsigned int rttWidth, unsigned int rttHeight, QString filename);
};


#endif // TOOLS_H
