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
        map->param_order.append(current_symbol);

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
                    current_event->param_order.append(current_symbol);

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
                                current_page->param_order.append(current_symbol);

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
                                                        this->read_tone(&current_event_command->red, &current_event_command->green,
                                                                        &current_event_command->blue, &current_event_command->gray);
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
                                        current_event_command->debug();
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
                                        current_page->condition_param_order.append(current_symbol);

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
                                            current_page->self_switch_ch = this->read_string();
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
                                        current_page->graphic_param_order.append(current_symbol);

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

void IOMapFile::write_to_file(QString path, RPGMap *map)
{
    this->last_visited_function = "write_to_file";
    this->symbol_cache.clear();

    if (!path.isEmpty())
        this->path = path;
    this->file.setFileName(path);
    this->file.open(QIODevice::WriteOnly);
    this->write_header();

    this->write_object("RPG::Map", 11);

    this->write_symbol_or_link("@bgm");
    this->write_audiofile_object(&map->bgm);

    this->write_symbol_or_link("@events");
    this->write_list(map->events.length());
    for (int i = 0; i < map->events.length(); i++)
    {
        this->write_integer(map->events.at(i)->id); //key
        this->write_object("RPG::Event", 5);

        this->write_symbol_or_link("@pages");
        this->write_array(map->events.at(i)->pages.length());
        for (int j = 0; j < map->events.at(i)->pages.length(); j++)
        {
            this->write_object("RPG::Event::Page", 13);

            this->write_symbol_or_link("@list");
            this->write_array(map->events.at(i)->pages.at(j)->list.length());

            for (int k = 0; k < map->events.at(i)->pages.at(j)->list.length(); k++)
            {
                this->write_object("RPG::EventCommand", 3);

                this->write_symbol_or_link("@parameters");

                if (map->events.at(i)->pages.at(j)->list.at(k)->code == 0) //Empty Event
                {
                    this->write_array(0);
                }
                if (map->events.at(i)->pages.at(j)->list.at(k)->code == 205) //Change Fog Color Tone
                {
                    this->write_array(2); // tone + time(int)
                    this->write_tone(map->events.at(i)->pages.at(j)->list.at(k)->red,map->events.at(i)->pages.at(j)->list.at(k)->green,
                                     map->events.at(i)->pages.at(j)->list.at(k)->blue,map->events.at(i)->pages.at(j)->list.at(k)->gray, true);
                    this->write_integer(map->events.at(i)->pages.at(j)->list.at(k)->parameters.at(0).toInt());
                }

                this->write_symbol_or_link("@indent");
                this->write_integer(map->events.at(i)->pages.at(j)->list.at(k)->indent);
                this->write_symbol_or_link("@code");
                this->write_integer(map->events.at(i)->pages.at(j)->list.at(k)->code);
            }

            this->write_symbol_or_link("@move_type");
            this->write_integer(map->events.at(i)->pages.at(j)->move_type);

            this->write_symbol_or_link("@direction_fix");
            this->write_bool(map->events.at(i)->pages.at(j)->direction_fix);



            this->write_symbol_or_link("@condition");
            this->write_object("RPG::Event::Page::Condition", 9);
            this->write_symbol_or_link("@switch2_valid");
            this->write_bool(map->events.at(i)->pages.at(j)->switch2_valid);
            this->write_symbol_or_link("@self_switch_ch");
            this->write_string(map->events.at(i)->pages.at(j)->self_switch_ch);
            this->write_symbol_or_link("@switch1_id");
            this->write_integer(map->events.at(i)->pages.at(j)->switch1_id);
            this->write_symbol_or_link("@switch1_valid");
            this->write_bool(map->events.at(i)->pages.at(j)->switch1_valid);
            this->write_symbol_or_link("@variable_value");
            this->write_integer(map->events.at(i)->pages.at(j)->variable_value);
            this->write_symbol_or_link("@self_switch_valid");
            this->write_bool(map->events.at(i)->pages.at(j)->self_switch_valid);
            this->write_symbol_or_link("@variable_id");
            this->write_integer(map->events.at(i)->pages.at(j)->variable_id);
            this->write_symbol_or_link("@variable_valid");
            this->write_bool(map->events.at(i)->pages.at(j)->variable_valid);
            this->write_symbol_or_link("@switch2_id");
            this->write_integer(map->events.at(i)->pages.at(j)->switch2_id);



            this->write_symbol_or_link("@move_route");
            this->write_move_route_object(&map->events.at(i)->pages.at(j)->move_route,0);


            this->write_symbol_or_link("@trigger");
            this->write_integer(map->events.at(i)->pages.at(j)->trigger);

            this->write_symbol_or_link("@step_anime");
            this->write_bool(map->events.at(i)->pages.at(j)->step_anime);


            this->write_symbol_or_link("@move_frequency");
            this->write_integer(map->events.at(i)->pages.at(j)->move_frequency);

            this->write_symbol_or_link("@always_on_top");
            this->write_bool(map->events.at(i)->pages.at(j)->always_on_top);



            this->write_symbol_or_link("@graphic");
            this->write_object("RPG::Event::Page::Graphic", 7);
            this->write_symbol_or_link("@opacity");
            this->write_integer(map->events.at(i)->pages.at(j)->opacity);
            this->write_symbol_or_link("@character_name");
            this->write_string(map->events.at(i)->pages.at(j)->character_name);
            this->write_symbol_or_link("@pattern");
            this->write_integer(map->events.at(i)->pages.at(j)->pattern);
            this->write_symbol_or_link("@tile_id");
            this->write_integer(map->events.at(i)->pages.at(j)->tile_id);
            this->write_symbol_or_link("@direction");
            this->write_integer(map->events.at(i)->pages.at(j)->direction);
            this->write_symbol_or_link("@blend_type");
            this->write_integer(map->events.at(i)->pages.at(j)->blend_type);
            this->write_symbol_or_link("@character_hue");
            this->write_integer(map->events.at(i)->pages.at(j)->character_hue);


            this->write_symbol_or_link("@walk_anime");
            this->write_bool(map->events.at(i)->pages.at(j)->walk_anime);
            this->write_symbol_or_link("@move_speed");
            this->write_integer(map->events.at(i)->pages.at(j)->move_speed);
            this->write_symbol_or_link("@through");
            this->write_bool(map->events.at(i)->pages.at(j)->through);
        }

        this->write_symbol_or_link("@name");
        this->write_string(map->events.at(i)->name);

        this->write_symbol_or_link("@y");
        this->write_integer(map->events.at(i)->y);

        this->write_symbol_or_link("@x");
        this->write_integer(map->events.at(i)->x);

        this->write_symbol_or_link("@id");
        this->write_integer(map->events.at(i)->id);
    }

    this->write_symbol_or_link("@tileset_id");
    this->write_integer(map->tileset_id);

    this->write_symbol_or_link("@bgs");
    this->write_audiofile_object(&map->bgs);


    this->write_symbol_or_link("@autoplay_bgm");
    this->write_bool(map->autoplay_bgm);


    this->write_symbol_or_link("@data");
    this->write_table_for_map(&map->data, map->height, map->width);



    this->write_symbol_or_link("@autoplay_bgs");
    this->write_bool(map->autoplay_bgs);

    this->write_symbol_or_link("@height");
    this->write_integer(map->height);

    this->write_symbol_or_link("@encounter_step");
    this->write_integer(map->encounter_step);

    this->write_symbol_or_link("@width");
    this->write_integer(map->width);

    this->write_symbol_or_link("@encounter_list");
    this->write_array(map->encounter_list.length());
    for (int i = 0; i < map->encounter_list.length(); i++)
        this->write_integer(map->encounter_list.at(i));


    this->file.close();
}






