#include "rpgmapcontroller.h"
#include "rpgdb.h"

RPGMapController::RPGMapController(QObject *parent)
    : QObject{parent}
{

}

void RPGMapController::setMap(int id)
{
    QJsonDocument *doc;
    if ( (doc = db->get_mapfile_by_id(id)) != 0)
    {
        this->doc = doc;
    }
}

QJsonObject RPGMapController::event_on_pos(QPoint pos)
{
    if (doc == 0) return QJsonObject();
    QJsonObject events = this->get_jsonvalue("@events").toObject();
    foreach (const QString &key, events.keys())
    {
        QJsonObject obj = events.value(key).toObject();
        if (obj.value("@x").toInt() == pos.x() && obj.value("@y").toInt() == pos.y())
            return obj;
    }

    return QJsonObject();
}

QJsonObject RPGMapController::event_by_id(int id)
{
    if (doc == 0) return QJsonObject();
    return this->get_jsonvalue("@events").toObject().value(QString::number(id)).toObject();
}

void RPGMapController::set_event_by_id(int id, QJsonObject event)
{
    if (doc == 0) return;
    QJsonObject map_object = this->doc->object();
    QJsonObject events = map_object.value("@events").toObject();
    events.insert(QString::number(id), event);
    map_object.insert("@events", events);
    doc->setObject(map_object);
}

int RPGMapController::get_height()
{
    return this->get_jsonvalue("@height").toInt();
}

int RPGMapController::get_width()
{
    return this->get_jsonvalue("@width").toInt();
}

void RPGMapController::set_height(int tiles)
{
    this->set_jsonvalue("@height", tiles);
    //TODO
}

void RPGMapController::set_width(int tiles)
{
    this->set_jsonvalue("@width", tiles);
    //TODO
}

QJsonValue RPGMapController::get_jsonvalue(QString key)
{
    if (doc == 0) return -1;
    return this->doc->object().value(key);
}

void RPGMapController::set_jsonvalue(QString key, QJsonValue value)
{
    if (doc == 0) return;
    QJsonObject map_object = this->doc->object();
    map_object.insert(key, value);
    doc->setObject(map_object);
}

