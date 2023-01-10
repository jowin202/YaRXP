#include "rpgmapcontroller.h"
#include "rpgdb.h"
#include "autotileset.h"

#include "fileopener.h"

RPGMapController::RPGMapController(QObject *parent)
    : QObject{parent}
{

}

void RPGMapController::setMap(int id, bool load_images)
{
    QJsonDocument *doc;
    if ( (doc = db->get_mapfile_by_id(id)) != 0)
    {
        this->doc = doc;
        QJsonObject tileset = this->db->get_tileset_by_id(this->get_jsonvalue("@tileset_id").toInt());
        if (load_images)
        {
            this->tileset_image = FileOpener(db->tileset_dir,tileset.value("@tileset_name").toString()).get_image();

            QJsonArray autotile_names = tileset.value("@autotile_names").toArray();
            for (int i = 0; i < 7; i++)
            {
                this->autotiles[i] = Autotileset(FileOpener(db->autotiles_dir, autotile_names.at(i).toString()).get_image()).get_full_tileset();
            }
        }
    }
    else
    {
        //file does not exist
        db->create_mapfile_with_id(id);
        this->doc = db->get_mapfile_by_id(id);
        //create this file automatically if it does not exist
    }
}

int RPGMapController::get_autotile_nn(QPoint pos, int layer)
{
    if (doc == 0 || this->tileset_image.isNull()) return 0;
    QJsonArray data = this->get_jsonvalue("@data").toObject().value("values").toArray();
    int id = data.at(this->array_position(pos,layer)).toInt();

    if (id >= 384) return 0;
    int autotile_id = id/48;

    int res = 0;

    if (pos.x() == 0 || pos.y() == 0 || data.at(array_position(QPoint(pos.x()-1,pos.y()-1),layer)).toInt()/48 == autotile_id)
        res |= 0x01;

    if (pos.x() == this->get_width()-1 || pos.y() == 0 || data.at(array_position(QPoint(pos.x()+1,pos.y()-1),layer)).toInt()/48 == autotile_id)
        res |= 0x02;

    if (pos.x() == this->get_width()-1 || pos.y() == this->get_height()-1 || data.at(array_position(QPoint(pos.x()+1,pos.y()+1),layer)).toInt()/48 == autotile_id)
        res |= 0x04;

    if (pos.x() == 0 || pos.y() == this->get_height()-1 || data.at(array_position(QPoint(pos.x()-1,pos.y()+1),layer)).toInt()/48 == autotile_id)
        res |= 0x08;

    if (pos.x() == 0 || data.at(array_position(QPoint(pos.x()-1,pos.y()),layer)).toInt()/48 == autotile_id)
        res |= 0x10;

    if (pos.y() == 0 || data.at(array_position(QPoint(pos.x(),pos.y()-1),layer)).toInt()/48 == autotile_id)
        res |= 0x20;

    if (pos.x() == this->get_width()-1 || data.at(array_position(QPoint(pos.x()+1,pos.y()),layer)).toInt()/48 == autotile_id)
        res |= 0x40;

    if (pos.y() == this->get_height()+1 || data.at(array_position(QPoint(pos.x(),pos.y()+1),layer)).toInt()/48 == autotile_id)
        res |= 0x80;

    return res;
}

QImage RPGMapController::get_tile_from_pos(QPoint pos, int layer)
{
    if (doc == 0 || this->tileset_image.isNull()) return QImage();
    QJsonArray data = this->get_jsonvalue("@data").toObject().value("values").toArray();
    int id = data.at(this->array_position(pos,layer)).toInt();
    return this->get_tile_from_id(id);
}

