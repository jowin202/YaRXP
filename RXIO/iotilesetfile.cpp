#include "iotilesetfile.h"

#include <QDebug>

IOTilesetFile::IOTilesetFile(QObject *parent) : FileParser(parent)
{

}

IOTilesetFile::IOTilesetFile(QString path, QHash<int,RPGTileset *> *tileset_hash, QList<RPGTileset*> *tileset_list)
{
    this->last_visited_function = "IOTilesetFile";
    this->symbol_cache.clear();

    this->path = path;
    this->file.setFileName(path);
    this->file.open(QIODevice::ReadOnly);

    check_header();
    int length = read_array();

    read_one_byte(); // always zero, dont forget to write it

    for (int i = 0; i < length-1; i++) // one less because of zero
    {
        RPGTileset *tileset_object = new RPGTileset();

        QVariantList list = read_object();
        if (list.at(0).toString() != "RPG::Tileset")
            throw getException("RPG::Tileset expected");

        for (int j = 0; j < list.at(1).toInt(); j++)
        {
            QString current_symbol = read_symbol_or_link();

            if (current_symbol == "@id")
                tileset_object->id = this->read_integer();
            else if (current_symbol == "@tileset_name")
                tileset_object->tileset_name = this->read_string();
            else if (current_symbol == "@battleback_name")
                tileset_object->battleback_name = this->read_string();
            else if (current_symbol == "@panorama_hue")
                tileset_object->panorama_hue = this->read_integer();
            else if (current_symbol == "@panorama_name")
                tileset_object->panorama_name = this->read_string();
            else if (current_symbol == "@name")
                tileset_object->name = this->read_string();
            else if (current_symbol == "@fog_sx")
                tileset_object->fog_sx = this->read_integer();
            else if (current_symbol == "@fog_sy")
                tileset_object->fog_sy = this->read_integer();
            else if (current_symbol == "@fog_hue")
                tileset_object->fog_hue = this->read_integer();
            else if (current_symbol == "@fog_zoom")
                tileset_object->fog_zoom = this->read_integer();
            else if (current_symbol == "@fog_name")
                tileset_object->fog_name = this->read_string();
            else if (current_symbol == "@fog_opacity")
                tileset_object->fog_opacity = this->read_integer();
            else if (current_symbol == "@fog_blend_type")
                tileset_object->fog_blend_type = this->read_integer();
            else if (current_symbol == "@terrain_tags")
                read_table(&tileset_object->terrain_tag);
            else if (current_symbol == "@priorities")
                read_table(&tileset_object->priorities);
            else if (current_symbol == "@passages")
                read_table(&tileset_object->passages);
            else if (current_symbol == "@autotile_names")
            {
                int num_autotiles = read_array(); //7
                for (int i = 0; i < num_autotiles; i++)
                    tileset_object->autotile_names.append(this->read_string());
            }
        }
        tileset_list->append(tileset_object);
        tileset_hash->insert(tileset_object->id, tileset_object);

    }


    this->file.close();
}

void IOTilesetFile::write_to_file(QString path, QList<RPGTileset*> *tileset_list)
{
    this->last_visited_function = "write_to_file";
    this->symbol_cache.clear();

    if (!path.isEmpty())
        this->path = path;
    this->file.setFileName(path);
    this->file.open(QIODevice::WriteOnly);
    this->write_header();

    this->write_array(tileset_list->length()+1); //because of zero at beginning

    this->write_one_byte((int)'0');

    for (int i = 0; i < tileset_list->length(); i++)
    {
        this->write_object("RPG::Tileset", 17);

        this->write_symbol_or_link("@terrain_tags");
        this->write_table(&tileset_list->at(i)->terrain_tag);

        this->write_symbol_or_link("@panorama_hue");
        this->write_integer(tileset_list->at(i)->panorama_hue);

        this->write_symbol_or_link("@fog_sy");
        this->write_integer(tileset_list->at(i)->fog_sy);

        this->write_symbol_or_link("@name");
        this->write_string(tileset_list->at(i)->name);

        this->write_symbol_or_link("@fog_opacity");
        this->write_integer(tileset_list->at(i)->fog_opacity);

        this->write_symbol_or_link("@priorities");
        this->write_table(&tileset_list->at(i)->priorities);

        this->write_symbol_or_link("@panorama_name");
        this->write_string(tileset_list->at(i)->panorama_name);

        this->write_symbol_or_link("@fog_sx");
        this->write_integer(tileset_list->at(i)->fog_sx);

        this->write_symbol_or_link("@fog_hue");
        this->write_integer(tileset_list->at(i)->fog_hue);

        this->write_symbol_or_link("@passages");
        this->write_table(&tileset_list->at(i)->passages);

        this->write_symbol_or_link("@autotile_names");
        this->write_array(tileset_list->at(i)->autotile_names.length()); //7
        for (int j = 0; j < tileset_list->at(i)->autotile_names.length(); j++)
            this->write_string(tileset_list->at(i)->autotile_names.at(j));

        this->write_symbol_or_link("@fog_zoom");
        this->write_integer(tileset_list->at(i)->fog_zoom);

        this->write_symbol_or_link("@fog_name");
        this->write_string(tileset_list->at(i)->fog_name);

        this->write_symbol_or_link("@battleback_name");
        this->write_string(tileset_list->at(i)->battleback_name);

        this->write_symbol_or_link("@tileset_name");
        this->write_string(tileset_list->at(i)->tileset_name);

        this->write_symbol_or_link("@id");
        this->write_integer(tileset_list->at(i)->id);

        this->write_symbol_or_link("@fog_blend_type");
        this->write_integer(tileset_list->at(i)->fog_blend_type);
    }


    this->file.close();
}
