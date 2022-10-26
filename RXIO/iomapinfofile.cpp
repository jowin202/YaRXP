#include "RXObjects/rpgmapinfo.h"
#include "iomapinfofile.h"


IOMapInfoFile::IOMapInfoFile(QObject *parent) : FileParser(parent)
{

}

IOMapInfoFile::IOMapInfoFile(QString path, QList<RPGMapInfo*> *mapinfo_list)
{
    this->last_visited_function = "IOMapInfoFile";
    this->symbol_cache.clear();
    this->object_count = 0;

    this->path = path;
    this->file.setFileName(path);
    this->file.open(QIODevice::ReadOnly);

    check_header();
    int length = read_list();

    for (int i = 0 ; i < length; i++)
    {
        int key = read_integer();
        RPGMapInfo *mapinfo_item = new RPGMapInfo;
        mapinfo_item->set_id(key);
        QVariantList list = read_object();
        if (list.at(0).toString() != "RPG::MapInfo")
            throw getException("RPG::MapInfo expected");
        if (list.at(1).toInt() != 6)
            throw getException("RPG::MapInfo expects 6 parameters");

        for (int j = 0; j < list.at(1).toInt(); j++)
        {
            QString current_symbol = read_symbol_or_link();

            if (current_symbol == "@scroll_x")
                mapinfo_item->scroll_x = read_integer();
            else if (current_symbol == "@scroll_y")
                mapinfo_item->scroll_y = read_integer();
            else if (current_symbol == "@parent_id")
                mapinfo_item->parent_id = read_integer();
            else if (current_symbol == "@order")
                mapinfo_item->order = read_integer();
            else if (current_symbol == "@name")
                mapinfo_item->name = read_string();
            else if (current_symbol == "@expanded")
                mapinfo_item->expanded = read_bool();
        }
        mapinfo_list->append(mapinfo_item);

    }

    this->file.close();

}

void IOMapInfoFile::write_to_file(QString path, QList<RPGMapInfo*> *mapinfo_list)
{
    this->last_visited_function = "write_to_file";
    this->symbol_cache.clear();
    this->object_count = 0;

    if (!path.isEmpty())
        this->path = path;
    this->file.setFileName(path);
    this->file.open(QIODevice::WriteOnly);
    this->write_header();
    this->write_list(mapinfo_list->length());

    for (int i = 0; i < mapinfo_list->length(); i++)
    {
        this->write_integer(mapinfo_list->at(i)->id); //key
        this->write_object("RPG::MapInfo", 6);

        this->write_symbol_or_link("@scroll_x");
        this->write_integer(mapinfo_list->at(i)->scroll_x);


        this->write_symbol_or_link("@name");
        this->write_string(mapinfo_list->at(i)->name);


        this->write_symbol_or_link("@expanded");
        this->write_bool(mapinfo_list->at(i)->expanded);

        this->write_symbol_or_link("@order");
        this->write_integer(mapinfo_list->at(i)->order);

        this->write_symbol_or_link("@scroll_y");
        this->write_integer(mapinfo_list->at(i)->scroll_y);

        this->write_symbol_or_link("@parent_id");
        this->write_integer(mapinfo_list->at(i)->parent_id);

    }


    this->file.close();

}
