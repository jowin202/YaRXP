#include "ioitemfile.h"


IOItemFile::IOItemFile()
{

}

IOItemFile::IOItemFile(QString path, QList<RPGItem *> *item_list)
{
    this->last_visited_function = "IOItemFile";
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
        RPGItem *current_item = new RPGItem;

        QVariantList list = read_object();
        if (list.at(0).toString() != "RPG::Item")
            throw getException("RPG::Item expected");

        for (int j = 0; j < list.at(1).toInt(); j++)
        {
            QString current_symbol = read_symbol_or_link();
            current_item->param_order << current_symbol;


            if (current_symbol == "@id")
                current_item->id = this->read_integer();
            else if (current_symbol == "@price")
                current_item->price = this->read_integer();
            else if (current_symbol == "@consumable")
                current_item->consumable = this->read_bool(); //BOOL not INT
            else if (current_symbol == "@parameter_type")
                current_item->parameter_type = this->read_integer();
            else if (current_symbol == "@parameter_points")
                current_item->parameter_points = this->read_integer();
            else if (current_symbol == "@recover_hp_rate")
                current_item->recover_hp_rate = this->read_integer();
            else if (current_symbol == "@recover_hp")
                current_item->recover_hp = this->read_integer();
            else if (current_symbol == "@recover_sp_rate")
                current_item->recover_sp_rate = this->read_integer();
            else if (current_symbol == "@recover_sp")
                current_item->recover_sp = this->read_integer();
            else if (current_symbol == "@hit")
                current_item->hit = this->read_integer();
            else if (current_symbol == "@pdef_f")
                current_item->pdef_f = this->read_integer();
            else if (current_symbol == "@mdef_f")
                current_item->mdef_f = this->read_integer();
            else if (current_symbol == "@variance")
                current_item->variance = this->read_integer();
            else if (current_symbol == "@scope")
                current_item->scope = this->read_integer();
            else if (current_symbol == "@occasion")
                current_item->occasion = this->read_integer();
            else if (current_symbol == "@animation1_id")
                current_item->animation1_id = this->read_integer();
            else if (current_symbol == "@animation2_id")
                current_item->animation2_id = this->read_integer();
            else if (current_symbol == "@common_event_id")
                current_item->common_event_id = this->read_integer();
            else if (current_symbol == "@name")
                current_item->name = this->read_string();
            else if (current_symbol == "@icon_name")
                current_item->icon_name = this->read_string();
            else if (current_symbol == "@description")
                current_item->description = this->read_string();
            else if (current_symbol == "@menu_se")
                this->read_audiofile_object(&current_item->menu_se);
            else if (current_symbol == "@element_set")
            {
                int size = this->read_array();
                for (int e = 0; e < size; e++)
                    current_item->element_set.append(this->read_integer());
            }
            else if (current_symbol == "@plus_state_set")
            {
                int size = this->read_array();
                for (int e = 0; e < size; e++)
                    current_item->plus_state_set.append(this->read_integer());
            }
            else if (current_symbol == "@minus_state_set")
            {
                int size = this->read_array();
                for (int e = 0; e < size; e++)
                    current_item->minus_state_set.append(this->read_integer());
            }
        }
        item_list->append(current_item);
    }
    this->file.close();
}

void IOItemFile::write_to_file(QString path, QList<RPGItem *> *item_list)
{
    this->last_visited_function = "write_to_file";
    this->symbol_cache.clear();
    this->object_count = 0;

    if (!path.isEmpty())
        this->path = path;
    this->file.setFileName(path);
    this->file.open(QIODevice::WriteOnly);
    this->write_header();

    this->write_array(item_list->length()+1); //zero at beginning
    this->write_one_byte((int)'0');

    for (int i = 0; i < item_list->length(); i++)
    {
        RPGItem *current_item = item_list->at(i);
        this->write_object("RPG::Item", 25);
        for (int j = 0; j < current_item->param_order.length(); j++)
        {
            QString current_symbol = current_item->param_order.at(j);
            this->write_symbol_or_link(current_symbol);

            if (current_symbol == "@id")
                this->write_integer(current_item->id);
            else if (current_symbol == "@price")
                this->write_integer(current_item->price);
            else if (current_symbol == "@consumable")
                this->write_bool(current_item->consumable); // BOOL not INT
            else if (current_symbol == "@parameter_type")
                this->write_integer(current_item->parameter_type);
            else if (current_symbol == "@parameter_points")
                this->write_integer(current_item->parameter_points);
            else if (current_symbol == "@recover_hp_rate")
                this->write_integer(current_item->recover_hp_rate);
            else if (current_symbol == "@recover_hp")
                this->write_integer(current_item->recover_hp);
            else if (current_symbol == "@recover_sp_rate")
                this->write_integer(current_item->recover_sp_rate);
            else if (current_symbol == "@recover_sp")
                this->write_integer(current_item->recover_sp);
            else if (current_symbol == "@hit")
                this->write_integer(current_item->hit);
            else if (current_symbol == "@pdef_f")
                this->write_integer(current_item->pdef_f);
            else if (current_symbol == "@mdef_f")
                this->write_integer(current_item->mdef_f);
            else if (current_symbol == "@variance")
                this->write_integer(current_item->variance);
            else if (current_symbol == "@scope")
                this->write_integer(current_item->scope);
            else if (current_symbol == "@occasion")
                this->write_integer(current_item->occasion);
            else if (current_symbol == "@animation1_id")
                this->write_integer(current_item->animation1_id);
            else if (current_symbol == "@animation2_id")
                this->write_integer(current_item->animation2_id);
            else if (current_symbol == "@common_event_id")
                this->write_integer(current_item->common_event_id);
            else if (current_symbol == "@name")
                this->write_string(current_item->name);
            else if (current_symbol == "@icon_name")
                this->write_string(current_item->icon_name);
            else if (current_symbol == "@description")
                this->write_string(current_item->description);
            else if (current_symbol == "@menu_se")
                this->write_audiofile_object(&current_item->menu_se);
            else if (current_symbol == "@element_set")
            {
                this->write_array(current_item->element_set.size());
                for (int e = 0; e < current_item->element_set.size(); e++)
                    this->write_integer(current_item->element_set.at(e));
            }
            else if (current_symbol == "@plus_state_set")
            {
                this->write_array(current_item->plus_state_set.size());
                for (int e = 0; e < current_item->plus_state_set.size(); e++)
                    this->write_integer(current_item->plus_state_set.at(e));
            }
            else if (current_symbol == "@minus_state_set")
            {
                this->write_array(current_item->minus_state_set.size());
                for (int e = 0; e < current_item->minus_state_set.size(); e++)
                    this->write_integer(current_item->minus_state_set.at(e));
            }

        }
    }
    this->file.close();
}
