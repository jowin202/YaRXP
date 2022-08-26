#include "ioclassfile.h"

IOClassFile::IOClassFile()
{

}

IOClassFile::IOClassFile(QString path, QList<RPGClass *> *class_list)
{
    this->last_visited_function = "IOClassFile";
    this->symbol_cache.clear();

    this->path = path;
    this->file.setFileName(path);
    this->file.open(QIODevice::ReadOnly);

    check_header();
    int length = read_array();
    this->read_one_byte(); //withdraw first zero

    for (int i = 1 ; i < length; i++)
    {
        RPGClass *current_class = new RPGClass;

        QVariantList list = read_object();
        if (list.at(0).toString() != "RPG::Class")
            throw getException("RPG::Class expected");

        for (int j = 0; j < list.at(1).toInt(); j++)
        {
            QString current_symbol = read_symbol_or_link();
            current_class->param_order << current_symbol;

            if (current_symbol == "@id")
                current_class->id = this->read_integer();
            else if (current_symbol == "@position")
                current_class->position = this->read_integer();
            else if (current_symbol == "@name")
                current_class->name = this->read_string();
            else if (current_symbol == "@element_ranks")
                this->read_table(&current_class->element_ranks);
            else if (current_symbol == "@state_ranks")
                this->read_table(&current_class->state_ranks);
            else if (current_symbol == "@weapon_set")
            {
                int wset_size = this->read_array();
                for (int w = 0; w < wset_size; w++)
                    current_class->weapon_set.append(this->read_integer());
            }
            else if (current_symbol == "@armor_set")
            {
                int aset_size = this->read_array();
                for (int a = 0; a < aset_size; a++)
                    current_class->armor_set.append(this->read_integer());
            }
            else if (current_symbol == "@learnings")
            {
                int num_learnings = this->read_array();

                for (int l = 0; l < num_learnings; l++)
                {
                    QVariantList param_list = this->read_object();
                    if (param_list.at(0).toString() != "RPG::Class::Learning")
                        throw getException("RPG::Class::Learning expected");

                    for (int n = 0; n < param_list.at(1).toInt(); n++)
                    {
                        QString current_symbol = this->read_symbol_or_link();

                        if (current_symbol == "@skill_id")
                        {
                            if (n==0) current_class->learning_order = true; //skill_id first
                            current_class->learnings_skill_id.append(this->read_integer());
                        }
                        else if (current_symbol == "@level")
                        {
                            if (n==0) current_class->learning_order = false; //level first
                            current_class->learnings_level.append(this->read_integer());
                        }

                    }
                }


            }

        }
        class_list->append(current_class);
    }

    this->file.close();
}

void IOClassFile::write_to_file(QString path, QList<RPGClass *> *class_list)
{
    this->last_visited_function = "write_to_file";
    this->symbol_cache.clear();

    if (!path.isEmpty())
        this->path = path;
    this->file.setFileName(path);
    this->file.open(QIODevice::WriteOnly);
    this->write_header();

    this->write_array(class_list->length()+1); //zero at beginning
    this->write_one_byte((int)'0');

    for (int i = 0; i < class_list->length(); i++)
    {
        RPGClass *current_class = class_list->at(i);
        this->write_object("RPG::Class", 8);
        for (int j = 0; j < current_class->param_order.length(); j++)
        {
            QString current_symbol = current_class->param_order.at(j);
            this->write_symbol_or_link(current_symbol);

            if (current_symbol == "@id")
                this->write_integer(current_class->id);
            else if (current_symbol == "@position")
                this->write_integer(current_class->position);
            else if (current_symbol == "@name")
                this->write_string(current_class->name);
            else if (current_symbol == "@element_ranks")
                this->write_table(&current_class->element_ranks);
            else if (current_symbol == "@state_ranks")
                this->write_table(&current_class->state_ranks);
            else if (current_symbol == "@weapon_set")
            {
                this->write_array(current_class->weapon_set.size());
                for (int w = 0; w < current_class->weapon_set.size(); w++)
                    this->write_integer(current_class->weapon_set.at(w));
            }
            else if (current_symbol == "@armor_set")
            {
                this->write_array(current_class->armor_set.size());
                for (int a = 0; a < current_class->armor_set.size(); a++)
                    this->write_integer(current_class->armor_set.at(a));
            }
            else if (current_symbol == "@learnings")
            {
                this->write_array(current_class->learnings_level.length());
                for (int l = 0; l < current_class->learnings_level.length(); l++)
                {
                    this->write_object("RPG::Class::Learning", 2);
                    if (current_class->learning_order) //skill_id first
                    {
                        this->write_symbol_or_link("@skill_id");
                        this->write_integer(current_class->learnings_skill_id.at(l));
                        this->write_symbol_or_link("@level");
                        this->write_integer(current_class->learnings_level.at(l));
                    }
                    else // level first
                    {
                        this->write_symbol_or_link("@level");
                        this->write_integer(current_class->learnings_level.at(l));
                        this->write_symbol_or_link("@skill_id");
                        this->write_integer(current_class->learnings_skill_id.at(l));
                    }
                }
            }
        }
    }
    this->file.close();
}
