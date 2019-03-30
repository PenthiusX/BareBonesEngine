#ifndef _CONFIGCONTROLENTITY_H
#define _CONFIGCONTROLENTITY_H

#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <map>

class _ConfigControlEntity
{
    enum EntityType
    {
        BOOL,
        VAL,
        OBJ,
        GROUP
    };

public:
    _ConfigControlEntity();
    _ConfigControlEntity(QJsonObject cnfg);

    bool loadFromJson(QJsonObject cnfg);
    QJsonObject toJson();

protected:

    bool state;
    int value;
    EntityType typ;
    std::map<QString,_ConfigControlEntity> child_entities;
};

#endif // _CONFIGCONTROLENTITY_H
