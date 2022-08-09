#include "iosystemfile.h"

IOSystemFile::IOSystemFile(QObject *parent) : FileParser(parent)
{

}

IOSystemFile::IOSystemFile(QString path, RPGSettings *settings)
{
    this->last_visited_function = "IOSystemFile";
    this->symbol_cache.clear();

    this->path = path;
    this->file.setFileName(path);
    this->file.open(QIODevice::ReadOnly);

    check_header();
    QVariantList list = read_object();
    if (list.at(0).toString() != "RPG::System")
        throw getException("RPG::System expected");
    int num_params = list.at(1).toInt();

    for (int i = 0; i < num_params; i++)
    {
        QString current_symbol = this->read_symbol_or_link();

        if (current_symbol == "@variables")
        {
            int size = this->read_array();
            this->read_one_byte(); //withdraw zero, counting at 1
            for (int i = 1; i < size; i++)
                settings->variable_names.append(this->read_string());
        }
        else if (current_symbol == "@switches")
        {
            int size = this->read_array();
            this->read_one_byte(); //withdraw zero, counting at 1
            for (int i = 1; i < size; i++)
                settings->switches_names.append(this->read_string());
        }
        //soundeffects
        else if (current_symbol == "@cancel_se")
            this->read_audiofile_object(&settings->cancel_se);
        else if (current_symbol == "@escape_se")
            this->read_audiofile_object(&settings->escape_se);
        else if (current_symbol == "@battle_end_me")
            this->read_audiofile_object(&settings->battle_end_me);
        else if (current_symbol == "@shop_se")
            this->read_audiofile_object(&settings->shop_se);
        else if (current_symbol == "@decision_se")
            this->read_audiofile_object(&settings->decision_se);
        else if (current_symbol == "@battle_start_se")
            this->read_audiofile_object(&settings->battle_start_se);
        else if (current_symbol == "@battle_bgm")
            this->read_audiofile_object(&settings->battle_bgm);
        else if (current_symbol == "@equip_se")
            this->read_audiofile_object(&settings->equip_se);
        else if (current_symbol == "@enemy_collapse_se")
            this->read_audiofile_object(&settings->enemy_collapse_se);
        else if (current_symbol == "@cursor_se")
            this->read_audiofile_object(&settings->cursor_se);
        else if (current_symbol == "@load_se")
            this->read_audiofile_object(&settings->load_se);
        else if (current_symbol == "@title_bgm")
            this->read_audiofile_object(&settings->title_bgm);
        else if (current_symbol == "@buzzer_se")
            this->read_audiofile_object(&settings->buzzer_se);
        else if (current_symbol == "@actor_collapse_se")
            this->read_audiofile_object(&settings->actor_collapse_se);
        else if (current_symbol == "@gameover_me")
            this->read_audiofile_object(&settings->gameover_me);
        else if (current_symbol == "@save_se")
            this->read_audiofile_object(&settings->save_se);
        //integers
        else if (current_symbol == "@magic_number")
            settings->magic_number = this->read_integer();
        else if (current_symbol == "@start_map_id")
            settings->start_map_id = this->read_integer();
        else if (current_symbol == "@edit_map_id")
            settings->edit_map_id = this->read_integer();
        else if (current_symbol == "@test_troop_id")
            settings->test_troop_id = this->read_integer();
        else if (current_symbol == "@start_x")
            settings->start_x = this->read_integer();
        else if (current_symbol == "@start_y")
            settings->start_y = this->read_integer();
        else if (current_symbol == "@battler_hue")
            settings->battler_hue = this->read_integer();
        else if (current_symbol == "@_")
            settings->_ = this->read_integer();
        //strings
        else if (current_symbol == "@gameover_name")
            settings->gameover_name = this->read_string();
        else if (current_symbol == "@title_name")
            settings->title_name = this->read_string();
        else if (current_symbol == "@windowskin_name")
            settings->windowskin_name = this->read_string();
        else if (current_symbol == "@battleback_name")
            settings->battleback_name = this->read_string();
        else if (current_symbol == "@battler_name")
            settings->battler_name = this->read_string();
        else if (current_symbol == "@battle_transition")
            settings->battle_transition = this->read_string();
        //other
        else if (current_symbol == "@words")
        {
            QVariantList params = this->read_object();
            if (params.at(0).toString() != "RPG::System::Words")
                throw getException("RPG::System::Words expected");
            int num = params.at(1).toInt();
            for (int i = 0; i < num; i++)
            {
                QList<RPGString> list;
                list.append(RPGString(this->read_symbol_or_link()));
                list.append(this->read_string());
                settings->words.append(list);
            }
        }
        else if (current_symbol == "@test_battlers")
        {
            int num = this->read_array();
            for (int i = 0; i < num; i++)
            {
                QList< QVariantList > current_test_battler;

                QVariantList params = this->read_object();
                if (params.at(0).toString() != "RPG::System::TestBattler")
                    throw getException("RPG::System::TestBattler expected");
                int num_params = params.at(1).toInt(); // should be 7
                for (int k = 0; k < num_params; k++)
                {
                    QVariantList list;
                    list.append(QVariant(this->read_symbol_or_link()));
                    list.append(QVariant(this->read_integer()));
                    current_test_battler.append(list);
                }
                settings->test_battlers.append(current_test_battler);
            }
        }
        else if (current_symbol == "@elements")
        {
            int num = this->read_array();
            for (int i = 0; i < num; i++)
                settings->elements.append(this->read_string());
        }
        else if (current_symbol == "@party_members")
        {
            int num = this->read_array();
            for (int i = 0; i < num; i++)
                settings->party_members.append(this->read_integer());
        }
    }


    this->file.close();



}

