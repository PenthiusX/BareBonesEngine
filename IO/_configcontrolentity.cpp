#include "_configcontrolentity.h"

//30_03_2019
_ConfigControlEntity::_ConfigControlEntity()
{

}

_ConfigControlEntity::_ConfigControlEntity(QJsonObject cnfg)
{

}

bool _ConfigControlEntity::loadFromJson(QJsonObject cnfg)
{
    return true;
}

QJsonObject _ConfigControlEntity::toJson()
{
    QJsonObject ret;
    return ret;
}