QImage RPGMapController::get_tile_from_id(int id)
{
    if (id > 0 && id < 0x0180)
    {
        //autotiles
        int autotileset_num = id / 48 - 1;
        int autotile_num = id % 48;
        return QImage(autotiles[autotileset_num].copy((autotile_num % 8)*32, (autotile_num / 8)*32,32,32));
    }
    //else

    id -= 0x0180; //remove offset
    int x = id % 8;
    int y = id / 8;

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

void RPGMapController::remove_event_by_id(int id)
{
        if (doc == 0) return;
        QJsonObject map_object = this->doc->object();
        QJsonObject events = map_object.value("@events").toObject();
        events.remove(QString::number(id));
        map_object.insert("@events", events);
        doc->setObject(map_object);
}

int RPGMapController::get_next_event_id()
{
    QJsonObject events = this->get_jsonvalue("@events").toObject();
    for (int i = 1; i < this->get_height()*this->get_width(); i++)
    {
        if (!events.contains(QString::number(i)))
            return i;
    }

    return -1;
}

void RPGMapController::move_event(QPoint from, QPoint to)
{
    QJsonObject event_on_from_pos = this->event_on_pos(from);
    QJsonObject event_on_to_pos = this->event_on_pos(to);
    if (!event_on_from_pos.contains("RXClass") || event_on_to_pos.contains("RXClass")) //from pos should contain event, to pos should not
        return;

    int id = event_on_from_pos.value("@id").toInt();
    event_on_from_pos.insert("@x", to.x());
    event_on_from_pos.insert("@y", to.y());
    this->set_event_by_id(id, event_on_from_pos);
}

QList<int> RPGMapController::get_elements_in_rectangle(QRect rect, int fromlayer, int tolayer)
{
    if (doc == 0) return QList<int>();
    QJsonArray values = this->doc->object().value("@data").toObject().value("values").toArray();
    QList<int> result;
    result.append(rect.width());
    result.append(rect.height());

    for (int l = fromlayer; l <= tolayer; l++)
    {
        for (int y = rect.topLeft().y(); y <= rect.bottomRight().y(); y++)
        {
            for (int x = rect.topLeft().x(); x <= rect.bottomRight().x(); x++)
            {
                int pos = array_position(QPoint(x,y),l);
                if (pos < values.count())
                    result.append(values[pos].toInt());
            }
        }
    }

    return result;
}

void RPGMapController::delete_elements_in_rectangle(QRect rect, int fromlayer, int tolayer)
{
    if (doc == 0) return;
    QJsonArray values = this->doc->object().value("@data").toObject().value("values").toArray();

    for (int l = fromlayer; l <= tolayer; l++)
    {
        for (int y = rect.topLeft().y(); y <= rect.bottomRight().y(); y++)
        {
            for (int x = rect.topLeft().x(); x <= rect.bottomRight().x(); x++)
            {
                int pos = array_position(QPoint(x,y),l);
                if (pos < values.count())
                    values[pos] = 0;
            }
        }
    }

    QJsonObject obj = this->doc->object();
    QJsonObject data_obj = this->doc->object().value("@data").toObject();
    data_obj.insert("values", values);
    obj.insert("@data", data_obj);
    this->doc->setObject(obj);
}

void RPGMapController::put_elements_from_list(QPoint pos, QPoint rel_pos, QList<int> list, int fromlayer, int tolayer)
{
    if (doc == 0) return;
    QJsonArray values = this->doc->object().value("@data").toObject().value("values").toArray();
    if (list.length() <= 2) return;

    int width = list.first(); list.pop_front();
    int height = list.first(); list.pop_front();
    int xr = rel_pos.x();
    int yr = rel_pos.y();


    for (int l = fromlayer; l <= tolayer; l++)
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                QPoint pos_on_map = pos + QPoint(x,y);
                if (pos_on_map.x() >= this->get_width() || pos_on_map.x() < 0) continue;
                if (pos_on_map.y() >= this->get_height() || pos_on_map.y() < 0) continue;
                int pos_in_array = array_position(pos_on_map,l);

                //thx to MW
                int xe = (x+xr)%width;
                int ye = (y+yr)%height;
                if (xe < 0) xe += width;
                if (ye < 0) ye += height;

                values[pos_in_array] = list[ xe + width*ye + width*height*(l-fromlayer)];
            }
        }
    }

    QJsonObject obj = this->doc->object();
    QJsonObject data_obj = this->doc->object().value("@data").toObject();
    data_obj.insert("values", values);
    obj.insert("@data", data_obj);
    this->doc->setObject(obj);


    //fix autotiles
    for (int l = fromlayer; l <= tolayer; l++)
    {
        for (int y = -1; y <= height; y++)
        {
            for (int x = -1; x <= width; x++)
            {
                QPoint pos_on_map = pos + QPoint(x,y);
                if (pos_on_map.x() >= this->get_width() || pos_on_map.x() < 0) continue;
                if (pos_on_map.y() >= this->get_height() || pos_on_map.y() < 0) continue;
                int pos_in_array = array_position(pos_on_map,l);

                if (values[pos_in_array].toInt() >= 384 || values[pos_in_array].toInt() < 48)
                    values[pos_in_array] = values[pos_in_array].toInt();
                else
                {
                    int autotile_correction = Autotileset().get_id_by_neighbourhood(this->get_autotile_nn(pos_on_map, l));
                    values[pos_in_array]= values[pos_in_array].toInt()/48 * 48 + autotile_correction;
                }
            }
        }
    }


    obj = this->doc->object();
    data_obj = this->doc->object().value("@data").toObject();
    data_obj.insert("values", values);
    obj.insert("@data", data_obj);
    this->doc->setObject(obj);
}

