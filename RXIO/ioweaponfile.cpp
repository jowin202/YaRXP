#include "ioweaponfile.h"

IOWeaponFile::IOWeaponFile()
{

}

IOWeaponFile::IOWeaponFile(QString path, QList<RPGWeapon *> *weapon_list)
{
    this->last_visited_function = "IOWeaponFile";
    this->symbol_cache.clear();

    this->path = path;
    this->file.setFileName(path);
    this->file.open(QIODevice::ReadOnly);

    check_header();
    int length = read_array();
    this->read_one_byte(); //withdraw first zero

    for (int i = 1 ; i < length; i++)
    {
        RPGWeapon *current_weapon = new RPGWeapon;

        QVariantList list = read_object();
        if (list.at(0).toString() != "RPG::Weapon")
            throw getException("RPG::Weapon expected");

        for (int j = 0; j < list.at(1).toInt(); j++)
        {
            QString current_symbol = read_symbol_or_link();
            current_weapon->param_order << current_symbol;


            if (current_symbol == "@id")
                current_weapon->id = this->read_integer();
            else if (current_symbol == "@animation1_id")
                current_weapon->animation1_id = this->read_integer();
            else if (current_symbol == "@animation2_id")
                current_weapon->animation2_id = this->read_integer();
            else if (current_symbol == "@price")
                current_weapon->price = this->read_integer();
            else if (current_symbol == "@pdef")
                current_weapon->pdef = this->read_integer();
            else if (current_symbol == "@mdef")
                current_weapon->mdef = this->read_integer();
            else if (current_symbol == "@atk")
                current_weapon->atk = this->read_integer();
            else if (current_symbol == "@str_plus")
                current_weapon->str_plus = this->read_integer();
            else if (current_symbol == "@dex_plus")
                current_weapon->dex_plus = this->read_integer();
            else if (current_symbol == "@agi_plus")
                current_weapon->agi_plus = this->read_integer();
            else if (current_symbol == "@int_plus")
                current_weapon->int_plus = this->read_integer();

            else if (current_symbol == "@name")
                current_weapon->name = this->read_string();
            else if (current_symbol == "@icon_name")
                current_weapon->icon_name = this->read_string();
            else if (current_symbol == "@description")
                current_weapon->description = this->read_string();
            else if (current_symbol == "@element_set")
            {
                int size = this->read_array();
                for (int e = 0; e < size; e++)
                    current_weapon->element_set.append(this->read_integer());
            }
            else if (current_symbol == "@minus_state_set")
            {
                int size = this->read_array();
                for (int e = 0; e < size; e++)
                    current_weapon->minus_state_set.append(this->read_integer());
            }
            else if (current_symbol == "@plus_state_set")
            {
                int size = this->read_array();
                for (int e = 0; e < size; e++)
                    current_weapon->plus_state_set.append(this->read_integer());
            }
        }
        weapon_list->append(current_weapon);
    }
    this->file.close();
}

void IOWeaponFile::write_to_file(QString path, QList<RPGWeapon *> *weapon_list)
{
    this->last_visited_function = "write_to_file";
    this->symbol_cache.clear();

    if (!path.isEmpty())
        this->path = path;
    this->file.setFileName(path);
    this->file.open(QIODevice::WriteOnly);
    this->write_header();

    this->write_array(weapon_list->length()+1); //zero at beginning
    this->write_one_byte((int)'0');

    for (int i = 0; i < weapon_list->length(); i++)
    {
        RPGWeapon *current_weapon = weapon_list->at(i);
        this->write_object("RPG::Weapon", 17);
        for (int j = 0; j < current_weapon->param_order.length(); j++)
        {
            QString current_symbol = current_weapon->param_order.at(j);
            this->write_symbol_or_link(current_symbol);

            if (current_symbol == "@id")
                this->write_integer(current_weapon->id);
            else if (current_symbol == "@animation1_id")
                this->write_integer(current_weapon->animation1_id);
            else if (current_symbol == "@animation2_id")
                this->write_integer(current_weapon->animation2_id);
            else if (current_symbol == "@price")
                this->write_integer(current_weapon->price);
            else if (current_symbol == "@pdef")
                this->write_integer(current_weapon->pdef);
            else if (current_symbol == "@mdef")
                this->write_integer(current_weapon->mdef);
            else if (current_symbol == "@atk")
                this->write_integer(current_weapon->atk);
            else if (current_symbol == "@str_plus")
                this->write_integer(current_weapon->str_plus);
            else if (current_symbol == "@dex_plus")
                this->write_integer(current_weapon->dex_plus);
            else if (current_symbol == "@agi_plus")
                this->write_integer(current_weapon->agi_plus);
            else if (current_symbol == "@int_plus")
                this->write_integer(current_weapon->int_plus);
            else if (current_symbol == "@name")
                this->write_string(current_weapon->name);
            else if (current_symbol == "@icon_name")
                this->write_string(current_weapon->icon_name);
            else if (current_symbol == "@description")
                this->write_string(current_weapon->description);
            else if (current_symbol == "@element_set")
            {
                this->write_array(current_weapon->element_set.size());
                for (int e = 0; e < current_weapon->element_set.size(); e++)
                    this->write_integer(current_weapon->element_set.at(e));
            }
            else if (current_symbol == "@plus_state_set")
            {
                this->write_array(current_weapon->plus_state_set.size());
                for (int e = 0; e < current_weapon->plus_state_set.size(); e++)
                    this->write_integer(current_weapon->plus_state_set.at(e));
            }
            else if (current_symbol == "@minus_state_set")
            {
                this->write_array(current_weapon->minus_state_set.size());
                for (int e = 0; e < current_weapon->minus_state_set.size(); e++)
                    this->write_integer(current_weapon->minus_state_set.at(e));
            }

        }
    }
    this->file.close();
}
