#include "ioenemyfile.h"

IOEnemyFile::IOEnemyFile()
{

}

IOEnemyFile::IOEnemyFile(QString path, QList<RPGEnemy *> *enemy_list)
{
    this->last_visited_function = "IOEnemyFile";
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
        RPGEnemy *current_enemy = new RPGEnemy;

        QVariantList list = read_object();
        if (list.at(0).toString() != "RPG::Enemy")
            throw getException("RPG::Enemy expected");

        for (int j = 0; j < list.at(1).toInt(); j++)
        {
            QString current_symbol = read_symbol_or_link();
            current_enemy->param_order << current_symbol;

            if (current_symbol == "@name")
                current_enemy->name = this->read_string();
            else if (current_symbol == "@battler_name")
                current_enemy->battler_name = this->read_string();
            else if (current_symbol == "@battler_hue")
                current_enemy->battler_hue = this->read_integer();
            else if (current_symbol == "@id")
                current_enemy->id = this->read_integer();
            else if (current_symbol == "@maxhp")
                current_enemy->maxhp = this->read_integer();
            else if (current_symbol == "@maxsp")
                current_enemy->maxsp = this->read_integer();
            else if (current_symbol == "@str")
                current_enemy->str = this->read_integer();
            else if (current_symbol == "@dex")
                current_enemy->dex = this->read_integer();
            else if (current_symbol == "@agi")
                current_enemy->agi = this->read_integer();
            else if (current_symbol == "@int")
                current_enemy->int_var = this->read_integer(); // int is not a variable name
            else if (current_symbol == "@atk")
                current_enemy->atk = this->read_integer();
            else if (current_symbol == "@pdef")
                current_enemy->pdef = this->read_integer();
            else if (current_symbol == "@mdef")
                current_enemy->mdef = this->read_integer();
            else if (current_symbol == "@eva")
                current_enemy->eva = this->read_integer();
            else if (current_symbol == "@animation1_id")
                current_enemy->animation1_id = this->read_integer();
            else if (current_symbol == "@animation2_id")
                current_enemy->animation2_id = this->read_integer();
            else if (current_symbol == "@exp")
                current_enemy->exp = this->read_integer();
            else if (current_symbol == "@gold")
                current_enemy->gold = this->read_integer();
            else if (current_symbol == "@treasure_prob")
                current_enemy->treasure_prob = this->read_integer();
            else if (current_symbol == "@armor_id")
                current_enemy->armor_id = this->read_integer();
            else if (current_symbol == "@weapon_id")
                current_enemy->weapon_id = this->read_integer();
            else if (current_symbol == "@item_id")
                current_enemy->item_id = this->read_integer();
            else if (current_symbol == "@element_ranks")
                this->read_table(&current_enemy->element_ranks);
            else if (current_symbol == "@state_ranks")
                this->read_table(&current_enemy->state_ranks);
            else if (current_symbol == "@actions")
            {
                int num_actions = this->read_array();

                for (int a = 0; a < num_actions; a++)
                {
                    QVariantList list = read_object();
                    if (list.at(0).toString() != "RPG::Enemy::Action")
                        throw getException("RPG::Enemy::Action expected");

                    RPGEnemyAction *current_enemy_action = new RPGEnemyAction();
                    for (int k = 0; k < list.at(1).toInt(); k++)
                    {
                        QString current_symbol = read_symbol_or_link();
                        current_enemy_action->param_order.append(current_symbol);

                        if (current_symbol == "@condition_level")
                            current_enemy_action->condition_level = this->read_integer();
                        else if (current_symbol == "@condition_switch_id")
                            current_enemy_action->condition_switch_id = this->read_integer();
                        else if (current_symbol == "@kind")
                            current_enemy_action->kind = this->read_integer();
                        else if (current_symbol == "@condition_turn_b")
                            current_enemy_action->condition_turn_b = this->read_integer();
                        else if (current_symbol == "@skill_id")
                            current_enemy_action->skill_id = this->read_integer();
                        else if (current_symbol == "@rating")
                            current_enemy_action->rating = this->read_integer();
                        else if (current_symbol == "@condition_turn_a")
                            current_enemy_action->condition_turn_a = this->read_integer();
                        else if (current_symbol == "@condition_hp")
                            current_enemy_action->condition_hp = this->read_integer();
                        else if (current_symbol == "@basic")
                            current_enemy_action->basic = this->read_integer();
                    }
                    current_enemy->action.append(current_enemy_action);
                }
            }
        }
        enemy_list->append(current_enemy);
    }
    this->file.close();
}

