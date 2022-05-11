#include "rxdataparser.h"

RXDataParser::RXDataParser(QString file) : QObject()
{
    this->filePath = file;
    this->file.setFileName(file); //absolute path
}

void RXDataParser::print_file_pos_as_hex()
{
    qDebug() << QString::number(this->file.pos(),16).toUpper();
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
        qDebug() << "unexpected eof";
        return -1;
    }
    return (one_byte.at(0));
}

int RXDataParser::look_one_byte_ahead()
{
    qint64 position = this->file.pos();
    QByteArray one_byte = this->file.read(1);
    if (one_byte.length() != 1)
    {
        qDebug() << "unexpected eof";
        return -1;
    }
    this->file.seek(position);
    return (one_byte.at(0));
}

QString RXDataParser::look_ahead_object_type()
{
    qint64 position = this->file.pos();
    QByteArray one_byte = this->file.read(1);
    if (one_byte.length() != 1)
    {
        qDebug() << "unexpected eof";
        return "";
    }
    if (one_byte.at(0) != 'o')
    {
        qDebug() << "error with object";
        return "";
    }
    QString object_type = this->read_symbol_or_link(false);
    this->file.seek(position);
    return object_type;
}

QString RXDataParser::read_symbol_or_link(bool save_symbol)
{
    QString symbol;
    int current_byte = this->read_one_byte();
    if (current_byte == ':')
    {
        int symbol_len = this->read_fixnum();
        symbol = QString(this->file.read(symbol_len));
        if (save_symbol)
            this->symbol_cache << symbol;
    }
    else if (current_byte == ';')
    {
        int symbol_index = this->read_fixnum();
        symbol = this->symbol_cache.at(symbol_index);
    }
    else
    {
        qDebug() << ("error: symbol or link expected: 0x" + QString::number(this->file.pos(),16).toUpper());
        exit(1);
    }

    return symbol;
}

QString RXDataParser::read_string()
{
    int var = this->read_one_byte();
    if (var == '"')
    {
        int str_len = this->read_fixnum();
        return this->file.read(str_len);
    }
    else if (var == 'I')
    {
        QString str = this->read_string();
        int val = this->read_fixnum(); //should be 1
        if (val != 1) //instance variable E=True is for encoding.
        {
            qDebug() << "instance var error weird"; //should never be reached
            exit(1);
        }
        this->read_symbol_or_link(); //should be E
        this->read_bool(); //should be (T)rue
        return str;
    }
    else
    {
        qDebug() << "error: string expected";
        return "";
    }
}