void IOMapFile::write_to_file_with_order(QString path, RPGMap *map)
{
    this->last_visited_function = "write_to_file_with_order";
    this->symbol_cache.clear();
    this->object_count = 0;

    if (!path.isEmpty())
        this->path = path;
    this->file.setFileName(path);
    this->file.open(QIODevice::WriteOnly);
    this->write_header();


    this->write_object("RPG::Map", 11);

    for (int p = 0; p < map->param_order.length(); p++)
    {
        QString current_symbol = map->param_order.at(p);
        this->write_symbol_or_link(current_symbol);

        if (current_symbol == "@bgm")
            this->write_audiofile_object(&map->bgm);
        else if (current_symbol == "@bgs")
            this->write_audiofile_object(&map->bgs);
        else if (current_symbol == "@autoplay_bgm")
            this->write_bool(map->autoplay_bgm);
        else if (current_symbol == "@autoplay_bgs")
            this->write_bool(map->autoplay_bgs);
        else if (current_symbol == "@tileset_id")
            this->write_integer(map->tileset_id);
        else if (current_symbol == "@height")
            this->write_integer(map->height);
        else if (current_symbol == "@width")
            this->write_integer(map->width);
        else if (current_symbol == "@encounter_step")
            this->write_integer(map->encounter_step);
        else if (current_symbol == "@encounter_list")
        {
            this->write_array(map->encounter_list.length());
            for (int i = 0; i < map->encounter_list.length(); i++)
                this->write_integer(map->encounter_list.at(i));
        }
        else if (current_symbol == "@data")
            this->write_table_for_map(&map->data, map->height, map->width);
        else if (current_symbol == "@events")
        {
            this->write_list(map->events.length());
            for (int i = 0; i < map->events.length(); i++)
            {
                this->write_integer(map->events.at(i)->id); //key
                RPGEvent *current_event = map->events.at(i);
                this->write_object("RPG::Event", 5);


                for (int q = 0; q < current_event->param_order.length(); q++)
                {
                    QString current_symbol = current_event->param_order.at(q);
                    this->write_symbol_or_link(current_symbol);

                    if (current_symbol == "@x")
                        this->write_integer(current_event->x);
                    else if (current_symbol == "@y")
                        this->write_integer(current_event->y);
                    else if (current_symbol == "@id")
                        this->write_integer(current_event->id);
                    else if (current_symbol == "@name")
                        this->write_string(current_event->name);
                    else if (current_symbol == "@pages")
                    {
                        this->write_array(current_event->pages.length());
                        for (int j = 0; j < current_event->pages.length(); j++)
                        {
                            this->write_object("RPG::Event::Page", 13);
                            RPGEventPage *current_page = current_event->pages.at(j);

                            for (int r = 0; r < current_page->param_order.length(); r++)
                            {
                                QString current_symbol = current_page->param_order.at(r);
                                this->write_symbol_or_link(current_symbol);

                                if (current_symbol == "@direction_fix")
                                    this->write_bool(current_page->direction_fix);
                                else if (current_symbol == "@step_anime")
                                    this->write_bool(current_page->step_anime);
                                else if (current_symbol == "@always_on_top")
                                    this->write_bool(current_page->always_on_top);
                                else if (current_symbol == "@walk_anime")
                                    this->write_bool(current_page->walk_anime);
                                else if (current_symbol == "@through")
                                    this->write_bool(current_page->through);
                                else if (current_symbol == "@move_type")
                                    this->write_integer(current_page->move_type);
                                else if (current_symbol == "@move_speed")
                                    this->write_integer(current_page->move_speed);
                                else if (current_symbol == "@trigger")
                                    this->write_integer(current_page->trigger);
                                else if (current_symbol == "@move_frequency")
                                    this->write_integer(current_page->move_frequency);
                                else if (current_symbol == "@move_route")
                                    this->write_move_route_object(&current_page->move_route, 0);
                                else if (current_symbol == "@condition")
                                {
                                    this->write_object("RPG::Event::Page::Condition", 9);

                                    for (int s = 0; s < current_page->condition_param_order.length(); s++)
                                    {
                                        QString current_symbol = current_page->condition_param_order.at(s);
                                        this->write_symbol_or_link(current_symbol);

                                        if (current_symbol == "@switch1_valid")
                                            this->write_bool(current_page->switch1_valid);
                                        else if (current_symbol == "@switch2_valid")
                                            this->write_bool(current_page->switch2_valid);
                                        else if (current_symbol == "@variable_valid")
                                            this->write_bool(current_page->variable_valid);
                                        else if (current_symbol == "@switch1_id")
                                            this->write_integer(current_page->switch1_id);
                                        else if (current_symbol == "@switch2_id")
                                            this->write_integer(current_page->switch2_id);
                                        else if (current_symbol == "@variable_id")
                                            this->write_integer(current_page->variable_id);
                                        else if (current_symbol == "@variable_value")
                                            this->write_integer(current_page->variable_value);
                                        else if (current_symbol == "@self_switch_valid")
                                            this->write_bool(current_page->self_switch_valid);
                                        else if (current_symbol == "@self_switch_ch")
                                            this->write_string(current_page->self_switch_ch);

                                    }
                                }
                                else if (current_symbol == "@graphic")
                                {
                                    this->write_object("RPG::Event::Page::Graphic", 7);

                                    for (int t = 0; t < current_page->graphic_param_order.length(); t++)
                                    {
                                        QString current_symbol = current_page->graphic_param_order.at(t);
                                        this->write_symbol_or_link(current_symbol);

                                        if (current_symbol == "@opacity")
                                            this->write_integer(current_page->opacity);
                                        else if (current_symbol == "@pattern")
                                            this->write_integer(current_page->pattern);
                                        else if (current_symbol == "@tile_id")
                                            this->write_integer(current_page->tile_id);
                                        else if (current_symbol == "@direction")
                                            this->write_integer(current_page->direction);
                                        else if (current_symbol == "@blend_type")
                                            this->write_integer(current_page->blend_type);
                                        else if (current_symbol == "@character_hue")
                                            this->write_integer(current_page->character_hue);
                                        else if (current_symbol == "@character_name")
                                            this->write_string(current_page->character_name);
                                    }
                                }
                                else if (current_symbol == "@list")
                                {
                                    this->write_array(current_page->list.length());
                                    for (int k = 0; k < current_page->list.length(); k++)
                                    {
                                        this->write_object("RPG::EventCommand", 3);
                                        RPGEventCommand *current_command = current_page->list.at(k);

                                        this->write_symbol_or_link("@parameters");

                                        //Color Tone
                                        if (current_command->code == 205 || current_command->code == 223)
                                        {
                                            this->write_array(2);
                                            // tone + time(int)
                                            this->write_tone(current_command->red,current_command->green,
                                                             current_command->blue,current_command->gray, true);
                                            this->write_integer(current_command->parameters.at(0).toInt());
                                        }
                                        //screen flash
                                        else if (current_command->code == 224)
                                        {
                                            this->write_array(2);
                                            // Color + time(int)
                                            this->write_tone(current_command->red,current_command->green,
                                                             current_command->blue,current_command->gray, false);
                                            this->write_integer(current_command->parameters.at(0).toInt());
                                        }
                                        else if (current_command->code == 241 || current_command->code == 245
                                                 || current_command->code == 249 || current_command->code ==250 )
                                        {
                                            this->write_array(1);
                                            this->write_audiofile_object(&current_command->audiofile);
                                        }
                                        //show choices
                                        else if (current_command->code == 102)
                                        {
                                            this->write_array(2);
                                            this->write_array(current_command->choices_list.length());
                                            this->choices_reference_stack.push(object_count);
                                            for (int c = 0; c < current_command->choices_list.length(); c++)
                                                this->write_string(current_command->choices_list.at(c));
                                            this->write_integer(current_command->parameters.at(0).toInt());
                                        }
                                        //show choices multiline
                                        else if(current_command->code == 402)
                                        {
                                            this->write_array(2);
                                            this->write_integer(current_command->parameters.at(0).toInt());
                                            int ref = this->choices_reference_stack.pop();
                                            this->file.write("@");
                                            this->write_fixnum(ref);
                                            this->choices_reference_stack.push(++ref);
                                        }
                                        //move route
                                        else if (current_command->code == 209)
                                        {
                                            this->write_array(2);
                                            this->write_integer(current_command->parameters.at(0).toInt());
                                            this->move_route_references.clear();
                                            this->write_move_route_object(&current_command->move_route, &this->move_route_references);
                                            //qDebug() << "len:" << move_route_references.length();
                                        }
                                        //move route multiline
                                        else if (current_command->code == 509)
                                        {
                                            if (this->move_route_references.isEmpty())
                                                throw getException("MoveRoute references() are broken");
                                            this->write_array(1);
                                            this->file.write("@");
                                            this->write_fixnum(this->move_route_references.first());
                                            this->move_route_references.pop_front();
                                        }
                                        else
                                        {
                                            this->write_array(current_command->parameters.length());
                                            for (int z = 0; z < current_command->parameters.length(); z++)
                                                this->write_variant(current_command->parameters.at(z));

                                            if (current_command->code == 404) choices_reference_stack.pop(); //remove it for nested choices
                                        }

                                        this->write_symbol_or_link("@indent");
                                        this->write_integer(current_command->indent);
                                        this->write_symbol_or_link("@code");
                                        this->write_integer(current_command->code);

                                    }
                                } //current_symbol == list ( event list)
                            } //event page parameters
                        } //iterate over event pages
                    } // page
                } //iterate over event parameters
            } //iterate over events
        } //event param
    } //iterate over map params



    /*
    this->write_symbol_or_link("@ref_list");
    this->write_array(414);
    this->write_integer(0);
    for (int i = 1; i <= 413; i++)
    {
        file.write("@");
        this->write_fixnum(i);
    }
    */


    this->file.close();

}
