#include "godotexporter.h"
#include "../RXIO2/rpgdb.h"
#include "../RXIO2/rpgmapinfocontroller.h"


#include "../RXIO2/autotileset.h"
#include "../RXIO2/fileopener.h"

GodotExporter::GodotExporter(RPGDB *db, QString path)
{
    this->db = db;
    this->path = path;
    if (!path.endsWith(QDir::separator()))
        this->path = this->path + QDir::separator();

    this->create_dir_structure();
}

QString GodotExporter::create_tileset(QString name)
{
    if (this->imported_tilesets.contains(name))
        return this->imported_tilesets.value(name);
    else
    {
        QString id = this->random_id();
        QFile f(FileOpener(db->tileset_dir, name).get_image_path());
        f.copy(path + "Graphics" + QDir::separator() + "Tilesets" + QDir::separator() + name + ".png");

        /*
        QFile f2(f.fileName() + ".import");
        f2.open(QIODevice::WriteOnly);
        f2.write(QString("[remap]\n\nimporter=\"texture\"\ntype=\"CompressedTexture2D\"uid=\"uid://" + id + "\"").toUtf8());
        f2.close();
        */

        this->imported_tilesets.insert(name, id);
        return id;
    }
}

QVariantList GodotExporter::create_autotile(QString name)
{
    if (this->imported_autotiles.contains(name))
        return this->imported_autotiles.value(name);
    else
    {
        QVariantList params;
        params << this->random_id() << this->random_id() << this->random_id(); //3 IDs

        QImage autotile_image = FileOpener(db->autotiles_dir, name).get_image();

        if (autotile_image.width() == 96 && autotile_image.height() == 128)
        {
            //standard
            Autotileset autotileset(FileOpener(db->autotiles_dir, name).get_image());
            autotileset.get_full_tileset().save(path + "Graphics" + QDir::separator() + "Autotiles" + QDir::separator() + name + ".png");
            params << 1; //standard autotile with no animation
        }
        else if (autotile_image.height() == 128 && autotile_image.width() >= 96)
        {
            //animated
            int num = ((autotile_image.width()-1)/96+1);
            QImage result(num * 256, 192, QImage::Format_ARGB32_Premultiplied);
            result.fill(Qt::transparent);
            QPainter painter;
            painter.begin(&result);
            for (int i = 0; i < num; i++)
            {
                Autotileset autotileset(autotile_image.copy(96*i,0,96,128));
                painter.drawImage(256*i, 0, autotileset.get_full_tileset());
            }
            painter.end();
            result.save(path + "Graphics" + QDir::separator() + "Autotiles" + QDir::separator() + name + ".png");
            params << num; //num of animations
        }
        else if (autotile_image.height() == 32)
        {
            int num = ((autotile_image.width()-1)/32+1);
            QImage result(num * 256, 192, QImage::Format_ARGB32_Premultiplied);
            result.fill(Qt::transparent);
            QPainter painter;
            painter.begin(&result);

            for (int i = 0; i < num; i++)
            {
                for (int x = 0; x < 8; x++)
                {
                    for (int y = 0; y < 6; y++)
                    {
                        painter.drawImage(256*i + 32*x,32*y, autotile_image.copy(32*i,0,32,32));
                    }
                }
            }
            painter.end();
            result.save(path + "Graphics" + QDir::separator() + "Autotiles" + QDir::separator() + name + ".png");
            params << num; //num of animations
        }
        else
        {
            //should not happen
            //just in case
            Autotileset autotileset(FileOpener(db->autotiles_dir, name).get_image());
            autotileset.get_full_tileset().save(path + "Graphics" + QDir::separator() + "Autotiles" + QDir::separator() + name + ".png");
            params << 1; //num of animations
        }

        this->imported_autotiles.insert(name, params);
        return params;
    }
}



void GodotExporter::create_dir_structure()
{
    QDir().mkdir(path + "World");
    QDir().mkdir(path + "Graphics");
    QDir().mkdir(path + "Graphics" + QDir::separator() + "Tilesets");
    QDir().mkdir(path + "Graphics" + QDir::separator() + "Autotiles");

    RPGMapInfoController mic(db);

    QJsonArray toplevel_maps = mic.get_toplevel_maps();
    QJsonArray non_toplevel_maps = mic.get_child_maps();

    for (int i = 0; i < toplevel_maps.count(); i++)
    {
        QString map_name = toplevel_maps.at(i).toObject().value("@name").toString();
        int id = toplevel_maps.at(i).toObject().value("id").toInt();
        QDir().mkdir(path + "World" + QDir::separator() + map_name); //if exists

        QString path_to_map = path + "World" + QDir::separator() + map_name + QDir::separator();
        this->map_id_to_dir.insert(id,path_to_map);

        this->write_map_to_file(id, map_name);
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

                this->write_map_to_file(id, map_name);

                non_toplevel_maps.removeAt(i--); //do this at the end
            }
        }
        int cnt2 = non_toplevel_maps.count();
        if (cnt1 == cnt2)
            break; //break here if parent_id loop
    }
}

