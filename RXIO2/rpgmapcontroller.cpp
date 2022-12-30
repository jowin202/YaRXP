#include "rpgmapcontroller.h"
#include "rpgdb.h"
#include "autotileset.h"

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
        QJsonObject tileset = this->db->get_tileset_by_id(this->get_jsonvalue("@tileset_id").toInt());
        this->tileset_image = QImage(this->db->project_dir + "Graphics" + QDir::separator() + "Tilesets" + QDir::separator() + tileset.value("@tileset_name").toString());

        QJsonArray autotile_names = tileset.value("@autotile_names").toArray();
        for (int i = 0; i < 7; i++)
        {
            this->autotiles[i] = Autotileset(QImage(this->db->project_dir + "Graphics" + QDir::separator() + "Autotiles" + QDir::separator() + autotile_names.at(i).toString())).get_full_tileset();
        }
    }
}

QImage RPGMapController::get_tile_from_pos(QPoint pos, int layer)
{
    if (doc == 0 || this->tileset_image.isNull()) return QImage();
    QJsonArray data = this->get_jsonvalue("@data").toObject().value("values").toArray();
    int tile = data.at(this->array_position(pos,layer)).toInt();

    if (tile > 0 && tile < 0x0180)
    {
        //autotiles
        int autotileset_num = tile / 48 - 1;
        int autotile_num = tile % 48;
        return QImage(autotiles[autotileset_num].copy((autotile_num % 8)*32, (autotile_num / 8)*32,32,32));
    }
    //else

    tile -= 0x0180; //remove offset
    int x = tile % 8;
    int y = tile / 8;

    if (x < 0 || y < 0 || this->tileset_image.isNull())
        return QImage();
    return this->tileset_image.copy(32*x, 32*y, 32, 32);

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

