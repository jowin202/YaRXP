#include "rxdataparser.h"

RXDataParser::RXDataParser(QString file) : QObject()
{
    this->filePath = file;
    this->file.setFileName(file); //absolute path
}

void RXDataParser::check_header()
{
    this->file.open(QIODevice::ReadOnly);
    if (this->file.exists())
    {
        QByteArray buffer = file.read(2);
        if (buffer.length() < 2)
            qDebug() << "error: file too short";
        if (buffer.at(0) != 0x04 || buffer.at(1) != 0x08)
            qDebug() << "error: wrong version in header";
    }
    else
        qDebug() << "error: file does not exist";
}

void RXDataParser::close_file_if_open()
{
    if (this->file.isOpen())
        this->file.close();
}

int RXDataParser::get_byte_and_rev()
{
    quint64 pos = this->file.pos();
    int val = this->read_one_byte();
    this->file.seek(pos);
    return val;
}

int RXDataParser::read_fixnum()
{
    int num = this->read_one_byte();

    if (num == 0)
        return num;

    if (num > 0)
    {
        if (4 < num && num < 128) return num - 5;
        quint32 x = 0;
        for (int i = 0; i < 4; i++)
        {
            x = (i < num ? this->read_one_byte() << 24 : 0) | (x >> 8);
        }
        return (int)x;
    }
    else{
        if (-129 < num && num < -4) return num + 5;
        num = -num;

        int x = -1;
        const quint32 mask = ~(0xff << 24);
        for (int i = 0; i < 4; i++)
            x = (i < num ? this->read_one_byte() << 24 : 0xff) | ((x >> 8) & mask);
        return x;
    }

    return num;
}

int RXDataParser::read_one_byte()
{
    QByteArray one_byte = file.read(1);
    if (one_byte.length() != 1)
    {
        qDebug() << "weird error";
        return -1;
    }
    return (one_byte.at(0));
}

QString RXDataParser::read_symbol_or_link()
{
    QString symbol;
    int current_byte = this->read_one_byte();
    if (current_byte == ':') //name of the object, should be "RPG::MapInfo" or the link
    {
        int symbol_len = this->read_fixnum();
        symbol = QString(this->file.read(symbol_len));
        this->symbol_cache << symbol;
    }
    else if (current_byte == ';')
    {
        int symbol_index = this->read_fixnum(); // todo: check if it is RPG::MapInfo ..
        symbol = this->symbol_cache.at(symbol_index);
    }
    else
    {
        qDebug() << "error: symbol or link expected";
        exit(1);
    }

    return symbol;
}

QString RXDataParser::read_string()
{
    if (this->read_one_byte() == '"')
    {
        int str_len = this->read_fixnum();
        return this->file.read(str_len);
    }
    else
    {
        qDebug() << "error: string expected";
        return "";
    }
}

QVariant RXDataParser::read_variant()
{
    int read_byte = this->read_one_byte();
    if (read_byte == 'i')
        return QVariant(this->read_fixnum());
    else if (read_byte == '"'){
        int str_len = this->read_fixnum();
        return QVariant(this->file.read(str_len));
    }
    return QVariant();
}

int RXDataParser::read_integer()
{
    if (this->read_one_byte() == 'i') //get object id (key)
    {
        return this->read_fixnum();
    }
    else
    {
        qDebug() << "error: integer expected";
        return -1;
    }
}

bool RXDataParser::read_bool()
{
    int val = this->read_one_byte();
    if (val == 'T')
    {
        return true;
    }
    if (val == 'F')
    {
        return false;
    }
    qDebug() << "error: bool expected";
    return false;
}

RPGMapInfo* RXDataParser::read_mapinfo_object()
{
    RPGMapInfo *mapinfo_item = new RPGMapInfo();
    QString current_symbol;

    if (this->read_one_byte() != 'o')
    {
        qDebug() << "error: object as value expected";
    }


    if (read_symbol_or_link() != "RPG::MapInfo")
    {
        qDebug() << "error: wrong object, RPG::MapInfo expected";
    }


    int attribute_count = this->read_fixnum(); //should always be 6
    for (int j = 0; j < attribute_count; j++)
    {
        current_symbol = read_symbol_or_link();

        if (current_symbol == "@scroll_x" || current_symbol == "@scroll_y"  || current_symbol == "@parent_id" || current_symbol == "@order") //value
            mapinfo_item->setParameter(current_symbol, this->read_integer());
        else if (current_symbol == "@name")
            mapinfo_item->setParameter(current_symbol, this->read_string());
        else if (current_symbol == "@expanded")
            mapinfo_item->setParameter(current_symbol, this->read_bool());
    }
    return mapinfo_item;
}