QVariant RXDataParser::read_variant() //string int or symbol
{
    int read_byte = this->look_one_byte_ahead();
    if (read_byte == 'i')
        return QVariant(this->read_integer());
    else if (read_byte == '"')
        return QVariant(this->read_string());
    else if (read_byte == 'I') // Instance Variable
    {
        return QVariant(this->read_string());
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
        else
        {
            qDebug() << "Error parsing RPGEvent object: attribute " << current_symbol;
            exit(1);
        }
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
        else
        {
            qDebug() << "Error parsing RPGEventPage object: attribute " << current_symbol;
            exit(1);
        }
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
                if (this->look_one_byte_ahead() == 'o') //audiofile or move route
                {
                    if (this->look_ahead_object_type() == "RPG::AudioFile")
                        event_command_object->audiofile = this->read_audiofile_object();
                    else if (this->look_ahead_object_type() == "RPG::MoveRoute")
                    {
                        this->read_move_route_object();
                    }
                    else
                    {
                        qDebug() << "Error parsing command: object (audiofile or moveroute) expected";
                        exit(1);
                    }
                }
                else if (this->look_one_byte_ahead() == '@')
                {
                    //There is a link to the 209 object in the first 509 object
                    //link is used as '@' + object number. It is redundant, hence ignored.
                    this->read_one_byte(); //throw away the @ sign
                    this->read_fixnum();
                }
                else if (this->look_one_byte_ahead() == 'u') //color tone
                {
                    this->read_one_byte(); //skip this byte
                    if (this->read_symbol_or_link() == "Tone")
                    {
                        this->read_fixnum(); //int size = ... 32 bytes for 4 doubles. unused
                        double red,green,blue,gray;
                        this->file.read((char*)&red, 8);
                        this->file.read((char*)&green, 8);
                        this->file.read((char*)&blue, 8);
                        this->file.read((char*)&gray, 8);
                    }
                    else
                    {
                        qDebug() << "Error parsing command: Tone expected";
                        exit(1);
                    }
                }
                else if (this->look_one_byte_ahead() == '[') //another list in parameters. stringlist for choices
                {
                    //show choices list
                    this->read_one_byte();
                    int num_choices = this->read_fixnum();
                    for (int i = 0; i < num_choices; i++)
                    {
                        event_command_object->choices_list << this->read_string();
                    }
                }
                else //string or int
                {
                    list << this->read_variant();
                }
            }
            event_command_object->setParameter(current_symbol, list);
        }

    }
    //event_command_object->debug();

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
        exit(1);
    }

    if (read_symbol_or_link() != "RPG::MoveCommand")
    {
        qDebug() << "error: wrong object, RPG::MoveCommand expected";
        exit(1);
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
            //parameters are needed for advanced move command (e.g. direction for jumping)
            //TODO: list can contain audiofiles (see setmoveroute dialog)
            for (int p = 0; p < params_len; p++) //assumed parameters are string or int
            {
                int current_byte = this->look_one_byte_ahead();
                if (current_byte == '"' || current_byte == 'i' || current_byte == 'I')
                    move_command_object->parameters.append(QVariant(this->read_variant()));
                else if (current_byte == 'o')
                {

                    //TODO: e.g. audiofile
                    if (this->look_ahead_object_type() == "RPG::AudioFile")
                    {
                        move_command_object->audiofile = this->read_audiofile_object();
                    }
                }
            }
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
        else if (current_symbol == "@encounter_list")
        {
            if (this->read_one_byte() != '[')
            {
                qDebug() << "error: expect byte [ for encounter list";
                exit(1);
            }
            if (this->read_one_byte() != '\0')
            {
                qDebug() << "error: expect empty encounter list (not relevant for Pokemon)";
                exit(1);
            }

        }
        else if (current_symbol == "@data")
        {
            if (this->read_one_byte() != 'u') //user defined data structure
            {
                qDebug() << "error: table expected for attribute data, expect byte 'u'";
                exit(1);
            }
            if (this->read_symbol_or_link() != "Table")
            {
                qDebug() << "error: table expected for attribute data, expetect symbol 'Table'";
                exit(1);
            }

            this->read_fixnum(); //ignore this value ... it is the size of the table


            //skip 4 bytes don't know why -.-
            for (int i = 0; i < 4; i++)
                this->read_one_byte();
            int x=0, y=0, z=0, size;

            for (int i = 0; i < 4; i++)
                x |= ((this->read_one_byte() & 0xFF) << (8*i)) ;
            for (int i = 0; i < 4; i++)
                y |= ((this->read_one_byte() & 0xFF) << (8*i)) ;
            for (int i = 0; i < 4; i++)
                z |= ((this->read_one_byte() & 0xFF) << (8*i)) ;
            for (int i = 0; i < 4; i++)
                size |= ((this->read_one_byte() & 0xFF) << (8*i)) ;



            map->width = x;
            map->height = y;
            if (z != 3)
            {
                qDebug() << "Error: z of map has to be 3";
                exit(1);
            }


            for (int i = 0; i < x*y*z; i++)
                map->data.append((this->read_one_byte()&0xFF) | ((this->read_one_byte() & 0xFF) << 8));
        }
        else {
            qDebug() << "error: unknown attribute " << current_symbol << " at parsing RPGMap";
            exit(1);
        }
    }


    this->close_file_if_open();
    return map;
}

