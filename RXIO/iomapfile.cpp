#include "iomapfile.h"
#include "fileparser.h"

IOMapFile::IOMapFile(QObject *parent) : FileParser(parent)
{

}

IOMapFile::IOMapFile(QString path, RPGMap *map)
{
    this->last_visited_function = "IOMapInfoFile";
    this->symbol_cache.clear();

    this->path = path;
    this->file.setFileName(path);
    this->file.open(QIODevice::ReadOnly);

    check_header();
    QVariantList params = this->read_object();
    if (params.at(0).toString() != "RPG::Map")
        throw getException("RPG::Map expected");
    int num_params = params.at(1).toInt();

    for (int i = 0; i < num_params; i++)
    {
        QString current_symbol = this->read_symbol_or_link();

        if (current_symbol == "@bgm")
            this->read_audiofile_object(&map->bgm);
        else if (current_symbol == "@bgs")
            this->read_audiofile_object(&map->bgs);
        else if (current_symbol == "@width")
            map->width = this->read_integer();
        else if (current_symbol == "@height")
            map->height = this->read_integer();
        else if (current_symbol == "@tileset_id")
            map->tileset_id = this->read_integer();
        else if (current_symbol == "@encounter_step")
            map->encounter_step = this->read_integer();
        else if(current_symbol == "@autoplay_bgm")
            map->autoplay_bgm = this->read_bool();
        else if(current_symbol == "@autoplay_bgs")
            map->autoplay_bgs = this->read_bool();
        else if (current_symbol == "@data")
            this->read_table_for_map(&map->data);
        else if (current_symbol == "@encounter_list")
        {
            int n = this->read_array();
            //TODO: check this (as it is not relevant for pokemon, it is not well tested)
            for (int i = 0; i < n; i++)
                map->encounter_list.append(this->read_integer()); //assuming encounter list consists of integers only
        }
        else if (current_symbol == "@events")
        {
            int num_events = this->read_list();
            for (int j = 0; j < num_events; j++)
            {
                //iterate over events
                this->read_integer(); //event key, withdrawn as it is the same than event->id
                RPGEvent *current_event = new RPGEvent();
                QVariantList params = this->read_object();
                if (params.at(0).toString() != "RPG::Event")
                    throw getException("RPG::Event expected");
                int element_attribute_count = params.at(1).toInt();

                for (int k = 0; k < element_attribute_count; k++)
                {
                    //iterate over current event parameters
                    QString current_symbol = this->read_symbol_or_link();

                    if (current_symbol == "@x")
                        current_event->x = this->read_integer();
                    else if(current_symbol == "@y")
                        current_event->y = this->read_integer();
                    else if(current_symbol == "@id")
                        current_event->id = this->read_integer();
                    else if(current_symbol == "@name")
                        current_event->name = this->read_string();
                    else if (current_symbol == "@pages")
                    {
                        int num_pages = this->read_array();

                        for (int l = 0; l < num_pages; l++)
                        {
                            //iterate over event pages
                            RPGEventPage *current_page = new RPGEventPage();
                            QVariantList params = this->read_object();
                            if (params.at(0).toString() != "RPG::Event::Page")
                                throw getException("RPG::Event::Page expected");
                            int element_attribute_count = params.at(1).toInt();

                            for (int m = 0; m < element_attribute_count; m++)
                            {
                                //iterate over attributes of element page
                                QString current_symbol = this->read_symbol_or_link();
                                if (current_symbol == "@move_speed")
                                    current_page->move_speed = this->read_integer();
                                else if (current_symbol == "@move_frequency")
                                    current_page->move_frequency = this->read_integer();
                                else if (current_symbol == "@move_type")
                                    current_page->move_type = this->read_integer();
                                else if (current_symbol == "@trigger")
                                    current_page->trigger = this->read_integer();
                                else if (current_symbol == "@walk_anime")
                                    current_page->walk_anime = this->read_bool();
                                else if (current_symbol == "@step_anime")
                                    current_page->step_anime = this->read_bool();
                                else if (current_symbol == "@through")
                                    current_page->through = this->read_bool();
                                else if (current_symbol == "@direction_fix")
                                    current_page->direction_fix = this->read_bool();
                                else if (current_symbol == "@always_on_top")
                                    current_page->always_on_top = this->read_bool();
                                else if (current_symbol == "@list")
                                {
                                    int num_commands = this->read_array();
                                    for (int n = 0; n < num_commands; n++)
                                    {
                                        //iterate over event commands
                                        RPGEventCommand *current_event_command = new RPGEventCommand();
                                        QVariantList params = this->read_object();
                                        if (params.at(0).toString() != "RPG::EventCommand")
                                            throw getException("RPG::EventCommand expected");
                                        int num_event_command_attributes = params.at(1).toInt();

                                        for (int o = 0; o < num_event_command_attributes; o++)
                                        {
                                            //iterate over event command attributes
                                            QString current_symbol = this->read_symbol_or_link();
                                            if (current_symbol == "@code")
                                                current_event_command->code =  this->read_integer();
                                            else if (current_symbol == "@indent")
                                                current_event_command->indent = this->read_integer();
                                            else if (current_symbol == "@parameters")
                                            {
                                                int num_command_params = this->read_array();
                                                for (int p = 0; p < num_command_params; p++)
                                                {
                                                    //iterate over params of event command
                                                    if (this->look_one_byte_ahead() == 'o') //audiofile or move route
                                                    {
                                                        if (this->look_ahead_object_type() == "RPG::AudioFile")
                                                            this->read_audiofile_object(&current_event_command->audiofile);
                                                        else if (this->look_ahead_object_type() == "RPG::MoveRoute")
                                                            this->read_move_route_object(&current_event_command->move_route);

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
                                                        this->read_one_byte(); // withdraw the u
                                                        QString symbol = this->read_symbol_or_link();
                                                        if (symbol == "Tone" || symbol == "Color") //old version had color instead of tone (feuergruen)
                                                        {
                                                            this->read_fixnum(); //32 unused

                                                            this->file.read((char*)&current_event_command->red, 8);
                                                            this->file.read((char*)&current_event_command->green, 8);
                                                            this->file.read((char*)&current_event_command->blue, 8);
                                                            this->file.read((char*)&current_event_command->gray, 8);
                                                        }
                                                        else throw getException("Color tone expected");

                                                    }
                                                    else if (this->look_one_byte_ahead() == '[') //another list in parameters. stringlist for choices
                                                    {
                                                        //code 102 only
                                                        int num_choices = this->read_array();
                                                        for (int choice = 0; choice < num_choices; choice++)
                                                            current_event_command->choices_list.append(this->read_string());
                                                    }
                                                    else //string or int or bool
                                                    {
                                                        current_event_command->parameters.append(this->read_variant()); //change this to a RPGString compatible variant list
                                                    }
                                                }
                                            }

                                        }
                                        current_page->list.append(current_event_command);
                                    }
                                }
                                else if (current_symbol == "@condition")
                                {
                                    QVariantList params = this->read_object();
                                    if (params.at(0).toString() != "RPG::Event::Page::Condition")
                                        throw getException("PG::Event::Page::Condition expected");
                                    int condition_param_count = params.at(1).toInt();

                                    for (int c = 0; c < condition_param_count; c++)
                                    {
                                        current_symbol = read_symbol_or_link();
                                        if (current_symbol == "@switch1_id")
                                            current_page->switch1_id = this->read_integer();
                                        if (current_symbol == "@switch2_id")
                                            current_page->switch2_id = this->read_integer();
                                        if (current_symbol == "@variable_id")
                                            current_page->variable_id = this->read_integer();
                                        if (current_symbol == "@variable_value")
                                            current_page->variable_value = this->read_integer();
                                        else if (current_symbol == "@self_switch_valid")
                                            current_page->self_switch_valid = this->read_bool();
                                        else if (current_symbol == "@variable_valid")
                                            current_page->variable_valid = this->read_bool();
                                        else if (current_symbol == "@switch1_valid")
                                            current_page->switch1_valid = this->read_bool();
                                        else if (current_symbol == "@switch2_valid")
                                            current_page->switch2_valid = this->read_bool();
                                        else if(current_symbol == "@self_switch_ch")
                                        {
                                            QString self_switch = this->read_string();
                                            if (self_switch == "A")
                                                current_page->self_switch_ch = 1;
                                            else if (self_switch == "B")
                                                current_page->self_switch_ch = 2;
                                            else if (self_switch == "C")
                                                current_page->self_switch_ch = 3;
                                            else if (self_switch == "D")
                                                current_page->self_switch_ch = 4;
                                        }
                                    }

                                }
                                else if (current_symbol == "@move_route")
                                {
                                    this->read_move_route_object(&current_page->move_route);
                                }
                                else if (current_symbol == "@graphic")
                                {
                                    QVariantList params = this->read_object();
                                    if (params.at(0).toString() != "RPG::Event::Page::Graphic")
                                        throw getException("PG::Event::Page::Graphic expected");
                                    int graphic_param_count = params.at(1).toInt();

                                    for (int g = 0; g < graphic_param_count; g++)
                                    {
                                        current_symbol = read_symbol_or_link();
                                        if (current_symbol == "@direction")
                                            current_page->direction = this->read_integer();
                                        else if (current_symbol == "@blend_type")
                                            current_page->blend_type = this->read_integer();
                                        else if (current_symbol == "@tile_id")
                                            current_page->tile_id = this->read_integer();
                                        else if (current_symbol == "@pattern")
                                            current_page->pattern = this->read_integer();
                                        else if (current_symbol == "@character_hue")
                                            current_page->character_hue = this->read_integer();
                                        else if (current_symbol == "@opacity")
                                            current_page->opacity = this->read_integer();
                                        else if (current_symbol == "@character_name")
                                            current_page->character_name = this->read_string();
                                    }
                                }
                                else throw getException("event page parameters missing");
                            }
                            current_event->pages.append(current_page);
                        }

                    }
                    else throw getException("current event parameters missing");
                }

                map->events.append(current_event);
            }

        }

    }


    this->file.close();
}

void IOMapFile::write_to_file(QString path)
{

}
