#include "iotroopfile.h"

IOTroopFile::IOTroopFile()
{

}

IOTroopFile::IOTroopFile(QString path, QList<RPGTroop *> *troop_list)
{
    this->last_visited_function = "IOTroopFile";
    this->symbol_cache.clear();

    this->path = path;
    this->file.setFileName(path);
    this->file.open(QIODevice::ReadOnly);

    check_header();
    int length = read_array();
    this->read_one_byte(); //withdraw first zero

    for (int i = 1 ; i < length; i++)
    {
        RPGTroop *current_troop = new RPGTroop;

        QVariantList list = read_object();
        if (list.at(0).toString() != "RPG::Troop")
            throw getException("RPG::Troop expected");

        for (int j = 0; j < list.at(1).toInt(); j++)
        {
            QString current_symbol = read_symbol_or_link();
            current_troop->param_order << current_symbol;

            if (current_symbol == "@id")
                current_troop->id = this->read_integer();
            else if (current_symbol == "@name")
                current_troop->name = this->read_string();
            else if (current_symbol == "@members")
            {
                int num_members = this->read_array();
                for (int k = 0; k < num_members; k++)
                {
                    QVariantList list = read_object();
                    if (list.at(0).toString() != "RPG::Troop::Member")
                        throw getException("RPG::Troop::Member expected");
                    RPGTroopMember *current_troop_member = new RPGTroopMember;
                    for (int l = 0; l < list.at(1).toInt(); l++)
                    {
                        QString current_symbol = this->read_symbol_or_link();
                        current_troop_member->param_order.append(current_symbol);

                        if (current_symbol == "@x")
                            current_troop_member->x = this->read_integer();
                        else if (current_symbol == "@y")
                            current_troop_member->y = this->read_integer();
                        else if (current_symbol == "@enemy_id")
                            current_troop_member->enemy_id = this->read_integer();
                        else if (current_symbol == "@immortal")
                            current_troop_member->immortal = this->read_bool();
                        else if (current_symbol == "@hidden")
                            current_troop_member->hidden = this->read_bool();
                    }

                    current_troop->members.append(current_troop_member);
                }
            }
            else if (current_symbol == "@pages")
            {
                int num_pages = this->read_array();
                for (int k = 0; k < num_pages; k++)
                {
                    QVariantList list = read_object();
                    if (list.at(0).toString() != "RPG::Troop::Page")
                        throw getException("RPG::Troop::Page expected");
                    RPGTroopPage *current_troop_page = new RPGTroopPage;

                    for (int l = 0; l < list.at(1).toInt(); l++)
                    {
                        QString current_symbol = this->read_symbol_or_link();
                        current_troop_page->param_order.append(current_symbol);


                        if (current_symbol == "@span")
                            current_troop_page->span = this->read_integer();
                        else if (current_symbol == "@list")
                            this->read_event_list(&current_troop_page->list);
                        else if (current_symbol == "@condition")
                        {
                            QVariantList list = read_object();
                            if (list.at(0).toString() != "RPG::Troop::Page::Condition")
                                throw getException("RPG::Troop::Page::Condition expected");

                            for (int m = 0; m < list.at(1).toInt(); m++)
                            {
                                QString current_symbol = this->read_symbol_or_link();
                                current_troop_page->condition_param_order.append(current_symbol);

                                if (current_symbol == "@switch_id")
                                    current_troop_page->switch_id = this->read_integer();
                                else if (current_symbol == "@turn_a")
                                    current_troop_page->turn_a = this->read_integer();
                                else if (current_symbol == "@turn_b")
                                    current_troop_page->turn_b = this->read_integer();
                                else if (current_symbol == "@enemy_index")
                                    current_troop_page->enemy_index = this->read_integer();
                                else if (current_symbol == "@enemy_hp")
                                    current_troop_page->enemy_hp = this->read_integer();
                                else if (current_symbol == "@actor_id")
                                    current_troop_page->actor_id = this->read_integer();
                                else if (current_symbol == "@actor_hp")
                                    current_troop_page->actor_hp = this->read_integer();
                                else if (current_symbol == "@enemy_valid")
                                    current_troop_page->enemy_valid = this->read_bool();
                                else if (current_symbol == "@switch_valid")
                                    current_troop_page->switch_valid = this->read_bool();
                                else if (current_symbol == "@turn_valid")
                                    current_troop_page->turn_valid = this->read_bool();
                                else if (current_symbol == "@actor_valid")
                                    current_troop_page->actor_valid = this->read_bool();
                            }
                        }
                    }
                    current_troop->pages.append(current_troop_page);
                }
            }
        }
        troop_list->append(current_troop);
    }
    this->file.close();
}