RPGTileset *RXDataParser::parseTileset()
{
    RPGTileset *tileset_object = new RPGTileset();
    QString current_symbol;

    if (this->read_one_byte() != 'o')
    {
        qDebug() << "error: object as value expected";
        exit(1);
    }


    if (read_symbol_or_link() != "RPG::Tileset")
    {
        qDebug() << "error: wrong object, RPG::Tileset expected";
        exit(1);
    }

    int attribute_count = this->read_fixnum(); //17
    for (int j = 0; j < attribute_count; j++)
    {
        current_symbol = read_symbol_or_link();
        if (current_symbol == "@id")
        {
            tileset_object->id = this->read_integer();
        }
        else if (current_symbol == "@tileset_name")
        {
            tileset_object->tileset_name = this->read_string();
        }
        else if (current_symbol == "@battleback_name")
        {
            tileset_object->battleback_name = this->read_string();
        }
        else if (current_symbol == "@panorama_hue")
        {
            tileset_object->panorama_hue = this->read_integer();
        }
        else if (current_symbol == "@panorama_name")
        {
            tileset_object->panorama_name = this->read_string();
        }
        else if (current_symbol == "@name")
        {
            tileset_object->name = this->read_string();
        }
        else if (current_symbol == "@fog_sx")
        {
            tileset_object->fog_sx = this->read_integer();
        }
        else if (current_symbol == "@fog_sy")
        {
            tileset_object->fog_sy = this->read_integer();
        }
        else if (current_symbol == "@fog_hue")
        {
            tileset_object->fog_hue = this->read_integer();
        }
        else if (current_symbol == "@fog_zoom")
        {
            tileset_object->fog_zoom = this->read_integer();
        }
        else if (current_symbol == "@fog_name")
        {
            tileset_object->fog_name = this->read_string();
        }
        else if (current_symbol == "@fog_opacity")
        {
            tileset_object->fog_opacity = this->read_integer();
        }
        else if (current_symbol == "@fog_blend_type")
        {
            tileset_object->fog_blend_type = this->read_integer();
        }
        else if (current_symbol == "@autotile_names")
        {
            if (this->read_one_byte() != '[')
            {
                qDebug() << "error: list expected";
                exit(1);
            }
            int num_autotiles = this->read_fixnum(); // 7
            for (int i = 0; i < num_autotiles; i++)
                tileset_object->autotile_names.append(this->read_string());
        }
        else if (current_symbol == "@passages")
        {
            if (this->read_one_byte() != 'u') //user defined data structure
            {
                qDebug() << "error: table expected, expect byte 'u'";
                exit(1);
            }
            if (this->read_symbol_or_link() != "Table")
            {
                qDebug() << "error: table expected, expetect symbol 'Table'";
                exit(1);
            }

            this->read_fixnum(); //ignore this value ... it is the size of the table


            //skip 4 bytes don't know why -.-
            for (int i = 0; i < 4; i++)
                this->read_one_byte();
            int x=0, y=0, z=0;

            for (int i = 0; i < 4; i++)
                x |= ((this->read_one_byte() & 0xFF) << (8*i)) ;
            for (int i = 0; i < 4; i++)
                y |= ((this->read_one_byte() & 0xFF) << (8*i)) ;
            for (int i = 0; i < 4; i++)
                z |= ((this->read_one_byte() & 0xFF) << (8*i)) ;
            for (int i = 0; i < 4; i++)
                this->read_one_byte() ; //withdraw result, size can be calculated

            int size = x*y*z;


            for (int i = 0; i < size*2; i++)
                tileset_object->passages.append(this->read_one_byte());
        }
        else if (current_symbol == "@priorities")
        {
            if (this->read_one_byte() != 'u') //user defined data structure
            {
                qDebug() << "error: table expected, expect byte 'u'";
                exit(1);
            }
            if (this->read_symbol_or_link() != "Table")
            {
                qDebug() << "error: table expected, expetect symbol 'Table'";
                exit(1);
            }

            this->read_fixnum(); //ignore this value ... it is the size of the table


            //skip 4 bytes don't know why -.-
            for (int i = 0; i < 4; i++)
                this->read_one_byte();
            int x=0, y=0, z=0;

            for (int i = 0; i < 4; i++)
                x |= ((this->read_one_byte() & 0xFF) << (8*i)) ;
            for (int i = 0; i < 4; i++)
                y |= ((this->read_one_byte() & 0xFF) << (8*i)) ;
            for (int i = 0; i < 4; i++)
                z |= ((this->read_one_byte() & 0xFF) << (8*i)) ;
            for (int i = 0; i < 4; i++)
                this->read_one_byte() ; //withdraw result, size can be calculated

            int size = x*y*z;


            for (int i = 0; i < size*2; i++)
                tileset_object->priorities.append(this->read_one_byte());
        }
        else if (current_symbol == "@terrain_tags")
        {
            if (this->read_one_byte() != 'u') //user defined data structure
            {
                qDebug() << "error: table expected, expect byte 'u'";
                exit(1);
            }
            if (this->read_symbol_or_link() != "Table")
            {
                qDebug() << "error: table expected, expetect symbol 'Table'";
                exit(1);
            }

            this->read_fixnum(); //ignore this value ... it is the size of the table


            //skip 4 bytes don't know why -.-
            for (int i = 0; i < 4; i++)
                this->read_one_byte();
            int x=0, y=0, z=0;

            for (int i = 0; i < 4; i++)
                x |= ((this->read_one_byte() & 0xFF) << (8*i)) ;
            for (int i = 0; i < 4; i++)
                y |= ((this->read_one_byte() & 0xFF) << (8*i)) ;
            for (int i = 0; i < 4; i++)
                z |= ((this->read_one_byte() & 0xFF) << (8*i)) ;
            for (int i = 0; i < 4; i++)
                this->read_one_byte() ; //withdraw result, size can be calculated

            int size = x*y*z;

            for (int i = 0; i < size*2; i++)
                tileset_object->terrain_tag.append(this->read_one_byte());

            //qDebug() << this->file.pos();
        }
    }
    return tileset_object;
}

void RXDataParser::parseTilesetList(QList<RPGTileset *> *tileset_list)
{
    tileset_list->clear();
    this->symbol_cache.clear();
    this->check_header();

    if (this->read_one_byte() != '[')
    {
        qDebug() << "Error parsing tileset: list expected";
        exit(1);
    }
    int num_list = this->read_fixnum();


    for (int i = 0; i < num_list; i++)
    {
        if (this->look_one_byte_ahead() == '0')
        {
            this->read_one_byte(); //ignore and do nothing
        }
        else if (this->look_one_byte_ahead() == 'o')
        {
            RPGTileset *tileset = this->parseTileset();
            tileset_list->append(tileset);
        }
    }
    this->close_file_if_open();
}

