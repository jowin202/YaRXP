#include "iostatefile.h"

IOStateFile::IOStateFile()
{

}

IOStateFile::IOStateFile(QString path, QList<RPGState *> *state_list)
{
    this->last_visited_function = "IOStateFile";
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
        RPGState *current_state = new RPGState;

        QVariantList list = read_object();
        if (list.at(0).toString() != "RPG::State")
            throw getException("RPG::State expected");

        for (int j = 0; j < list.at(1).toInt(); j++)
        {
            QString current_symbol = read_symbol_or_link();
            current_state->param_order << current_symbol;

            if (current_symbol == "@id")
                current_state->id = this->read_integer();
            if (current_symbol == "@name")
                current_state->name = this->read_string();
            else if (current_symbol == "@animation_id")
                current_state->animation_id = this->read_integer();
            else if (current_symbol == "@restriction")
                current_state->restriction = this->read_integer();
            else if (current_symbol == "@nonresistance")
                current_state->nonresistance = this->read_bool();
            else if (current_symbol == "@zero_hp")
                current_state->zero_hp = this->read_bool();
            else if (current_symbol == "@cant_get_exp")
                current_state->cant_get_exp = this->read_bool();
            else if (current_symbol == "@cant_evade")
                current_state->cant_evade = this->read_bool();
            else if (current_symbol == "@slip_damage")
                current_state->slip_damage = this->read_bool();

            else if (current_symbol == "@rating")
                current_state->rating = this->read_integer();
            else if (current_symbol == "@hit_rate")
                current_state->hit_rate = this->read_integer();
            else if (current_symbol == "@maxhp_rate")
                current_state->maxhp_rate = this->read_integer();
            else if (current_symbol == "@maxsp_rate")
                current_state->maxsp_rate = this->read_integer();
            else if (current_symbol == "@str_rate")
                current_state->str_rate = this->read_integer();
            else if (current_symbol == "@dex_rate")
                current_state->dex_rate = this->read_integer();
            else if (current_symbol == "@agi_rate")
                current_state->agi_rate = this->read_integer();
            else if (current_symbol == "@int_rate")
                current_state->int_rate = this->read_integer();
            else if (current_symbol == "@atk_rate")
                current_state->atk_rate = this->read_integer();
            else if (current_symbol == "@pdef_rate")
                current_state->pdef_rate = this->read_integer();
            else if (current_symbol == "@mdef_rate")
                current_state->mdef_rate = this->read_integer();
            else if (current_symbol == "@eva")
                current_state->eva = this->read_integer();
            else if (current_symbol == "@battle_only")
                current_state->battle_only = this->read_bool();
            else if (current_symbol == "@hold_turn")
                current_state->hold_turn = this->read_integer();
            else if (current_symbol == "@auto_release_prob")
                current_state->auto_release_prob = this->read_integer();
            else if (current_symbol == "@shock_release_prob")
                current_state->shock_release_prob = this->read_integer();
            else if (current_symbol == "@guard_element_set")
            {
                int size = this->read_array();
                for (int e = 0; e < size; e++)
                    current_state->guard_element_set.append(this->read_integer());
            }
            else if (current_symbol == "@minus_state_set")
            {
                int size = this->read_array();
                for (int e = 0; e < size; e++)
                    current_state->minus_state_set.append(this->read_integer());
            }
            else if (current_symbol == "@plus_state_set")
            {
                int size = this->read_array();
                for (int e = 0; e < size; e++)
                    current_state->plus_state_set.append(this->read_integer());
            }
        }
        state_list->append(current_state);
    }
    this->file.close();
}

void IOStateFile::write_to_file(QString path, QList<RPGState *> *state_list)
{
    this->last_visited_function = "write_to_file";
    this->symbol_cache.clear();
    this->object_count = 0;

    if (!path.isEmpty())
        this->path = path;
    this->file.setFileName(path);
    this->file.open(QIODevice::WriteOnly);
    this->write_header();

    this->write_array(state_list->length()+1); //zero at beginning
    this->write_one_byte((int)'0');

    for (int i = 0; i < state_list->length(); i++)
    {
        RPGState *current_state = state_list->at(i);
        this->write_object("RPG::State", 28);
        for (int j = 0; j < current_state->param_order.length(); j++)
        {
            QString current_symbol = current_state->param_order.at(j);
            this->write_symbol_or_link(current_symbol);

            if (current_symbol == "@id")
                this->write_integer(current_state->id);
            if (current_symbol == "@name")
                this->write_string(current_state->name);
            else if (current_symbol == "@animation_id")
                this->write_integer(current_state->animation_id);
            else if (current_symbol == "@restriction")
                this->write_integer(current_state->restriction);
            else if (current_symbol == "@nonresistance")
                this->write_bool(current_state->nonresistance);
            else if (current_symbol == "@zero_hp")
                this->write_bool(current_state->zero_hp);
            else if (current_symbol == "@cant_get_exp")
                this->write_bool(current_state->cant_get_exp);
            else if (current_symbol == "@cant_evade")
                this->write_bool(current_state->cant_evade);
            else if (current_symbol == "@slip_damage")
                this->write_bool(current_state->slip_damage);

            else if (current_symbol == "@rating")
                this->write_integer(current_state->rating);
            else if (current_symbol == "@hit_rate")
                this->write_integer(current_state->hit_rate);
            else if (current_symbol == "@maxhp_rate")
                this->write_integer(current_state->maxhp_rate);
            else if (current_symbol == "@maxsp_rate")
                this->write_integer(current_state->maxsp_rate);
            else if (current_symbol == "@str_rate")
                this->write_integer(current_state->str_rate);
            else if (current_symbol == "@dex_rate")
                this->write_integer(current_state->dex_rate);
            else if (current_symbol == "@agi_rate")
                this->write_integer(current_state->agi_rate);
            else if (current_symbol == "@int_rate")
                this->write_integer(current_state->int_rate);
            else if (current_symbol == "@atk_rate")
                this->write_integer(current_state->atk_rate);
            else if (current_symbol == "@pdef_rate")
                this->write_integer(current_state->pdef_rate);
            else if (current_symbol == "@mdef_rate")
                this->write_integer(current_state->mdef_rate);
            else if (current_symbol == "@eva")
                this->write_integer(current_state->eva);
            else if (current_symbol == "@battle_only")
                this->write_bool(current_state->battle_only);
            else if (current_symbol == "@hold_turn")
                this->write_integer(current_state->hold_turn);
            else if (current_symbol == "@auto_release_prob")
                this->write_integer(current_state->auto_release_prob);
            else if (current_symbol == "@shock_release_prob")
                this->write_integer(current_state->shock_release_prob);
            else if (current_symbol == "@guard_element_set")
            {
                this->write_array(current_state->guard_element_set.size());
                for (int e = 0; e < current_state->guard_element_set.size(); e++)
                    this->write_integer(current_state->guard_element_set.at(e));
            }
            else if (current_symbol == "@plus_state_set")
            {
                this->write_array(current_state->plus_state_set.size());
                for (int e = 0; e < current_state->plus_state_set.size(); e++)
                    this->write_integer(current_state->plus_state_set.at(e));
            }
            else if (current_symbol == "@minus_state_set")
            {
                this->write_array(current_state->minus_state_set.size());
                for (int e = 0; e < current_state->minus_state_set.size(); e++)
                    this->write_integer(current_state->minus_state_set.at(e));
            }

        }
    }
    this->file.close();
}
