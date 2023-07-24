#include "godotexporter.h"
#include "../RXIO2/rpgdb.h"
#include "../RXIO2/rpgmapinfocontroller.h"

GodotExporter::GodotExporter(RPGDB *db, QString path)
{
    this->db = db;
    this->path = path;
    if (!path.endsWith(QDir::separator()))
        this->path = this->path + QDir::separator();

    this->create_dir_structure();
}



void GodotExporter::create_dir_structure()
{
    QDir().mkdir(path + "world");

    RPGMapInfoController mic(db);

    QJsonArray toplevel_maps = mic.get_toplevel_maps();
    QJsonArray non_toplevel_maps = mic.get_child_maps();

    for (int i = 0; i < toplevel_maps.count(); i++)
    {
        QString map_name = toplevel_maps.at(i).toObject().value("@name").toString();
        int id = toplevel_maps.at(i).toObject().value("id").toInt();
        QDir().mkdir(path + "world" + QDir::separator() + map_name); //if exists

        QString path_to_map = path + "world" + QDir::separator() + map_name + QDir::separator();
        this->map_id_to_dir.insert(id,path_to_map);

        this->write_to_file(id, map_name);
    }


    while(non_toplevel_maps.count() > 0)
    {
        int cnt1 = non_toplevel_maps.count();
        for (int i = 0; i < non_toplevel_maps.count(); i++)
        {
            int parent_id = non_toplevel_maps.at(i).toObject().value("@parent_id").toInt();
            if (this->map_id_to_dir.contains(parent_id))
            {
                QString map_name = non_toplevel_maps.at(i).toObject().value("@name").toString();
                int id = non_toplevel_maps.at(i).toObject().value("id").toInt();




                QString path_to_map = this->map_id_to_dir.value(parent_id) + map_name + QDir::separator();
                this->map_id_to_dir.insert(id,path_to_map);

                QDir().mkdir(path_to_map); //if exists

                this->write_to_file(id, map_name);

                non_toplevel_maps.removeAt(i--); //do this at the end
            }
        }
        int cnt2 = non_toplevel_maps.count();
        if (cnt1 == cnt2)
            break; //break here if parent_id loop
    }
}