RPGAudioFile* RXDataParser::read_audiofile_object()
{
    RPGAudioFile *audiofile = new RPGAudioFile();
    QString current_symbol;

    if (this->read_one_byte() != 'o')
    {
        qDebug() << "error: object as value expected";
    }


    if (read_symbol_or_link() != "RPG::AudioFile")
    {
        qDebug() << "error: wrong object, RPG::AudioFile expected";
    }
    int attribute_count = this->read_fixnum();
    for (int j = 0; j < attribute_count; j++)
    {
        current_symbol = read_symbol_or_link();

        if (current_symbol == "@pitch" || current_symbol == "@volume")
            audiofile->setParameter(current_symbol, this->read_integer());
        else if (current_symbol == "@name")
            audiofile->setParameter(current_symbol, this->read_string());
    }
    return audiofile;
}

void RXDataParser::read_event_list(QList<RPGEvent *> *list)
{
    if (this->read_one_byte() != 0x7b)
    {
        qDebug() << "byte 7b (list) expected";
    }
    int num_events = this->read_fixnum();
    int event_key;
    QString current_symbol;

    for (int i = 0; i < num_events; i++)
    {
        event_key = this->read_integer();
        //qDebug() << "start with event " << event_key;
        RPGEvent *event = this->read_event_object();
        //qDebug() << "----- " << event->id;
        list->append(event);

    }
}

RPGEvent *RXDataParser::read_event_object()
{
    RPGEvent *event_object = new RPGEvent();
    QString current_symbol;

    if (this->read_one_byte() != 'o')
    {
        qDebug() << "error: object as value expected";
    }

    if (read_symbol_or_link() != "RPG::Event")
    {
        qDebug() << "error: wrong object, RPG::Event expected";
    }
    int attribute_count = this->read_fixnum();
    for (int j = 0; j < attribute_count; j++)
    {
        current_symbol = read_symbol_or_link();
        if (current_symbol == "@x" || current_symbol == "@y" || current_symbol == "@id")
            event_object->setParameter(current_symbol, this->read_integer());
        else if (current_symbol == "@name")
            event_object->setParameter(current_symbol, this->read_string());
        else if (current_symbol == "@pages")
            this->read_event_pages_list(&event_object->pages);
    }
    return event_object;
}

void RXDataParser::read_event_pages_list(QList<RPGEventPage *> *list)
{
    if (this->read_one_byte() != 0x5b)
    {
        qDebug() << "byte 5b (array) expected";
    }
    int num_pages = this->read_fixnum();

    for (int i = 0; i < num_pages; i++)
    {
        list->append(this->read_event_page_object());
    }
}

RPGEventPage *RXDataParser::read_event_page_object()
{
    RPGEventPage *event_page_object = new RPGEventPage();
    QString current_symbol;

    if (this->read_one_byte() != 'o')
    {
        qDebug() << "error: object as value expected";
    }

    if (read_symbol_or_link() != "RPG::Event::Page")
    {
        qDebug() << "error: wrong object, RPG::EventPage expected";
    }

    int attribute_count = this->read_fixnum(); // 13
    for (int j = 0; j < attribute_count; j++)
    {
        current_symbol = read_symbol_or_link();
        //qDebug() << current_symbol;
        if (current_symbol == "@move_speed" || current_symbol == "@move_frequency" || current_symbol == "@move_type" || current_symbol == "@trigger")
            event_page_object->setParameter(current_symbol, this->read_integer());
        else if (current_symbol == "@walk_anime" || current_symbol == "@step_anime" || current_symbol == "@through" || current_symbol == "@direction_fix" || current_symbol == "@always_on_top")
            event_page_object->setParameter(current_symbol, this->read_bool());
        else if (current_symbol == "@list")
            this->read_event_command_list(&event_page_object->list);
        else if (current_symbol == "@condition")
            event_page_object->setParameter(current_symbol, this->read_event_page_condition_object());
        else if (current_symbol == "@move_route")
            event_page_object->setParameter(current_symbol, this->read_move_route_object());
        else if (current_symbol == "@graphic")
            event_page_object->setParameter(current_symbol, this->read_event_page_graphic());
    }
    return event_page_object;
}