void IOSystemFile::write_to_file(QString path, RPGSettings *settings)
{
    this->last_visited_function = "write_to_file";
    this->symbol_cache.clear();

    if (!path.isEmpty())
        this->path = path;
    this->file.setFileName(path);
    this->file.open(QIODevice::WriteOnly);
    this->write_header();

    this->write_object("RPG::System", 36);

    this->write_symbol_or_link("@cancel_se");
    this->write_audiofile_object(&settings->cancel_se);

    this->write_symbol_or_link("@variables");
    this->write_array(settings->variable_names.length()+1);
    this->write_one_byte((int)'0'); //start counting with 1
    for (int i = 0; i < settings->variable_names.length(); i++)
        this->write_string(settings->variable_names.at(i));


    this->write_symbol_or_link("@battle_end_me");
    this->write_audiofile_object(&settings->battle_end_me);

    this->write_symbol_or_link("@escape_se");
    this->write_audiofile_object(&settings->escape_se);

    this->write_symbol_or_link("@magic_number");
    this->write_integer(settings->magic_number);

    this->write_symbol_or_link("@gameover_name");
    this->write_string(settings->gameover_name);

    this->write_symbol_or_link("@shop_se");
    this->write_audiofile_object(&settings->shop_se);

    this->write_symbol_or_link("@start_map_id");
    this->write_integer(settings->start_map_id);

    this->write_symbol_or_link("@decision_se");
    this->write_audiofile_object(&settings->decision_se);


    this->write_symbol_or_link("@switches");
    this->write_array(settings->switches_names.length()+1);
    this->write_one_byte((int)'0'); //start counting with 1
    for (int i = 0; i < settings->switches_names.length(); i++)
        this->write_string(settings->switches_names.at(i));

    this->write_symbol_or_link("@words");
    this->write_object("RPG::System::Words", settings->words.length());
    for (int i = 0; i < settings->words.length(); i++)
    {
        this->write_symbol_or_link(settings->words.at(i).at(0));
        this->write_string(settings->words.at(i).at(1));
    }

    this->write_symbol_or_link("@battle_bgm");
    this->write_audiofile_object(&settings->battle_bgm);

    this->write_symbol_or_link("@battle_start_se");
    this->write_audiofile_object(&settings->battle_start_se);

    this->write_symbol_or_link("@edit_map_id");
    this->write_integer(settings->edit_map_id);

    this->write_symbol_or_link("@_");
    this->write_integer(settings->_);

    this->write_symbol_or_link("@title_name");
    this->write_string(settings->title_name);

    this->write_symbol_or_link("@equip_se");
    this->write_audiofile_object(&settings->equip_se);

    this->write_symbol_or_link("@test_troop_id");
    this->write_integer(settings->test_troop_id);

    this->write_symbol_or_link("@elements");
    this->write_array(settings->elements.length());
    for (int i = 0; i < settings->elements.length(); i++)
        this->write_string(settings->elements.at(i));


    this->write_symbol_or_link("@cursor_se");
    this->write_audiofile_object(&settings->cursor_se);

    this->write_symbol_or_link("@enemy_collapse_se");
    this->write_audiofile_object(&settings->enemy_collapse_se);

    this->write_symbol_or_link("@title_bgm");
    this->write_audiofile_object(&settings->title_bgm);

    this->write_symbol_or_link("@load_se");
    this->write_audiofile_object(&settings->load_se);

    this->write_symbol_or_link("@battler_hue");
    this->write_integer(settings->battler_hue);

    this->write_symbol_or_link("@start_y");
    this->write_integer(settings->start_y);

    this->write_symbol_or_link("@test_battlers");
    this->write_array(settings->test_battlers.length());


    for (int i = 0; i < settings->test_battlers.length(); i++)
    {
        this->write_object("RPG::System::TestBattler", settings->test_battlers.at(i).length());
        for (int j = 0; j < settings->test_battlers.at(i).length(); j++)
        {
            this->write_symbol_or_link(settings->test_battlers.at(i).at(j).at(0).toString());
            this->write_integer(settings->test_battlers.at(i).at(j).at(1).toInt());
        }

    }

    this->write_symbol_or_link("@windowskin_name");
    this->write_string(settings->windowskin_name);

    this->write_symbol_or_link("@buzzer_se");
    this->write_audiofile_object(&settings->buzzer_se);

    this->write_symbol_or_link("@gameover_me");
    this->write_audiofile_object(&settings->gameover_me);

    this->write_symbol_or_link("@actor_collapse_se");
    this->write_audiofile_object(&settings->actor_collapse_se);

    this->write_symbol_or_link("@party_members");
    this->write_array(settings->party_members.length());
    for (int i = 0; i < settings->party_members.length(); i++)
        this->write_integer(settings->party_members.at(i));


    this->write_symbol_or_link("@battleback_name");
    this->write_string(settings->battleback_name);

    this->write_symbol_or_link("@start_x");
    this->write_integer(settings->start_x);

    this->write_symbol_or_link("@battle_transition");
    this->write_string(settings->battle_transition);

    this->write_symbol_or_link("@save_se");
    this->write_audiofile_object(&settings->save_se);

    this->write_symbol_or_link("@battler_name");
    this->write_string(settings->battler_name);

    this->file.close();
}
