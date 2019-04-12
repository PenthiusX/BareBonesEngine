#ifndef _CONFIGCONTROLENTITY_H
#define _CONFIGCONTROLENTITY_H

#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <map>
#include <vector>

class _ConfigControlEntity
{
//    enum EntityType
//    {
//        BOOL,
//        VAL,
//        OBJ,
//        GROUP
//    };

public:
    _ConfigControlEntity();
    _ConfigControlEntity(QJsonObject cnfg);

    bool loadFromJson(QJsonObject cnfg);
    QJsonObject toJson();

    _ConfigControlEntity &operator[](QString key);

    _ConfigControlEntity &getChildEntity(QString key);
    float& getFloatEntity(QString key);
    QString& getStringEntity(QString key);
    std::vector<float>& getArrayEntity(QString key);

protected:
    //EntityType typ;
    std::map<QString,_ConfigControlEntity> child_entities;
    std::map<QString,bool> bool_child_entities;
    std::map<QString,float> float_child_entities;
    std::map<QString,std::vector<float>> array_child_entities;
    std::map<QString,QString> string_child_entities;
};

#endif // _CONFIGCONTROLENTITY_H
