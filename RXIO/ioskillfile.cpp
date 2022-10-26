#include "ioskillfile.h"

IOSKillFile::IOSKillFile()
{

}

IOSKillFile::IOSKillFile(QString path, QList<RPGSkill *> *skill_list)
{
    this->last_visited_function = "IOSKillFile";
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
        RPGSkill *current_skill = new RPGSkill;

        QVariantList list = read_object();
        if (list.at(0).toString() != "RPG::Skill")
            throw getException("RPG::Skill expected");

        for (int j = 0; j < list.at(1).toInt(); j++)
        {
            QString current_symbol = read_symbol_or_link();
            current_skill->param_order << current_symbol;


            if (current_symbol == "@id")
                current_skill->id = this->read_integer();
            else if (current_symbol == "@sp_cost")
                current_skill->sp_cost = this->read_integer();
            else if (current_symbol == "@power")
                current_skill->power = this->read_integer();
            else if (current_symbol == "@atk_f")
                current_skill->atk_f = this->read_integer();
            else if (current_symbol == "@eva_f")
                current_skill->eva_f = this->read_integer();
            else if (current_symbol == "@str_f")
                current_skill->str_f = this->read_integer();
            else if (current_symbol == "@dex_f")
                current_skill->dex_f = this->read_integer();
            else if (current_symbol == "@agi_f")
                current_skill->agi_f = this->read_integer();
            else if (current_symbol == "@int_f")
                current_skill->int_f = this->read_integer();
            else if (current_symbol == "@hit")
                current_skill->hit = this->read_integer();
            else if (current_symbol == "@pdef_f")
                current_skill->pdef_f = this->read_integer();
            else if (current_symbol == "@mdef_f")
                current_skill->mdef_f = this->read_integer();
            else if (current_symbol == "@variance")
                current_skill->variance = this->read_integer();
            else if (current_symbol == "@scope")
                current_skill->scope = this->read_integer();
            else if (current_symbol == "@occasion")
                current_skill->occasion = this->read_integer();
            else if (current_symbol == "@animation1_id")
                current_skill->animation1_id = this->read_integer();
            else if (current_symbol == "@animation2_id")
                current_skill->animation2_id = this->read_integer();
            else if (current_symbol == "@common_event_id")
                current_skill->common_event_id = this->read_integer();
            else if (current_symbol == "@name")
                current_skill->name = this->read_string();
            else if (current_symbol == "@icon_name")
                current_skill->icon_name = this->read_string();
            else if (current_symbol == "@description")
                current_skill->description = this->read_string();
            else if (current_symbol == "@menu_se")
                this->read_audiofile_object(&current_skill->menu_se);
            else if (current_symbol == "@element_set")
            {
                int size = this->read_array();
                for (int e = 0; e < size; e++)
                    current_skill->element_set.append(this->read_integer());
            }
            else if (current_symbol == "@plus_state_set")
            {
                int size = this->read_array();
                for (int e = 0; e < size; e++)
                    current_skill->plus_state_set.append(this->read_integer());
            }
            else if (current_symbol == "@minus_state_set")
            {
                int size = this->read_array();
                for (int e = 0; e < size; e++)
                    current_skill->minus_state_set.append(this->read_integer());
            }
        }
        skill_list->append(current_skill);
    }
    this->file.close();
}

void IOSKillFile::write_to_file(QString path, QList<RPGSkill *> *skill_list)
{
    this->last_visited_function = "write_to_file";
    this->symbol_cache.clear();
    this->object_count = 0;

    if (!path.isEmpty())
        this->path = path;
    this->file.setFileName(path);
    this->file.open(QIODevice::WriteOnly);
    this->write_header();

    this->write_array(skill_list->length()+1); //zero at beginning
    this->write_one_byte((int)'0');

    for (int i = 0; i < skill_list->length(); i++)
    {
        RPGSkill *current_skill = skill_list->at(i);
        this->write_object("RPG::Skill", 25);
        for (int j = 0; j < current_skill->param_order.length(); j++)
        {
            QString current_symbol = current_skill->param_order.at(j);
            this->write_symbol_or_link(current_symbol);

            if (current_symbol == "@id")
                this->write_integer(current_skill->id);
            else if (current_symbol == "@sp_cost")
                this->write_integer(current_skill->sp_cost);
            else if (current_symbol == "@power")
                this->write_integer(current_skill->power);
            else if (current_symbol == "@atk_f")
                this->write_integer(current_skill->atk_f);
            else if (current_symbol == "@eva_f")
                this->write_integer(current_skill->eva_f);
            else if (current_symbol == "@str_f")
                this->write_integer(current_skill->str_f);
            else if (current_symbol == "@dex_f")
                this->write_integer(current_skill->dex_f);
            else if (current_symbol == "@agi_f")
                this->write_integer(current_skill->agi_f);
            else if (current_symbol == "@int_f")
                this->write_integer(current_skill->int_f);
            else if (current_symbol == "@hit")
                this->write_integer(current_skill->hit);
            else if (current_symbol == "@pdef_f")
                this->write_integer(current_skill->pdef_f);
            else if (current_symbol == "@mdef_f")
                this->write_integer(current_skill->mdef_f);
            else if (current_symbol == "@variance")
                this->write_integer(current_skill->variance);
            else if (current_symbol == "@scope")
                this->write_integer(current_skill->scope);
            else if (current_symbol == "@occasion")
                this->write_integer(current_skill->occasion);
            else if (current_symbol == "@animation1_id")
                this->write_integer(current_skill->animation1_id);
            else if (current_symbol == "@animation2_id")
                this->write_integer(current_skill->animation2_id);
            else if (current_symbol == "@common_event_id")
                this->write_integer(current_skill->common_event_id);
            else if (current_symbol == "@name")
                this->write_string(current_skill->name);
            else if (current_symbol == "@icon_name")
                this->write_string(current_skill->icon_name);
            else if (current_symbol == "@description")
                this->write_string(current_skill->description);
            else if (current_symbol == "@menu_se")
                this->write_audiofile_object(&current_skill->menu_se);
            else if (current_symbol == "@element_set")
            {
                this->write_array(current_skill->element_set.size());
                for (int e = 0; e < current_skill->element_set.size(); e++)
                    this->write_integer(current_skill->element_set.at(e));
            }
            else if (current_symbol == "@plus_state_set")
            {
                this->write_array(current_skill->plus_state_set.size());
                for (int e = 0; e < current_skill->plus_state_set.size(); e++)
                    this->write_integer(current_skill->plus_state_set.at(e));
            }
            else if (current_symbol == "@minus_state_set")
            {
                this->write_array(current_skill->minus_state_set.size());
                for (int e = 0; e < current_skill->minus_state_set.size(); e++)
                    this->write_integer(current_skill->minus_state_set.at(e));
            }

        }
    }
    this->file.close();
}
