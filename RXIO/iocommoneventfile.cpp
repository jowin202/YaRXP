#include "iocommoneventfile.h"

IOCommonEventFile::IOCommonEventFile()
{

}

IOCommonEventFile::IOCommonEventFile(QString path, QList<RPGCommonEvent *> *common_event_list)
{
    this->last_visited_function = "IOCommonEventFile";
    this->symbol_cache.clear();
    this->object_count = 0;

    this->path = path;
    this->file.setFileName(path);
    this->file.open(QIODevice::ReadOnly);

    check_header();
    int length = read_array();
    this->read_one_byte(); //withdraw first zero

    for (int i = 1 ; i < length; i++)
    {
        RPGCommonEvent *current_common_event = new RPGCommonEvent;

        QVariantList list = read_object();
        if (list.at(0).toString() != "RPG::CommonEvent")
            throw getException("RPG::CommonEvent expected");

        for (int j = 0; j < list.at(1).toInt(); j++)
        {
            QString current_symbol = read_symbol_or_link();
            current_common_event->param_order << current_symbol;


            if (current_symbol == "@id")
                current_common_event->id = this->read_integer();
            else if (current_symbol == "@switch_id")
                current_common_event->switch_id = this->read_integer();
            else if (current_symbol == "@trigger")
                current_common_event->trigger = this->read_integer();
            else if (current_symbol == "@name")
                current_common_event->name = this->read_string();
            else if (current_symbol == "@list")
                this->read_event_list(&current_common_event->list);

        }
        common_event_list->append(current_common_event);
    }
    this->file.close();
}

void IOCommonEventFile::write_to_file(QString path, QList<RPGCommonEvent *> *common_event_list)
{
    this->last_visited_function = "write_to_file";
    this->symbol_cache.clear();
    this->object_count = 0;

    if (!path.isEmpty())
        this->path = path;
    this->file.setFileName(path);
    this->file.open(QIODevice::WriteOnly);
    this->write_header();

    this->write_array(common_event_list->length()+1); //zero at beginning + ref list
    this->write_one_byte((int)'0');

    for (int i = 0; i < common_event_list->length(); i++)
    {
        RPGCommonEvent *current_common_event = common_event_list->at(i);
        this->write_object("RPG::CommonEvent", 5);
        for (int j = 0; j < current_common_event->param_order.length(); j++)
        {
            QString current_symbol = current_common_event->param_order.at(j);
            this->write_symbol_or_link(current_symbol);

            if (current_symbol == "@id")
                this->write_integer(current_common_event->id);
            else if (current_symbol == "@switch_id")
                this->write_integer(current_common_event->switch_id);
            else if (current_symbol == "@trigger")
                this->write_integer(current_common_event->trigger);
            else if (current_symbol == "@name")
                this->write_string(current_common_event->name);
            else if (current_symbol == "@list")
                this->write_event_list(&current_common_event->list);
        }
    }


    this->file.close();
}