void GodotExporter::write_to_file(int id, QString name)
{
    QFile f(this->map_id_to_dir.value(id) + QString("Map%1.tscn").arg(id,3,10,QChar('0')));
    f.open(QIODevice::WriteOnly);


    QJsonObject map = db->get_mapfile_by_id(id)->object();
    QJsonObject tileset = db->get_tileset_by_id(map.value("@tileset_id").toInt());

    int max_y = tileset.value("@passages").toObject().value("values").toArray().count()-384;
    int height = map.value("@height").toInt();
    int width = map.value("@width").toInt();
    QJsonArray tiles = map.value("@data").toObject().value("values").toArray();
    QJsonArray passages = tileset.value("@passages").toObject().value("values").toArray();
    QJsonArray priorities = tileset.value("@priorities").toObject().value("values").toArray();

    QString scene_uid = this->random_id();
    QString Texture2D_uid = this->random_id();
    QString Texture2D_id = this->random_id();
    QString TileSetAtlasSource_id = this->random_id();
    QString Tileset_id = this->random_id();

    f.write(QString("[gd_scene load_steps=4 format=3 uid=\"uid://" + scene_uid + "\"]\n\n").toUtf8());

    f.write(QString("[ext_resource type=\"Texture2D\" uid=\"uid://" + Texture2D_uid + "\" path=\"res://Graphics/Tilesets/" +
                    tileset.value("@tileset_name").toString() +".png\" id=\"" + Texture2D_id + "\"]\n\n").toUtf8());

    f.write(QString("[sub_resource type=\"TileSetAtlasSource\" id=\"" + TileSetAtlasSource_id +"\"]\n").toUtf8());
    f.write(QString("texture = ExtResource(\"" + Texture2D_id + "\")\n").toUtf8());
    f.write(QString("texture_region_size = Vector2i(32, 32)\n").toUtf8());
    f.write(QString("use_texture_padding = false\n").toUtf8());


    for (int y = 0; y < max_y; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            f.write(QString("%1:%2/0 = 0\n").arg(x).arg(y).toUtf8());
            f.write(QString("%1:%2/0/physics_layer_0/linear_velocity = Vector2(0, 0)\n").arg(x).arg(y).toUtf8());
            f.write(QString("%1:%2/0/physics_layer_0/angular_velocity = 0.0\n").arg(x).arg(y).toUtf8());
            if ((passages.at(384 + 8*y + x).toInt() & 0xF) > 0)
                f.write(QString("%1:%2/0/physics_layer_0/polygon_0/points = PackedVector2Array(-16, -16, 16, -16, 16, 16, -16, 16)\n").arg(x).arg(y).toUtf8());
            if (priorities.at(384 + 8*y + x).toInt() > 0)
                f.write(QString("%1:%2/0/z_index = %3\n").arg(x).arg(y).arg(priorities.at(384 + 8*y + x).toInt()).toUtf8());
        }
    }


    f.write(QString("\n[sub_resource type=\"TileSet\" id=\"" + Tileset_id + "\"]\n").toUtf8());
    f.write(QString("tile_size = Vector2i(32, 32)\n").toUtf8());
    f.write(QString("physics_layer_0/collision_layer = 1\n").toUtf8());
    f.write(QString("sources/0 = SubResource(\"" + TileSetAtlasSource_id + "\")\n\n").toUtf8());


    f.write(QString("[node name=\"" + name + "\" type=\"TileMap\"]\n").toUtf8());
    f.write(QString("position = Vector2(0, 0)\n").toUtf8());
    f.write(QString("tile_set = SubResource(\"" + Tileset_id + "\")\n").toUtf8());
    f.write(QString("format = 2\n").toUtf8());
    f.write(QString("layer_0/name = \"Ground\"\n").toUtf8());


    QString tiledata;

    tiledata = "layer_0/tile_data = PackedInt32Array(";
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int tile_id = tiles.at(array_position(x,y,0,width,height)).toInt();
            if (tile_id < 384) continue;

            tile_id -= 384;

            tiledata += QString("%1, %2, %3, ").arg(x+65536*y).arg(65536*(tile_id%8)).arg(tile_id/8);
        }
    }
    tiledata.chop(2);
    tiledata += ")\n";
    f.write(tiledata.toUtf8());

    f.write(QString("layer_1/name = \"Middle\"\n").toUtf8());
    f.write(QString("layer_1/enabled = true\n").toUtf8());
    f.write(QString("layer_1/modulate = Color(1, 1, 1, 1)\n").toUtf8());
    f.write(QString("layer_1/y_sort_enabled = false\n").toUtf8());
    f.write(QString("layer_1/y_sort_origin = 0\n").toUtf8());
    f.write(QString("layer_1/z_index = 0\n").toUtf8());

    tiledata = "layer_1/tile_data = PackedInt32Array(";
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int tile_id = tiles.at(array_position(x,y,1,width,height)).toInt();
            if (tile_id < 384) continue;

            tile_id -= 384;

            tiledata += QString("%1, %2, %3, ").arg(x+65536*y).arg(65536*(tile_id%8)).arg(tile_id/8);
        }
    }
    tiledata.chop(2);
    tiledata += ")\n";
    f.write(tiledata.toUtf8());

    f.write(QString("layer_2/name = \"Top\"\n").toUtf8());
    f.write(QString("layer_2/enabled = true\n").toUtf8());
    f.write(QString("layer_2/modulate = Color(1, 1, 1, 1)\n").toUtf8());
    f.write(QString("layer_2/y_sort_enabled = false\n").toUtf8());
    f.write(QString("layer_2/y_sort_origin = 0\n").toUtf8());
    f.write(QString("layer_2/z_index = 0\n").toUtf8());

    tiledata = "layer_2/tile_data = PackedInt32Array(";
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int tile_id = tiles.at(array_position(x,y,2,width,height)).toInt();
            if (tile_id < 384) continue;

            tile_id -= 384;

            tiledata += QString("%1, %2, %3, ").arg(x+65536*y).arg(65536*(tile_id%8)).arg(tile_id/8);
        }
    }
    tiledata.chop(2);
    tiledata += ")\n";
    f.write(tiledata.toUtf8());


    f.close();
}

QString GodotExporter::random_id()
{
    QCryptographicHash hash(QCryptographicHash::Sha3_512);
    hash.addData(QString::number(QDateTime::currentMSecsSinceEpoch() + (counter++)).toUtf8());
    return QString(hash.result().toBase64().replace("+","").replace("/", "").mid(0,16));
}