void IOEnemyFile::write_to_file(QString path, QList<RPGEnemy *> *enemy_list)
{
    this->last_visited_function = "write_to_file";
    this->symbol_cache.clear();
    this->object_count = 0;

    if (!path.isEmpty())
        this->path = path;
    this->file.setFileName(path);
    this->file.open(QIODevice::WriteOnly);
    this->write_header();

    this->write_array(enemy_list->length()+1); //zero at beginning
    this->write_one_byte((int)'0');

    for (int i = 0; i < enemy_list->length(); i++)
    {
        RPGEnemy *current_enemy = enemy_list->at(i);
        this->write_object("RPG::Enemy", 25);
        for (int j = 0; j < current_enemy->param_order.length(); j++)
        {
            QString current_symbol = current_enemy->param_order.at(j);
            this->write_symbol_or_link(current_symbol);

            if (current_symbol == "@name")
                this->write_string(current_enemy->name);
            else if (current_symbol == "@battler_name")
                this->write_string(current_enemy->battler_name);
            else if (current_symbol == "@battler_hue")
                this->write_integer(current_enemy->battler_hue);
            else if (current_symbol == "@id")
                this->write_integer(current_enemy->id);
            else if (current_symbol == "@maxhp")
                this->write_integer(current_enemy->maxhp);
            else if (current_symbol == "@maxsp")
                this->write_integer(current_enemy->maxsp);
            else if (current_symbol == "@str")
                this->write_integer(current_enemy->str);
            else if (current_symbol == "@dex")
                this->write_integer(current_enemy->dex);
            else if (current_symbol == "@agi")
                this->write_integer(current_enemy->agi);
            else if (current_symbol == "@int")
                this->write_integer(current_enemy->int_var); // int is not a variable name
            else if (current_symbol == "@atk")
                this->write_integer(current_enemy->atk);
            else if (current_symbol == "@pdef")
                this->write_integer(current_enemy->pdef);
            else if (current_symbol == "@mdef")
                this->write_integer(current_enemy->mdef);
            else if (current_symbol == "@eva")
                this->write_integer(current_enemy->eva);
            else if (current_symbol == "@animation1_id")
                this->write_integer(current_enemy->animation1_id);
            else if (current_symbol == "@animation2_id")
                this->write_integer(current_enemy->animation2_id);
            else if (current_symbol == "@exp")
                this->write_integer(current_enemy->exp);
            else if (current_symbol == "@gold")
                this->write_integer(current_enemy->gold);
            else if (current_symbol == "@treasure_prob")
                this->write_integer(current_enemy->treasure_prob);
            else if (current_symbol == "@armor_id")
                this->write_integer(current_enemy->armor_id);
            else if (current_symbol == "@weapon_id")
                this->write_integer(current_enemy->weapon_id);
            else if (current_symbol == "@item_id")
                this->write_integer(current_enemy->item_id);
            else if (current_symbol == "@element_ranks")
                this->write_table(&current_enemy->element_ranks);
            else if (current_symbol == "@state_ranks")
                this->write_table(&current_enemy->state_ranks);
            else if (current_symbol == "@actions")
            {

                this->write_array(current_enemy->action.length());

                for (int a = 0; a < current_enemy->action.length(); a++)
                {
                    this->write_object("RPG::Enemy::Action", 9);
                    RPGEnemyAction *current_enemy_action = current_enemy->action.at(a);

                    for (int k = 0; k < current_enemy_action->param_order.size(); k++)
                    {
                        QString current_symbol = current_enemy_action->param_order.at(k);
                        this->write_symbol_or_link(current_symbol);

                        if (current_symbol == "@condition_level")
                            this->write_integer(current_enemy_action->condition_level);
                        else if (current_symbol == "@condition_switch_id")
                            this->write_integer(current_enemy_action->condition_switch_id);
                        else if (current_symbol == "@kind")
                            this->write_integer(current_enemy_action->kind);
                        else if (current_symbol == "@condition_turn_b")
                            this->write_integer(current_enemy_action->condition_turn_b);
                        else if (current_symbol == "@skill_id")
                            this->write_integer(current_enemy_action->skill_id);
                        else if (current_symbol == "@rating")
                            this->write_integer(current_enemy_action->rating);
                        else if (current_symbol == "@condition_turn_a")
                            this->write_integer(current_enemy_action->condition_turn_a);
                        else if (current_symbol == "@condition_hp")
                            this->write_integer(current_enemy_action->condition_hp);
                        else if (current_symbol == "@basic")
                            this->write_integer(current_enemy_action->basic);
                    }
                }
            }
        }

    }

    this->file.close();
}