void RXDataParser::read_event_command_list(QList<RPGEventCommand *> *list)
{
    if (this->read_one_byte() != 0x5b)
    {
        qDebug() << "byte 5b (array) expected";
    }
    int num_commands = this->read_fixnum();

    for (int i = 0; i < num_commands; i++)
    {
        list->append(this->read_event_command_object());
    }
}

RPGEventCommand *RXDataParser::read_event_command_object()
{
    RPGEventCommand *event_command_object = new RPGEventCommand();
    QString current_symbol;

    if (this->read_one_byte() != 'o')
    {
        qDebug() << "error: object as value expected";
    }

    if (read_symbol_or_link() != "RPG::EventCommand")
    {
        qDebug() << "error: wrong object, RPG::EventCommand expected";
    }
    int attribute_count = this->read_fixnum();

    for (int j = 0; j < attribute_count; j++)
    {
        current_symbol = read_symbol_or_link();
        if (current_symbol == "@code" || current_symbol == "@indent")
            event_command_object->setParameter(current_symbol, this->read_integer());
        else if (current_symbol == "@parameters")
        {
            if (read_one_byte() != 0x5B)
            {
                qDebug() << "error: 0x5B (array) expected for command list";
            }
            QList<QVariant> list;
            int count = this->read_fixnum();
            for (int k = 0; k < count; k++)
            {
                list << this->read_variant();
            }
            event_command_object->setParameter(current_symbol, list);
        }

    }
    return event_command_object;
}

RPGEventPageCondition *RXDataParser::read_event_page_condition_object()
{
    RPGEventPageCondition *event_page_condition_object = new RPGEventPageCondition();
    QString current_symbol;

    if (this->read_one_byte() != 'o')
    {
        qDebug() << "error: object as value expected";
    }

    if (read_symbol_or_link() != "RPG::Event::Page::Condition")
    {
        qDebug() << "error: wrong object, RPG::Event::Page::Condition expected";
    }

    int attribute_count = this->read_fixnum();
    for (int j = 0; j < attribute_count; j++)
    {
        current_symbol = read_symbol_or_link();
        if (current_symbol == "@switch1_id" || current_symbol == "@switch2_id" || current_symbol == "@variable_id" || current_symbol == "@variable_value")
            event_page_condition_object->setParameter(current_symbol, this->read_integer());
        else if (current_symbol == "@self_switch_valid" || current_symbol == "@variable_valid" || current_symbol == "@switch1_valid" || current_symbol == "@switch2_valid")
            event_page_condition_object->setParameter(current_symbol, this->read_bool());
        else if(current_symbol == "@self_switch_ch")
        {
            QString self_switch = this->read_string();
            if (self_switch == "A")
                event_page_condition_object->setParameter(current_symbol, 1);
            else if (self_switch == "B")
                event_page_condition_object->setParameter(current_symbol, 2);
            else if (self_switch == "C")
                event_page_condition_object->setParameter(current_symbol, 3);
            else if (self_switch == "D")
                event_page_condition_object->setParameter(current_symbol, 4);
        }
    }
    return event_page_condition_object;
}

RPGMoveRoute *RXDataParser::read_move_route_object()
{
    RPGMoveRoute *move_route_object = new RPGMoveRoute();
    QString current_symbol;

    if (this->read_one_byte() != 'o')
    {
        qDebug() << "error: object as value expected";
    }

    if (read_symbol_or_link() != "RPG::MoveRoute")
    {
        qDebug() << "error: wrong object, RPG::MoveRoute expected";
    }

    int attribute_count = this->read_fixnum();
    for (int j = 0; j < attribute_count; j++)
    {
        current_symbol = read_symbol_or_link();
        if (current_symbol == "@list")
            this->read_move_command_list(&move_route_object->list);
        else if (current_symbol == "@skippable" || current_symbol == "@repeat")
            move_route_object->setParameter(current_symbol, this->read_bool());
    }
    return move_route_object;
}

