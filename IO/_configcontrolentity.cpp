#include "_configcontrolentity.h"
#include <QDebug>
//30_03_2019

/*class: _ConfigControlEntity
 * to handle command,data,configuration from json file
 * this class contains config and their child configs in tree structure
 * also float,QString,Bool,array_of_float
 *
 *  ****** all child accessing operations should be done by referances ******
 *  ****** so that when parent config is exported it will have the updated values ******
 *
 *  eg:
 *       QString& cmd = configcontrolentity["Hardware"]["Commands"]["RS232"].getStringEntity("SET");
 *              ^
 *          referance
 *       cmd = "U";//so that internal value is updated
 *
 * created 30_03_2019
 * Author : Saurabh
*/


/* _ConfigControlEntity empty constructor
 * created 30_03_2019
*/
_ConfigControlEntity::_ConfigControlEntity()
{

}

/* _ConfigControlEntity constructor
 * stores the config and childs from json object
*/
_ConfigControlEntity::_ConfigControlEntity(QJsonObject cnfg)
{
    loadFromJson(cnfg);
}


/* Function: loadFromJson(QJsonObject cnfg)
 * stores the config and childs from json object
 * created 05_04_2019
*/
bool _ConfigControlEntity::loadFromJson(QJsonObject cnfg)
{
    foreach(const QString& key, cnfg.keys()) {
            QJsonValue value = cnfg.value(key);
            switch (value.type()) {
            case QJsonValue::Bool :
            {
               bool_child_entities[key] = value.toBool();
            }
                break;
            case QJsonValue::Double :
            {
               float_child_entities[key] = value.toDouble();
            }
                break;
            case QJsonValue::String :
            {
               string_child_entities[key] = value.toString();
            }
                break;
            case QJsonValue::Object :
            {
               child_entities[key] = _ConfigControlEntity(value.toObject());
            }
                break;
            case QJsonValue::Array :
            {
                std::vector<float> arr;
                QJsonArray json_array = value.toArray();
                for (int var = 0; var < json_array.size(); ++var) {
                    arr.push_back(json_array.at(var).toDouble());
                }
               array_child_entities[key] = arr;
            }
                break;
            }
            //child_entities[key] = _ConfigControlEntity(value.toObject());
            //qDebug() << "Key = " << key << ", Value = " << value ;//<< " obj" << value.toObject();
        }
    return true;
}


/* Function: toJson()
 * stores the config and childs from json object
 * created 05_04_2019
*/
QJsonObject _ConfigControlEntity::toJson()
{
    QJsonObject ret;
    for (auto const& child_entity : child_entities)
    {
        _ConfigControlEntity child_json = child_entity.second;
        ret.insert(child_entity.first,child_json.toJson());
    }
    for (auto const& child_entity : string_child_entities)
    {
        QString child_json = child_entity.second;
        ret.insert(child_entity.first,QJsonValue(child_json));
    }
    for (auto const& child_entity : bool_child_entities)
    {
        bool child_json = child_entity.second;
        ret.insert(child_entity.first,QJsonValue(child_json));
    }
    for (auto const& child_entity : float_child_entities)
    {
        float child_json = child_entity.second;
        ret.insert(child_entity.first,QJsonValue(child_json));
    }
    for (auto const& child_entity : array_child_entities)
    {
        std::vector<float> arr = child_entity.second;
        QJsonArray json_array;

        for (int var = 0; var < arr.size(); ++var) {
            json_array.push_back(QJsonValue(arr[var]));
        }
        ret.insert(child_entity.first,QJsonValue(json_array));
    }
    return ret;
}

/* Function: getChildEntity(QString key)
 * returns referance to child entity of this object of given key
 * created 05_04_2019
*/
_ConfigControlEntity &_ConfigControlEntity::getChildEntity(QString key)
{
    return child_entities[key];
}

/* Function: getFloatEntity(QString key)
 * returns referance to child float entity of this object of given key
 * created 12_04_2019
*/
float &_ConfigControlEntity::getFloatEntity(QString key)
{
    return float_child_entities[key];
}

/* Function: getStringEntity(QString key)
 * returns referance to child QString entity of this object of given key
 * created 12_04_2019
*/
QString &_ConfigControlEntity::getStringEntity(QString key)
{
    return string_child_entities[key];
}

/* Function: getStringEntity(QString key)
 * returns referance to child QString entity of this object of given key
 * created 12_04_2019
*/
std::vector<float> &_ConfigControlEntity::getArrayEntity(QString key)
{
    return array_child_entities[key];
}


/* operator[QString key]
 * returns referance to child entity of this object of given key
 * created 12_04_2019
*/
_ConfigControlEntity &_ConfigControlEntity::operator[](QString key)
{
    return child_entities[key];
}
