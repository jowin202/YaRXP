#include "rpgmapinfocontroller.h"
#include "rpgdb.h"

RPGMapInfoController::RPGMapInfoController(RPGDB *db)
    : QObject()
{
    this->db = db;
}

bool RPGMapInfoController::id_is_valid(int id)
{
    return db->get_mapinfos()->object().contains(QString::number(id));
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