RPGMoveCommand *RXDataParser::read_move_command_object()
{
    RPGMoveCommand *move_command_object = new RPGMoveCommand();
    QString current_symbol;

    if (this->read_one_byte() != 'o')
    {
        qDebug() << "error: object as value expected";
    }

    if (read_symbol_or_link() != "RPG::MoveCommand")
    {
        qDebug() << "error: wrong object, RPG::MoveCommand expected";
    }

    int attribute_count = this->read_fixnum();
    for (int j = 0; j < attribute_count; j++)
    {
        current_symbol = read_symbol_or_link();
        if (current_symbol == "@parameters")
        {
            if (this->read_one_byte() != 0x5B)
            {
                qDebug() << "error: parameters in MoveCommand expects array";
            }
            int params_len = this->read_fixnum(); // = 0, finished TODO CHECK THIS
            //TODO
        }
        else if (current_symbol == "@code")
        {
            move_command_object->setParameter(current_symbol, this->read_integer());
        }
    }
    return move_command_object;
}

void RXDataParser::read_move_command_list(QList<RPGMoveCommand *> *list)
{
    if (this->read_one_byte() != 0x5b)
    {
        qDebug() << "byte 5b (array) expected";
    }
    int num_commands = this->read_fixnum();

    for (int i = 0; i < num_commands; i++)
    {
        list->append(this->read_move_command_object());
    }
}

RPGEventPageGraphic *RXDataParser::read_event_page_graphic()
{
    RPGEventPageGraphic *event_page_graphic_object = new RPGEventPageGraphic();
    QString current_symbol;

    if (this->read_one_byte() != 'o')
    {
        qDebug() << "error: object as value expected";
    }

    if (read_symbol_or_link() != "RPG::Event::Page::Graphic")
    {
        qDebug() << "error: wrong object, RPG::Event::Page::Graphic expected";
    }

    int attribute_count = this->read_fixnum();
    for (int j = 0; j < attribute_count; j++)
    {
        current_symbol = read_symbol_or_link();
        if (current_symbol == "@direction" || current_symbol == "@blend_type" || current_symbol == "@tile_id" || current_symbol == "@pattern" || current_symbol == "@character_hue" || current_symbol == "@opacity")
            event_page_graphic_object->setParameter(current_symbol, this->read_integer());
        else if (current_symbol == "@character_name")
            event_page_graphic_object->setParameter(current_symbol, this->read_string());
    }

    return event_page_graphic_object;
}



void RXDataParser::parseMapInfo(QList<RPGMapInfo*> *map_list)
{
    this->symbol_cache.clear();
    map_list->clear();
    this->check_header();

    if (this->read_one_byte() != 0x7b)
    {
        qDebug() << "byte 7b (list) expected";
        return;
    }
    int length = this->read_fixnum(); //expected: 0x7b length key value key value key value ...

    int key;
    QString current_symbol;
    QString current_name;

    for (int i = 0; i < length; i++) //browse all objects in list
    {
        key = this->read_integer();
        RPGMapInfo *mapinfo_item = this->read_mapinfo_object();
        mapinfo_item->set_id(key);
        map_list->append(mapinfo_item);
    }

    this->close_file_if_open();
}

RPGMap* RXDataParser::parseMap()
{
    this->symbol_cache.clear();
    this->check_header();

    RPGMap *map = new RPGMap();
    QString current_symbol;

    if (this->read_one_byte() != 'o')
    {
        qDebug() << "error: object as value expected";
    }


    if (read_symbol_or_link() != "RPG::Map")
    {
        qDebug() << "error: wrong object, RPG::Map expected";
    }
    int attribute_count = this->read_fixnum(); // should be 11
    for (int j = 0; j < attribute_count; j++)
    {
        current_symbol = read_symbol_or_link();
        if (current_symbol == "@bgm" || current_symbol == "@bgs")
            map->setParameter(current_symbol, read_audiofile_object());
        else if (current_symbol == "@tileset_id" || current_symbol == "@width" || current_symbol == "@height" || current_symbol == "@encounter_step")
            map->setParameter(current_symbol, read_integer());
        else if (current_symbol == "@autoplay_bgs" || current_symbol == "@autoplay_bgm")
            map->setParameter(current_symbol, read_bool());
        else if (current_symbol == "@events")
            this->read_event_list(&map->events);
    }


    this->close_file_if_open();
    return map;
}