void GodotExporter::write_map_to_file(int id, QString name)
{
    QFile f(this->map_id_to_dir.value(id) + QString("Map%1.tscn").arg(id,3,10,QChar('0')));
    f.open(QIODevice::WriteOnly);


    QJsonObject map = db->get_mapfile_by_id(id)->object();
    QJsonObject tileset = db->get_tileset_by_id(map.value("@tileset_id").toInt());

    int max_y = (tileset.value("@passages").toObject().value("values").toArray().count()-384 -1)/8+1;
    int height = map.value("@height").toInt();
    int width = map.value("@width").toInt();
    QJsonArray tiles = map.value("@data").toObject().value("values").toArray();
    QJsonArray passages = tileset.value("@passages").toObject().value("values").toArray();
    QJsonArray priorities = tileset.value("@priorities").toObject().value("values").toArray();

    QString scene_uid = this->random_id();
    QString Texture2D_uid = this->create_tileset(tileset.value("@tileset_name").toString()); //this->random_id();
    QString Texture2D_id = this->random_id();
    QString TileSetAtlasSource_id = this->random_id();
    QString Tileset_id = this->random_id();

    //AUTOTILES
    int num_autotiles = 0;
    QString autotiles_resources_import = "";
    QJsonArray autotile_names = tileset.value("@autotile_names").toArray();
    for (int i = 0; i < autotile_names.count(); i++)
    {
        if (autotile_names.at(i).toString() == "")
            continue;

        num_autotiles++;
        QVariantList ids = this->create_autotile(autotile_names.at(i).toString());
        autotiles_resources_import += QString("[ext_resource type=\"Texture2D\" uid=\"uid://" + ids.at(0).toString() + "\" path=\"res://Graphics/Autotiles/" +
                                              autotile_names.at(i).toString() +".png\" id=\"" + ids.at(1).toString() + "\"]\n");

    }




    f.write(QString("[gd_scene load_steps=" + QString::number(4+num_autotiles*2) + " format=3 uid=\"uid://" + scene_uid + "\"]\n\n").toUtf8());

    f.write(autotiles_resources_import.toUtf8());
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

    for (int i = 0; i < autotile_names.count(); i++)
    {
        if (autotile_names.at(i).toString() == "")
            continue;

        QVariantList ids = this->imported_autotiles.value(autotile_names.at(i).toString());
        if (ids.count() != 4)
        {
            qDebug() << "error";
            exit(67); // should never happen
        }


        f.write(QString("\n\n[sub_resource type=\"TileSetAtlasSource\" id=\"" + ids.at(2).toString() +"\"]\n").toUtf8());
        f.write(QString("texture = ExtResource(\"" + ids.at(1).toString() + "\")\n").toUtf8());
        f.write(QString("texture_region_size = Vector2i(32, 32)\n").toUtf8());
        f.write(QString("use_texture_padding = false\n").toUtf8());


        for (int y = 0; y < 6; y++)
        {
            for (int x = 0; x < 8; x++)
            {
                int animation_frames = ids.at(3).toInt();
                if (animation_frames > 1)
                {
                    f.write(QString("%1:%2/animation_columns = " + QString::number(animation_frames) + "\n").arg(x).arg(y).toUtf8());
                    f.write(QString("%1:%2/animation_separation = Vector2i(7, 0)\n").arg(x).arg(y).toUtf8());
                    for (int i = 0; i < animation_frames; i++)
                        f.write(QString("%1:%2/animation_frame_" + QString::number(i) + "/duration = 1.0\n").arg(x).arg(y).toUtf8());
                }
                f.write(QString("%1:%2/0 = 0\n").arg(x).arg(y).toUtf8());
                f.write(QString("%1:%2/0/physics_layer_0/linear_velocity = Vector2(0, 0)\n").arg(x).arg(y).toUtf8());
                f.write(QString("%1:%2/0/physics_layer_0/angular_velocity = 0.0\n").arg(x).arg(y).toUtf8());
                if ((passages.at(48*(i+1) + 8*y + x).toInt() & 0xF) > 0)
                    f.write(QString("%1:%2/0/physics_layer_0/polygon_0/points = PackedVector2Array(-16, -16, 16, -16, 16, 16, -16, 16)\n").arg(x).arg(y).toUtf8());
                if (priorities.at(48*(i+1) + 8*y + x).toInt() > 0)
                    f.write(QString("%1:%2/0/z_index = %3\n").arg(x).arg(y).arg(priorities.at(384 + 8*y + x).toInt()).toUtf8());
            }
        }
    }


    f.write(QString("\n[sub_resource type=\"TileSet\" id=\"" + Tileset_id + "\"]\n").toUtf8());
    f.write(QString("tile_size = Vector2i(32, 32)\n").toUtf8());
    f.write(QString("physics_layer_0/collision_layer = 1\n").toUtf8());
    f.write(QString("sources/0 = SubResource(\"" + TileSetAtlasSource_id + "\")\n\n").toUtf8()); //main tileset

    for (int i = 0; i < autotile_names.count(); i++)
    {
        if (autotile_names.at(i).toString() == "")
            continue;

        QVariantList ids = imported_autotiles.value(autotile_names.at(i).toString());
        if (ids.count() != 4) exit(678);
        f.write(QString("sources/" + QString::number(i+1) + " = SubResource(\"" + ids.at(2).toString() + "\")\n\n").toUtf8()); //main tileset

    }


    f.write(QString("[node name=\"" + name + "\" type=\"TileMap\"]\n").toUtf8());
    f.write(QString("texture_filter = 5\n").toUtf8());
    f.write(QString("position = Vector2(0, 0)\n").toUtf8());
    f.write(QString("tile_set = SubResource(\"" + Tileset_id + "\")\n").toUtf8());
    f.write(QString("format = 2\n").toUtf8());
    f.write(QString("layer_0/name = \"Ground\"\n").toUtf8());


    QString tiledata;

    tiledata = "";
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int tile_id = tiles.at(array_position(x,y,0,width,height)).toInt();
            if (tile_id < 48)
                continue; //do nothing
            else if (tile_id < 384) //autotile
            {
                int autotile_id = tile_id/48; //between 1 and 7
                tiledata += QString("%1, %2, %3, ").arg(x+65536*y).arg(65536*(tile_id%8) + autotile_id).arg( (tile_id % 48)/8);
            }
            else
            {
                tile_id -= 384;
                tiledata += QString("%1, %2, %3, ").arg(x+65536*y).arg(65536*(tile_id%8)).arg(tile_id/8);
            }
        }
    }
    tiledata.chop(2);
    tiledata = "layer_0/tile_data = PackedInt32Array(" + tiledata;
    tiledata += ")\n";
    f.write(tiledata.toUtf8());

    f.write(QString("layer_1/name = \"Middle\"\n").toUtf8());
    f.write(QString("layer_1/enabled = true\n").toUtf8());
    f.write(QString("layer_1/modulate = Color(1, 1, 1, 1)\n").toUtf8());
    f.write(QString("layer_1/y_sort_enabled = false\n").toUtf8());
    f.write(QString("layer_1/y_sort_origin = 0\n").toUtf8());
    f.write(QString("layer_1/z_index = 0\n").toUtf8());

    tiledata = "";
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int tile_id = tiles.at(array_position(x,y,1,width,height)).toInt();
            if (tile_id < 48)
                continue; //do nothing
            else if (tile_id < 384) //autotile
            {
                int autotile_id = tile_id/48; //between 1 and 7
                tiledata += QString("%1, %2, %3, ").arg(x+65536*y).arg(65536*(tile_id%8) + autotile_id).arg( (tile_id % 48)/8);
            }
            else
            {
                tile_id -= 384;
                tiledata += QString("%1, %2, %3, ").arg(x+65536*y).arg(65536*(tile_id%8)).arg(tile_id/8);
            }
        }
    }
    tiledata.chop(2);
    tiledata = "layer_1/tile_data = PackedInt32Array(" + tiledata;
    tiledata += ")\n";
    f.write(tiledata.toUtf8());

    f.write(QString("layer_2/name = \"Top\"\n").toUtf8());
    f.write(QString("layer_2/enabled = true\n").toUtf8());
    f.write(QString("layer_2/modulate = Color(1, 1, 1, 1)\n").toUtf8());
    f.write(QString("layer_2/y_sort_enabled = false\n").toUtf8());
    f.write(QString("layer_2/y_sort_origin = 0\n").toUtf8());
    f.write(QString("layer_2/z_index = 0\n").toUtf8());

    tiledata = "";
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int tile_id = tiles.at(array_position(x,y,2,width,height)).toInt();
            if (tile_id < 48)
                continue; //do nothing
            else if (tile_id < 384) //autotile
            {
                int autotile_id = tile_id/48; //between 1 and 7
                tiledata += QString("%1, %2, %3, ").arg(x+65536*y).arg(65536*(tile_id%8) + autotile_id).arg( (tile_id % 48)/8);
            }
            else
            {
                tile_id -= 384;
                tiledata += QString("%1, %2, %3, ").arg(x+65536*y).arg(65536*(tile_id%8)).arg(tile_id/8);
            }
        }
    }
    tiledata.chop(2);
    tiledata = "layer_2/tile_data = PackedInt32Array(" + tiledata;
    tiledata += ")\n";
    f.write(tiledata.toUtf8());


    f.close();
}

QString GodotExporter::random_id()
{
    QCryptographicHash hash(QCryptographicHash::Sha3_512);
    hash.addData(QString::number(QDateTime::currentMSecsSinceEpoch() + (counter++)).toUtf8());
    return QString(hash.result().toBase64().replace("+","").replace("/", "").mid(0,13));
}