void IOTroopFile::write_to_file(QString path, QList<RPGTroop *> *troop_list)
{
    this->last_visited_function = "write_to_file";
    this->symbol_cache.clear();
    this->object_count = 0;

    if (!path.isEmpty())
        this->path = path;
    this->file.setFileName(path);
    this->file.open(QIODevice::WriteOnly);
    this->write_header();

    this->write_array(troop_list->length()+1); //zero at beginning
    this->write_one_byte((int)'0');

    for (int i = 0; i < troop_list->length(); i++)
    {
        RPGTroop *current_troop = troop_list->at(i);
        this->write_object("RPG::Troop", 4);
        for (int j = 0; j < current_troop->param_order.length(); j++)
        {
            QString current_symbol = current_troop->param_order.at(j);
            this->write_symbol_or_link(current_symbol);

            if (current_symbol == "@id")
                this->write_integer(current_troop->id);
            else if (current_symbol == "@name")
                this->write_string(current_troop->name);
            else if (current_symbol == "@members")
            {
                this->write_array(current_troop->members.length());
                for (int k = 0; k < current_troop->members.length(); k++)
                {
                    this->write_object("RPG::Troop::Member", 5);
                    RPGTroopMember *current_troop_member = current_troop->members.at(k);
                    for (int l = 0; l < current_troop_member->param_order.length(); l++)
                    {
                        QString current_symbol = current_troop_member->param_order.at(l);
                        this->write_symbol_or_link(current_symbol);

                        if (current_symbol == "@x")
                            this->write_integer(current_troop_member->x);
                        else if (current_symbol == "@y")
                            this->write_integer(current_troop_member->y);
                        else if (current_symbol == "@enemy_id")
                            this->write_integer(current_troop_member->enemy_id);
                        else if (current_symbol == "@immortal")
                            this->write_bool(current_troop_member->immortal);
                        else if (current_symbol == "@hidden")
                            this->write_bool(current_troop_member->hidden);
                    }

                }
            }
            else if (current_symbol == "@pages")
            {
                this->write_array(current_troop->pages.length());
                for (int k = 0; k < current_troop->pages.length(); k++)
                {
                    this->write_object("RPG::Troop::Page", 3);
                    RPGTroopPage *current_troop_page = current_troop->pages.at(k);
                    for (int l = 0; l < current_troop_page->param_order.length(); l++)
                    {
                        QString current_symbol = current_troop_page->param_order.at(l);
                        this->write_symbol_or_link(current_symbol);

                        if (current_symbol == "@span")
                            this->write_integer(current_troop_page->span);
                        else if (current_symbol == "@list")
                            this->write_event_list(&current_troop_page->list);
                        else if (current_symbol == "@condition")
                        {
                            this->write_object("RPG::Troop::Page::Condition", 11);

                            for (int m = 0; m < current_troop_page->condition_param_order.length(); m++)
                            {
                                QString current_symbol = current_troop_page->condition_param_order.at(m);
                                this->write_symbol_or_link(current_symbol);

                                if (current_symbol == "@switch_id")
                                    this->write_integer(current_troop_page->switch_id);
                                else if (current_symbol == "@turn_a")
                                    this->write_integer(current_troop_page->turn_a);
                                else if (current_symbol == "@turn_b")
                                    this->write_integer(current_troop_page->turn_b);
                                else if (current_symbol == "@enemy_index")
                                    this->write_integer(current_troop_page->enemy_index);
                                else if (current_symbol == "@enemy_hp")
                                    this->write_integer(current_troop_page->enemy_hp);
                                else if (current_symbol == "@actor_id")
                                    this->write_integer(current_troop_page->actor_id);
                                else if (current_symbol == "@actor_hp")
                                    this->write_integer(current_troop_page->actor_hp);
                                else if (current_symbol == "@enemy_valid")
                                    this->write_bool(current_troop_page->enemy_valid);
                                else if (current_symbol == "@switch_valid")
                                    this->write_bool(current_troop_page->switch_valid);
                                else if (current_symbol == "@turn_valid")
                                    this->write_bool(current_troop_page->turn_valid);
                                else if (current_symbol == "@actor_valid")
                                    this->write_bool(current_troop_page->actor_valid);
                            }
                        }
                    }
                }
            }
        }
    }
    this->file.close();
}
