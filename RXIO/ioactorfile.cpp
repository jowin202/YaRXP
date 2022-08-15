#include "ioactorfile.h"

IOActorFile::IOActorFile()
{

}

IOActorFile::IOActorFile(QString path, QList<RPGActor *> *actor_list)
{
    this->last_visited_function = "IOActorFile";
    this->symbol_cache.clear();

    this->path = path;
    this->file.setFileName(path);
    this->file.open(QIODevice::ReadOnly);

    check_header();
    int length = read_array();
    this->read_one_byte(); //withdraw first zero

    for (int i = 1 ; i < length; i++)
    {
        RPGActor *current_actor = new RPGActor;

        QVariantList list = read_object();
        if (list.at(0).toString() != "RPG::Actor")
            throw getException("RPG::Actor expected");

        for (int j = 0; j < list.at(1).toInt(); j++)
        {
            QString current_symbol = read_symbol_or_link();
            current_actor->param_order << current_symbol;

            if (current_symbol == "@id")
                current_actor->id = this->read_integer();
            else if (current_symbol == "@initial_level")
                current_actor->initial_level = this->read_integer();
            else if (current_symbol == "@final_level")
                current_actor->final_level = this->read_integer();
            else if (current_symbol == "@character_hue")
                current_actor->character_hue = this->read_integer();
            else if (current_symbol == "@battler_hue")
                current_actor->battler_hue = this->read_integer();
            else if (current_symbol == "@class_id")
                current_actor->class_id = this->read_integer();
            else if (current_symbol == "@exp_basis")
                current_actor->exp_basis = this->read_integer();
            else if (current_symbol == "@exp_inflation")
                current_actor->exp_inflation = this->read_integer();
            else if (current_symbol == "@weapon_id")
                current_actor->weapon_id = this->read_integer();
            else if (current_symbol == "@armor1_id")
                current_actor->armor1_id = this->read_integer();
            else if (current_symbol == "@armor2_id")
                current_actor->armor2_id = this->read_integer();
            else if (current_symbol == "@armor3_id")
                current_actor->armor3_id = this->read_integer();
            else if (current_symbol == "@armor4_id")
                current_actor->armor4_id = this->read_integer();
            else if (current_symbol == "@armor1_fix")
                current_actor->armor1_fix = this->read_bool();
            else if (current_symbol == "@armor2_fix")
                current_actor->armor2_fix = this->read_bool();
            else if (current_symbol == "@armor3_fix")
                current_actor->armor3_fix = this->read_bool();
            else if (current_symbol == "@armor4_fix")
                current_actor->armor4_fix = this->read_bool();
            else if (current_symbol == "@weapon_fix")
                current_actor->weapon_fix = this->read_bool();
            else if (current_symbol == "@battler_name")
                current_actor->battler_name = this->read_string();
            else if (current_symbol == "@character_name")
                current_actor->character_name = this->read_string();
            else if (current_symbol == "@name")
                current_actor->name = this->read_string();
            else if (current_symbol == "@parameters")
                this->read_table(&current_actor->parameters);
        }
        actor_list->append(current_actor);
    }

    this->file.close();
}

void IOActorFile::write_to_file(QString path, QList<RPGActor *> *actor_list)
{
    this->last_visited_function = "write_to_file";
    this->symbol_cache.clear();

    if (!path.isEmpty())
        this->path = path;
    this->file.setFileName(path);
    this->file.open(QIODevice::WriteOnly);
    this->write_header();

    this->write_array(actor_list->length()+1); //zero at beginning
    this->write_one_byte((int)'0');

    for (int i = 0; i < actor_list->length(); i++)
    {
        RPGActor *current_actor = actor_list->at(i);
        this->write_object("RPG::Actor", 22);
        for (int j = 0; j < current_actor->param_order.length(); j++)
        {
            QString current_symbol = current_actor->param_order.at(j);
            this->write_symbol_or_link(current_symbol);

            if (current_symbol == "@id")
                this->write_integer(current_actor->id);
            else if (current_symbol == "@initial_level")
                this->write_integer(current_actor->initial_level);
            else if (current_symbol == "@final_level")
                this->write_integer(current_actor->final_level);
            else if (current_symbol == "@character_hue")
                this->write_integer(current_actor->character_hue);
            else if (current_symbol == "@battler_hue")
                this->write_integer(current_actor->battler_hue);
            else if (current_symbol == "@class_id")
                this->write_integer(current_actor->class_id);
            else if (current_symbol == "@exp_basis")
                this->write_integer(current_actor->exp_basis);
            else if (current_symbol == "@exp_inflation")
                this->write_integer(current_actor->exp_inflation);
            else if (current_symbol == "@weapon_id")
                this->write_integer(current_actor->weapon_id);
            else if (current_symbol == "@armor1_id")
                this->write_integer(current_actor->armor1_id);
            else if (current_symbol == "@armor2_id")
                this->write_integer(current_actor->armor2_id);
            else if (current_symbol == "@armor3_id")
                this->write_integer(current_actor->armor3_id);
            else if (current_symbol == "@armor4_id")
                this->write_integer(current_actor->armor4_id);
            else if (current_symbol == "@armor1_fix")
                this->write_bool(current_actor->armor1_fix);
            else if (current_symbol == "@armor2_fix")
                this->write_bool(current_actor->armor2_fix);
            else if (current_symbol == "@armor3_fix")
                this->write_bool(current_actor->armor3_fix);
            else if (current_symbol == "@armor4_fix")
                this->write_bool(current_actor->armor4_fix);
            else if (current_symbol == "@weapon_fix")
                this->write_bool(current_actor->weapon_fix);
            else if (current_symbol == "@battler_name")
                this->write_string(current_actor->battler_name);
            else if (current_symbol == "@character_name")
                this->write_string(current_actor->character_name);
            else if (current_symbol == "@name")
                this->write_string(current_actor->name);
            else if (current_symbol == "@parameters")
                this->write_table(&current_actor->parameters);
        }
    }
    this->file.close();
}
