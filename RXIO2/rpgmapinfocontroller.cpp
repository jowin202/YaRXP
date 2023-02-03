#include "rpgmapinfocontroller.h"
#include "rpgdb.h"
#include "factory.h"

RPGMapInfoController::RPGMapInfoController(RPGDB *db)
    : QObject()
{
    this->db = db;
}

bool RPGMapInfoController::id_is_valid(int id)
{
    return db->get_mapinfos()->object().contains(QString::number(id));
}

int RPGMapInfoController::get_lowest_available_id()
{
    for (int i = 1; i <= 999; i++)
    {
        if (!this->id_is_valid(i))
            return i;
    }
    return -1;
}

void RPGMapInfoController::set_expanded(int id, bool expanded)
{
    QJsonObject dict = db->get_mapinfos()->object();
    QJsonObject obj = dict.value(QString::number(id)).toObject();
    obj.insert("@expanded", expanded);
    dict.insert(QString::number(id), obj);
    db->get_mapinfos()->setObject(dict);
}

bool RPGMapInfoController::get_expanded(int id)
{
    return db->get_mapinfos()->object().value(QString::number(id)).toObject().value("@expanded").toBool();
}

void RPGMapInfoController::set_name(int id, QString name)
{
    QJsonObject dict = db->get_mapinfos()->object();
    QJsonObject obj = dict.value(QString::number(id)).toObject();
    obj.insert("@name", name);
    dict.insert(QString::number(id), obj);
    db->get_mapinfos()->setObject(dict);
}

QString RPGMapInfoController::get_name(int id)
{
    return db->get_mapinfos()->object().value(QString::number(id)).toObject().value("@name").toString();
}

void RPGMapInfoController::set_order(int id, int order)
{
    QJsonObject dict = db->get_mapinfos()->object();
    QJsonObject obj = dict.value(QString::number(id)).toObject();
    obj.insert("@order", order);
    dict.insert(QString::number(id), obj);
    db->get_mapinfos()->setObject(dict);
}

int RPGMapInfoController::get_order(int id)
{
    return db->get_mapinfos()->object().value(QString::number(id)).toObject().value("@order").toInt();
}

void RPGMapInfoController::set_parent(int id, int parent)
{
    QJsonObject dict = db->get_mapinfos()->object();
    QJsonObject obj = dict.value(QString::number(id)).toObject();
    obj.insert("@parent_id", parent);
    dict.insert(QString::number(id), obj);
    db->get_mapinfos()->setObject(dict);
}

int RPGMapInfoController::get_parent(int id)
{
    return db->get_mapinfos()->object().value(QString::number(id)).toObject().value("@parent_id").toInt();
}

QJsonArray RPGMapInfoController::get_toplevel_maps()
{
    QJsonArray array;
    foreach (const QString& key, db->get_mapinfos()->object().keys())
    {
        if (key == "RXClass") continue;

        QJsonObject mapinfo = db->get_mapinfos()->object().value(key).toObject();
        mapinfo.insert("id", key.toInt());
        if (mapinfo.value("@parent_id").toInt() > 0) continue;
        array.append(mapinfo);
    }
    return array;
}

QJsonArray RPGMapInfoController::get_child_maps()
{
    QJsonArray array;
    foreach (const QString& key, db->get_mapinfos()->object().keys())
    {
        if (key == "RXClass") continue;

        QJsonObject mapinfo = db->get_mapinfos()->object().value(key).toObject();
        mapinfo.insert("id", key.toInt());
        if (mapinfo.value("@parent_id").toInt() <= 0) continue;
        array.append(mapinfo);
    }
    return array;
}

bool RPGMapInfoController::create_map(int id)
{
    QJsonObject mapinfo = Factory().create_map_info(false, "", 999, 0,0,0);

    if (this->db->create_mapfile_with_id(id) == 0)
        return false; // if failed no mapinfo entry

    QJsonObject dict = db->get_mapinfos()->object();
    dict.insert(QString::number(id), mapinfo);
    db->get_mapinfos()->setObject(dict);

    return true;
}

bool RPGMapInfoController::create_map_from_object(int id, QJsonObject map)
{
    QJsonObject mapinfo = Factory().create_map_info(false, "", 999, 0,0,0);

    if (this->db->create_mapfile_from_object_with_id(id, map) == 0)
        return false; // if failed no mapinfo entry

    QJsonObject dict = db->get_mapinfos()->object();
    dict.insert(QString::number(id), mapinfo);
    db->get_mapinfos()->setObject(dict);

    return true;
}