void RPGMapController::shift_map(int xtiles, int ytiles)
{
    if (doc == 0) return;
    xtiles = xtiles % this->get_width();
    ytiles = ytiles % this->get_height();

    QJsonObject events = this->get_jsonvalue("@events").toObject();
    foreach (const QString &key, events.keys())
    {
        QJsonObject event = events.value(key).toObject();
        if (xtiles != 0)
        {
            int new_width = event.value("@x").toInt() + xtiles;
            if (new_width < 0) new_width += this->get_width();
            event.insert("@x", new_width % this->get_width());
        }

        if (ytiles != 0)
        {
            int new_height = event.value("@y").toInt() + ytiles;
            if (new_height < 0) new_height += this->get_height();
            event.insert("@y", new_height % this->get_height());
        }

        this->set_event_by_id(event.value("@id").toInt(), event);
    }


    QJsonArray values = this->doc->object().value("@data").toObject().value("values").toArray();
    for (int layer = 0; layer <= 2; layer++)
    {
        for (int i = 0; i < xtiles; i++)
        {
            for (int y = 0; y < this->get_height(); y++)
            {
                int pos1 = array_position(QPoint(this->get_width()-1,y),layer);
                int pos2 = array_position(QPoint(0,y),layer);
                int tmp = values.at(pos1).toInt();
                values.removeAt(pos1);
                values.insert(pos2,tmp);
            }
        }
        for (int i = 0; i > xtiles; i--)
        {
            for (int y = 0; y < this->get_height(); y++)
            {
                int pos2 = array_position(QPoint(this->get_width()-1,y),layer);
                int pos1 = array_position(QPoint(0,y),layer);
                int tmp = values.at(pos1).toInt();
                values.removeAt(pos1);
                values.insert(pos2,tmp);
            }
        }

        for (int i = 0; i < ytiles; i++)
        {
            for (int x = 0; x < this->get_width(); x++)
            {
                int pos1 = array_position(QPoint(x,this->get_height()-1),layer);
                int pos2 = array_position(QPoint(x,0),layer);
                int tmp = values.at(pos1).toInt();
                values.removeAt(pos1);
                values.insert(pos2,tmp);
            }
        }
        for (int i = 0; i > ytiles; i--)
        {
            for (int x = this->get_width()-1; x >= 0; x--)
            {
                int pos2 = array_position(QPoint(x,this->get_height()-1),layer);
                int pos1 = array_position(QPoint(x,0),layer);
                int tmp = values.at(pos1).toInt();
                values.removeAt(pos1);
                values.insert(pos2,tmp);
            }
        }
    }
    QJsonObject obj = this->doc->object();
    QJsonObject data_obj = this->doc->object().value("@data").toObject();
    data_obj.insert("values", values);
    obj.insert("@data", data_obj);
    this->doc->setObject(obj);
}

void RPGMapController::set_size(int xtiles, int ytiles)
{
    if (doc == 0) return;

    QJsonObject events = this->get_jsonvalue("@events").toObject();
    foreach (const QString &key, events.keys())
    {
        QJsonObject event = events.value(key).toObject();
        if (event.value("@x").toInt() >= xtiles || event.value("@y").toInt() >= ytiles)
            this->remove_event_by_id(event.value("@id").toInt());
    }



    QJsonArray values = this->doc->object().value("@data").toObject().value("values").toArray();
    for (int layer = 2; layer >= 0; layer--)
    {
        for (int y = this->get_height()-1; y >= 0; y--)
        {
            for (int x = this->get_width(); x < xtiles; x++)
            {
                values.insert(array_position(QPoint(this->get_width(),y),layer),0);
            }
        }
        for (int y = this->get_height()-1; y >= 0; y--)
        {
            for (int x = xtiles; x < this->get_width(); x++)
            {
                values.removeAt(array_position(QPoint(xtiles,y),layer));
            }
        }
    }

    QJsonObject obj = this->doc->object();
    obj.insert("@width", xtiles);
    this->doc->setObject(obj);
    //xtiles has to be saved as width because array_position() and get_width() use it from there

    //new loop for y
    for (int layer = 2; layer >= 0; layer--)
    {
        for (int x = 0; x < this->get_width(); x++)
        {
            for (int y = this->get_height(); y < ytiles; y++)
            {
                values.insert(array_position(QPoint(x,this->get_height()),layer),0);
            }
        }
        for (int x = this->get_width()-1; x >= 0; x--)
        {
            for (int y = ytiles; y < this->get_height(); y++)
            {
                values.removeAt(array_position(QPoint(x,ytiles),layer));
            }
        }
    }

    obj = this->doc->object();
    QJsonObject data_obj = this->doc->object().value("@data").toObject();
    data_obj.insert("values", values);
    obj.insert("@data", data_obj);
    obj.insert("@height", ytiles);
    this->doc->setObject(obj);
}

int RPGMapController::get_height()
{
    return this->get_jsonvalue("@height").toInt();
}

int RPGMapController::get_width()
{
    return this->get_jsonvalue("@width").toInt();
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

