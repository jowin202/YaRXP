#include "ioarmorfile.h"

IOArmorFile::IOArmorFile()
{

}

IOArmorFile::IOArmorFile(QString path, QList<RPGArmor *> *armor_list)
{
    this->last_visited_function = "IOArmorFile";
    this->symbol_cache.clear();

    this->path = path;
    this->file.setFileName(path);
    this->file.open(QIODevice::ReadOnly);

    check_header();
    int length = read_array();
    this->read_one_byte(); //withdraw first zero

    for (int i = 1 ; i < length; i++)
    {
        RPGArmor *current_armor = new RPGArmor;

        QVariantList list = read_object();
        if (list.at(0).toString() != "RPG::Armor")
            throw getException("RPG::Skill expected");

        for (int j = 0; j < list.at(1).toInt(); j++)
        {
            QString current_symbol = read_symbol_or_link();
            current_armor->param_order << current_symbol;


            if (current_symbol == "@id")
                current_armor->id = this->read_integer();
            else if (current_symbol == "@kind")
                current_armor->kind = this->read_integer();
            else if (current_symbol == "@auto_state_id")
                current_armor->auto_state_id = this->read_integer();
            else if (current_symbol == "@price")
                current_armor->price = this->read_integer();
            else if (current_symbol == "@pdef")
                current_armor->pdef = this->read_integer();
            else if (current_symbol == "@mdef")
                current_armor->mdef = this->read_integer();
            else if (current_symbol == "@eva")
                current_armor->eva = this->read_integer();
            else if (current_symbol == "@str_plus")
                current_armor->str_plus = this->read_integer();
            else if (current_symbol == "@dex_plus")
                current_armor->dex_plus = this->read_integer();
            else if (current_symbol == "@agi_plus")
                current_armor->agi_plus = this->read_integer();
            else if (current_symbol == "@int_plus")
                current_armor->int_plus = this->read_integer();

            else if (current_symbol == "@name")
                current_armor->name = this->read_string();
            else if (current_symbol == "@icon_name")
                current_armor->icon_name = this->read_string();
            else if (current_symbol == "@description")
                current_armor->description = this->read_string();
            else if (current_symbol == "@guard_element_set")
            {
                int size = this->read_array();
                for (int e = 0; e < size; e++)
                    current_armor->guard_element_set.append(this->read_integer());
            }
            else if (current_symbol == "@guard_state_set")
            {
                int size = this->read_array();
                for (int e = 0; e < size; e++)
                    current_armor->guard_state_set.append(this->read_integer());
            }
        }
        armor_list->append(current_armor);
    }
    this->file.close();
}

void IOArmorFile::write_to_file(QString path, QList<RPGArmor *> *armor_list)
{
    this->last_visited_function = "write_to_file";
    this->symbol_cache.clear();

    if (!path.isEmpty())
        this->path = path;
    this->file.setFileName(path);
    this->file.open(QIODevice::WriteOnly);
    this->write_header();

    this->write_array(armor_list->length()+1); //zero at beginning
    this->write_one_byte((int)'0');

    for (int i = 0; i < armor_list->length(); i++)
    {
        RPGArmor *current_armor = armor_list->at(i);
        this->write_object("RPG::Armor", 16);
        for (int j = 0; j < current_armor->param_order.length(); j++)
        {
            QString current_symbol = current_armor->param_order.at(j);
            this->write_symbol_or_link(current_symbol);

            if (current_symbol == "@id")
                this->write_integer(current_armor->id);
            else if (current_symbol == "@kind")
                this->write_integer(current_armor->kind);
            else if (current_symbol == "@auto_state_id")
                this->write_integer(current_armor->auto_state_id);
            else if (current_symbol == "@price")
                this->write_integer(current_armor->price);
            else if (current_symbol == "@pdef")
                this->write_integer(current_armor->pdef);
            else if (current_symbol == "@mdef")
                this->write_integer(current_armor->mdef);
            else if (current_symbol == "@eva")
                this->write_integer(current_armor->eva);
            else if (current_symbol == "@str_plus")
                this->write_integer(current_armor->str_plus);
            else if (current_symbol == "@dex_plus")
                this->write_integer(current_armor->dex_plus);
            else if (current_symbol == "@agi_plus")
                this->write_integer(current_armor->agi_plus);
            else if (current_symbol == "@int_plus")
                this->write_integer(current_armor->int_plus);
            else if (current_symbol == "@name")
                this->write_string(current_armor->name);
            else if (current_symbol == "@icon_name")
                this->write_string(current_armor->icon_name);
            else if (current_symbol == "@description")
                this->write_string(current_armor->description);
            else if (current_symbol == "@guard_element_set")
            {
                this->write_array(current_armor->guard_element_set.size());
                for (int e = 0; e < current_armor->guard_element_set.size(); e++)
                    this->write_integer(current_armor->guard_element_set.at(e));
            }

            else if (current_symbol == "@guard_state_set")
            {
                this->write_array(current_armor->guard_state_set.size());
                for (int e = 0; e < current_armor->guard_state_set.size(); e++)
                    this->write_integer(current_armor->guard_state_set.at(e));
            }

        }
    }
    this->file.close();
}
