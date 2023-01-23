#include "factory.h"

Factory::Factory()
{

}

QJsonObject Factory::create_learnings()
{
    return create_learnings(1,1);
}

QJsonObject Factory::create_learnings(int level, int skill_id)
{
    QJsonObject obj;
    obj.insert("@level", level);
    obj.insert("@skill_id", skill_id);
    obj.insert("RXClass", "RPG::Class::Learning");
    return obj;
}

QJsonObject Factory::create_audiofile()
{
    return this->create_audiofile("", 100,100);
}

QJsonObject Factory::create_audiofile(QString name, int volume, int pitch)
{
    QJsonObject obj;
    obj.insert("@name", name);
    obj.insert("@volume", volume);
    obj.insert("@pitch", pitch);
    obj.insert("RXClass", "RPG::AudioFile");
    return obj;
}

QJsonObject Factory::create_enemy_action()
{
    return this->create_enemy_action(0,1,100,1,0,0,1,5,0);
}

QJsonObject Factory::create_enemy_action(int turn_a, int turn_b, int hp, int level, int switch_id, int kind, int skill, int rating, int basic)
{
    QJsonObject obj;
    obj.insert("@condition_turn_a", turn_a);
    obj.insert("@condition_turn_b", turn_b);
    obj.insert("@condition_hp", hp);
    obj.insert("@condition_level", level);
    obj.insert("@condition_switch_id", switch_id);
    obj.insert("@kind", kind);
    obj.insert("@skill_id", skill);
    obj.insert("@rating", rating);
    obj.insert("@basic", basic);
    obj.insert("RXClass", "RPG::Enemy::Action");
    return obj;
}

QJsonObject Factory::create_troop_member()
{
    return create_troop_member(1,false, false, 0,0);
}

QJsonObject Factory::create_troop_member(int enemy_id, bool hidden, bool immortal, int x, int y)
{
    QJsonObject obj;
    obj.insert("@enemy_id", enemy_id);
    obj.insert("@hidden", hidden);
    obj.insert("@immortal", immortal);
    obj.insert("@x", x);
    obj.insert("@y", y);
    obj.insert("RXClass", "RPG::Troop::Member");
    return obj;
}

QJsonObject Factory::create_empty_event_list()
{
    QJsonObject obj;
    obj.insert("@code", 0);
    obj.insert("@indent", 0);
    obj.insert("@parameters", QJsonArray());
    obj.insert("RXClass", "RPG::EventCommand");
    return obj;
}

QJsonObject Factory::create_troop_page_condition(int actor_hp, int actor_id, bool actor_valid, int enemy_hp, int enemy_index, bool enemy_valid, int switch_id, bool switch_valid, int turn_a, int turn_b, bool turn_valid)
{
    QJsonObject obj;
    obj.insert("@actor_hp", actor_hp);
    obj.insert("@actor_id", actor_id);
    obj.insert("@actor_valid", actor_valid);
    obj.insert("@enemy_hp", enemy_hp);
    obj.insert("@enemy_index", enemy_index);
    obj.insert("@enemy_valid", enemy_valid);
    obj.insert("@switch_id", switch_id);
    obj.insert("@switch_valid", switch_valid);
    obj.insert("@turn_a", turn_a);
    obj.insert("@turn_b", turn_b);
    obj.insert("@turn_valid", turn_valid);
    obj.insert("RXClass", "RPG::Troop::Page::Condition");
    return obj;
}

QJsonObject Factory::create_troop_page_condition()
{
    return create_troop_page_condition(50,1,false,50,0,false,1,false, 0,0,false);
}

QJsonObject Factory::create_troop_page()
{
    QJsonObject obj;
    obj.insert("@condition", this->create_troop_page_condition());
    QJsonArray commands;
    commands.append(this->create_empty_event_list());
    obj.insert("@list", commands);
    obj.insert("@span",0);

    obj.insert("RXClass", "RPG::Troop::Page");

    return obj;
}

QJsonObject Factory::create_animation_timing()
{
    return this->create_animation_timing(1, this->create_audiofile(),0,0,0,0,0,0,0);
}

QJsonObject Factory::create_animation_timing(int frame, QJsonObject audiofile, int condition, int flash_scope, int r, int g, int b, int strength, int duration)
{
    QJsonObject obj;
    obj.insert("@condition", condition);

    QJsonObject flash_color;
    flash_color.insert("RXClass", "Color");
    flash_color.insert("alpha_gray", strength);
    flash_color.insert("r", r);
    flash_color.insert("g", g);
    flash_color.insert("b", b);
    obj.insert("@flash_color", flash_color);

    obj.insert("@flash_duration", duration);
    obj.insert("@flash_scope", flash_scope);
    obj.insert("@frame", frame);
    obj.insert("@se", audiofile);
    obj.insert("RXClass", "RPG::Animation::Timing");
    return obj;
}


QJsonObject Factory::create_new_actor(int id)
{
    QJsonObject obj;
    obj.insert("@armor1_fix", false);
    obj.insert("@armor1_id", 0);
    obj.insert("@armor2_fix", false);
    obj.insert("@armor2_id", 0);
    obj.insert("@armor3_fix", false);
    obj.insert("@armor3_id", 0);
    obj.insert("@armor4_fix", false);
    obj.insert("@armor4_id", 0);
    obj.insert("@battler_hue", 0);
    obj.insert("@battler_name", "");
    obj.insert("@character_hue", 0);
    obj.insert("@character_name", "");
    obj.insert("@class_id", 1);
    obj.insert("@exp_basis", 30);
    obj.insert("@exp_inflation", 30);
    obj.insert("@final_level", 99);
    obj.insert("@id", id);
    obj.insert("@initial_level", 1);
    obj.insert("@name", "");
    obj.insert("@weapon_fix", false);
    obj.insert("@weapon_id", 0);
    obj.insert("RXClass", "RPG::Actor");

    //parameters object
    QJsonObject params;
    params.insert("RXClass", "Table");
    params.insert("shitty_num", 2);
    QJsonArray values;
    for (int i = 0; i < 6; i++) values.append(0);
    for (int i = 1; i < 100; i++)
    {
        values.append(10*(50+i*5));
        values.append(10*(50+i*5));
        values.append(50+i*5);
        values.append(50+i*5);
        values.append(50+i*5);
        values.append(50+i*5);
    }
    params.insert("values", values);
    params.insert("x", 6);
    params.insert("y", 100);
    params.insert("z", 1);

    obj.insert("@parameters", params);

    return obj;
}

QJsonObject Factory::create_new_class(int id, int element_num, int state_num)
{
    QJsonObject obj;

    obj.insert("@armor_set", QJsonArray());
    obj.insert("@id", id);
    obj.insert("@learnings", QJsonArray());
    obj.insert("@name", "");
    obj.insert("@position", 0);
    obj.insert("@weapon_set", QJsonArray());
    obj.insert("RXClass", "RPG::Class");

    QJsonObject element_ranks;
    element_ranks.insert("RXClass", "Table");
    element_ranks.insert("shitty_num", 1);
    QJsonArray element_values;
    element_values.append(0);
    for (int i = 1; i < element_num; i++)
        element_values.append(3);
    element_ranks.insert("values", element_values);
    element_ranks.insert("x", element_num);
    element_ranks.insert("y", 1);
    element_ranks.insert("z", 1);
    obj.insert("@element_ranks", element_ranks);

    QJsonObject state_ranks;
    state_ranks.insert("RXClass", "Table");
    state_ranks.insert("shitty_num", 1);
    QJsonArray state_values;
    state_values.append(0);
    for (int i = 1; i < state_num; i++)
        state_values.append(3);
    state_ranks.insert("values", state_values);
    state_ranks.insert("x", state_num);
    state_ranks.insert("y", 1);
    state_ranks.insert("z", 1);
    obj.insert("@state_ranks", state_ranks);

    return obj;
}

QJsonObject Factory::create_new_skill(int id)
{
    QJsonObject obj;
    obj.insert("@agi_f",0);
    obj.insert("@animation1_id",0);
    obj.insert("@animation2_id",0);
    obj.insert("@atk_f",0);
    obj.insert("@common_event_id",0);
    obj.insert("@description", "");
    obj.insert("@dex_f",0);
    obj.insert("@element_set",QJsonArray());
    obj.insert("@eva_f",0);
    obj.insert("@hit",100);
    obj.insert("@icon_name", "");
    obj.insert("@id",id);
    obj.insert("@int_f",100);
    obj.insert("@mdef_f",100);
    obj.insert("@menu_se", create_audiofile("",80,100));
    obj.insert("@minus_state_set",QJsonArray());
    obj.insert("@name", "");
    obj.insert("@occasion",1);
    obj.insert("@pdef_f",0);
    obj.insert("@plus_state_set",QJsonArray());
    obj.insert("@power",0);
    obj.insert("@scope",0);
    obj.insert("@sp_cost",0);
    obj.insert("@str_f",0);
    obj.insert("@variance",15);
    obj.insert("RXClass","RPG::Skill");

    return obj;
}

QJsonObject Factory::create_new_item(int id)
{
    QJsonObject obj;
    obj.insert("@animation1_id",0);
    obj.insert("@animation2_id",0);
    obj.insert("@common_event_id",0);
    obj.insert("@consumable",true);
    obj.insert("@description", "");
    obj.insert("@element_set",QJsonArray());
    obj.insert("@hit",100);
    obj.insert("@icon_name", "");
    obj.insert("@id",id);
    obj.insert("@mdef_f",0);
    obj.insert("@menu_se", create_audiofile("",80,100));
    obj.insert("@minus_state_set",QJsonArray());
    obj.insert("@name", "");
    obj.insert("@occasion",0);
    obj.insert("@parameter_points",0);
    obj.insert("@parameter_type",0);
    obj.insert("@pdef_f",0);
    obj.insert("@plus_state_set",QJsonArray());
    obj.insert("@price",0);
    obj.insert("@recover_hp",0);
    obj.insert("@recover_hp_rate",0);
    obj.insert("@recover_sp",0);
    obj.insert("@recover_sp_rate",0);
    obj.insert("@scope",0);
    obj.insert("@variance",0);

    obj.insert("RXClass","RPG::Item");

    return obj;

}

QJsonObject Factory::create_new_weapon(int id)
{
    QJsonObject obj;
    obj.insert("@agi_plus",0);
    obj.insert("@animation1_id",0);
    obj.insert("@animation2_id",0);
    obj.insert("@atk",0);
    obj.insert("@description", "");
    obj.insert("@dex_plus",0);
    obj.insert("@element_set",QJsonArray());
    obj.insert("@icon_name", "");
    obj.insert("@id",id);
    obj.insert("@int_plus",0);
    obj.insert("@mdef",0);
    obj.insert("@minus_state_set",QJsonArray());
    obj.insert("@name", "");
    obj.insert("@pdef",0);
    obj.insert("@plus_state_set",QJsonArray());
    obj.insert("@price",0);
    obj.insert("@str_plus",0);


    obj.insert("RXClass","RPG::Weapon");

    return obj;
}


QJsonObject Factory::create_new_armor(int id)
{
    QJsonObject obj;
    obj.insert("@agi_plus",0);
    obj.insert("@auto_state_id",0);
    obj.insert("@description", "");
    obj.insert("@dex_plus",0);
    obj.insert("@eva",0);
    obj.insert("@guard_element_set",QJsonArray());
    obj.insert("@guard_state_set",QJsonArray());
    obj.insert("@icon_name", "");
    obj.insert("@id",id);
    obj.insert("@int_plus",0);
    obj.insert("@kind",0);
    obj.insert("@mdef",0);
    obj.insert("@name", "");
    obj.insert("@pdef",0);
    obj.insert("@price",0);
    obj.insert("@str_plus",0);


    obj.insert("RXClass","RPG::Armor");

    return obj;
}

QJsonObject Factory::create_new_enemy(int id, int element_num, int state_num)
{
    QJsonObject obj;
    QJsonArray action_array;
    action_array.append(create_enemy_action(0,1,100,1,0,0,1,5,0));
    obj.insert("@actions", action_array);
    obj.insert("@agi",50);
    obj.insert("@animation1_id",0);
    obj.insert("@animation2_id",0);
    obj.insert("@armor_id",0);
    obj.insert("@atk",100);
    obj.insert("@battler_hue",0);
    obj.insert("@battler_name", "");
    obj.insert("@dex",50);

    QJsonObject element_ranks_obj;
    element_ranks_obj.insert("RXClass","Table");
    element_ranks_obj.insert("shitty_num",1);
    QJsonArray element_values;
    element_values.append(0);
    for (int i = 1; i < element_num; i++)
        element_values.append(3);
    element_ranks_obj.insert("values",element_values);

    element_ranks_obj.insert("x", element_num);
    element_ranks_obj.insert("y", 1);
    element_ranks_obj.insert("z", 1);

    obj.insert("@element_ranks",element_ranks_obj);


    obj.insert("@eva",0);
    obj.insert("@exp",0);
    obj.insert("@gold",0);
    obj.insert("@id", id);
    obj.insert("@int",50);
    obj.insert("@item_id",0);
    obj.insert("@maxhp",500);
    obj.insert("@maxsp",500);
    obj.insert("@mdef",100);
    obj.insert("@name","");
    obj.insert("@pdef",100);

    QJsonObject state_ranks_obj;
    state_ranks_obj.insert("RXClass","Table");
    state_ranks_obj.insert("shitty_num",1);
    QJsonArray state_values;
    state_values.append(0);
    for (int i = 1; i < state_num; i++)
        state_values.append(3);
    state_ranks_obj.insert("values",state_values);

    state_ranks_obj.insert("x", state_num);
    state_ranks_obj.insert("y", 1);
    state_ranks_obj.insert("z", 1);

    obj.insert("@state_ranks",state_ranks_obj);

    obj.insert("@str",50);
    obj.insert("@treasure_prob",100);
    obj.insert("@weapon_id",0);

    obj.insert("RXClass","RPG::Enemy");

    return obj;
}

QJsonObject Factory::create_new_troop(int id)
{
    QJsonObject obj;
    obj.insert("@id", id);
    obj.insert("@members",QJsonArray());
    obj.insert("@name", "");

    QJsonArray page_array;
    page_array.append(create_troop_page());
    obj.insert("@pages", page_array);

    obj.insert("RXClass","RPG::Troop");

    return obj;
}

QJsonObject Factory::create_new_state(int id)
{
    QJsonObject obj;
    obj.insert("@agi_rate",100);
    obj.insert("@animation_id",0);
    obj.insert("@atk_rate",100);
    obj.insert("@auto_release_prob",0);
    obj.insert("@battle_only",true);
    obj.insert("@cant_evade",false);
    obj.insert("@cant_get_exp",false);
    obj.insert("@dex_rate",100);
    obj.insert("@eva",0);
    obj.insert("@guard_element_set", QJsonArray());
    obj.insert("@hit_rate",100);
    obj.insert("@hold_turn",0);
    obj.insert("@id", id);
    obj.insert("@int_rate",100);
    obj.insert("@maxhp_rate",100);
    obj.insert("@maxsp_rate",100);
    obj.insert("@mdef_rate",100);
    obj.insert("@minus_state_set", QJsonArray());
    obj.insert("@name","");
    obj.insert("@nonresistance",false);
    obj.insert("@pdef_rate",100);
    obj.insert("@plus_state_set", QJsonArray());
    obj.insert("@rating",5);
    obj.insert("@restriction",0);
    obj.insert("@shock_release_prob",0);
    obj.insert("@slip_damage",false);
    obj.insert("@str_rate",100);
    obj.insert("@zero_hp",false);

    obj.insert("RXClass","RPG::State");

    return obj;
}

QJsonObject Factory::create_new_animation(int id)
{
    QJsonObject obj;
    obj.insert("@animation_hue",0);
    obj.insert("@animation_name","");
    obj.insert("@frame_max",1);


    QJsonObject table;
    table.insert("RXClass", "Table");
    table.insert("shitty_num", 2);
    table.insert("values", QJsonArray());
    table.insert("x", 0);
    table.insert("y", 0);
    table.insert("z", 1);

    QJsonObject frame;
    frame.insert("@cell_data", table);
    frame.insert("@cell_max",0);
    frame.insert("RXClass","RPG::Animation::Frame");

    QJsonArray frames;
    frames.append(frame);

    obj.insert("@frames",frames);
    obj.insert("@id",id);
    obj.insert("@name","");
    obj.insert("@position",1);
    obj.insert("@timings", QJsonArray());
    obj.insert("RXClass","RPG::Animation");

    return obj;
}

QJsonObject Factory::create_new_tileset(int id)
{
    QJsonObject obj;

    QJsonArray autotile_names;
    for (int i = 0; i < 7; i++) autotile_names.append("");

    obj.insert("@autotile_names", autotile_names);
    obj.insert("@battleback_name","");
    obj.insert("@fog_blend_type",0);
    obj.insert("@fog_hue",0);
    obj.insert("@fog_name","");
    obj.insert("@fog_opacity",64);
    obj.insert("@fog_sx",0);
    obj.insert("@fog_sy",0);
    obj.insert("@fog_zoom",200);
    obj.insert("@id",id);
    obj.insert("@name","");
    obj.insert("@panorama_hue",0);
    obj.insert("@panorama_name","");

    QJsonObject table;
    table.insert("RXClass", "Table");
    table.insert("shitty_num", 1);
    QJsonArray array;
    for (int i = 0; i < 384; i++)
        array.append(0);
    table.insert("values", array);
    table.insert("x", 384);
    table.insert("y", 1);
    table.insert("z", 1);

    obj.insert("@passages", table);
    obj.insert("@terrain_tags", table);

    //priorities 5 for the first field
    QJsonArray array_priorties;
    array_priorties.append(5);
    for (int i = 0; i < 383; i++)
        array_priorties.append(0);
    table.insert("values", array_priorties);

    obj.insert("@priorities", table);

    obj.insert("@tileset_name","");
    obj.insert("RXClass","RPG::Tileset");
    return obj;
}

QJsonObject Factory::create_new_commonevent(int id)
{
    QJsonObject obj;
    obj.insert("@id", id);
    QJsonArray event_list;
    event_list.append(this->create_empty_event_list());
    obj.insert("@list", event_list);
    obj.insert("@name","");
    obj.insert("@switch_id",1);
    obj.insert("@trigger",0);



    obj.insert("RXClass","RPG::CommonEvent");
    return obj;
}

QJsonDocument *Factory::create_new_map()
{
    QJsonObject obj;
    obj.insert("@autoplay_bgm", false);
    obj.insert("@autoplay_bgs", false);
    obj.insert("@bgm", this->create_audiofile("",100,100));
    obj.insert("@bgs", this->create_audiofile("",80,100));

    QJsonObject data;
    data.insert("RXClass", "Table");
    data.insert("shitty_num", 3);

    QJsonArray values;
    for (int i = 0; i < 900; i++) //20 x 15 x 3
        values.append(0);

    data.insert("values", values);
    data.insert("x", 20);
    data.insert("y", 15);
    data.insert("z", 3);
    obj.insert("@data", data);

    obj.insert("@encounter_list", QJsonArray());
    obj.insert("@encounter_step", 30);

    QJsonObject events;
    events.insert("RXClass", "dictionary");
    obj.insert("@events", events);

    obj.insert("@height", 15);
    obj.insert("@tileset_id",1);
    obj.insert("@width", 20);

    obj.insert("RXClass", "RPG::Map");
    QJsonDocument *doc = new QJsonDocument;
    doc->setObject(obj);
    return doc;
}

QJsonObject Factory::create_map_info(bool expanded, QString name, int order, int parent, int scroll_x, int scroll_y)
{
    QJsonObject obj;
    obj.insert("@expanded", expanded);
    obj.insert("@name", name);
    obj.insert("@order", order);
    obj.insert("@parent_id", parent);
    obj.insert("@scroll_x", scroll_x);
    obj.insert("@scroll_y", scroll_y);

    obj.insert("RXClass", "RPG::MapInfo");
    return obj;
}

QJsonObject Factory::create_event(int id, int x, int y)
{
    QJsonObject obj;
    obj.insert("@id", id);
    obj.insert("@x", x);
    obj.insert("@y", y);
    obj.insert("@name", QString("Event %1").arg(id));
    obj.insert("@pages", QJsonArray());
    obj.insert("RXClass", "RPG::Event");
    return obj;
}

QJsonObject Factory::create_event_page(bool always_on_top, bool direction_fix, bool step_anime, bool through, bool walk_anime, int move_type, int move_speed, int move_freq, int trigger, QJsonObject condition, QJsonArray list, QJsonObject graphic, QJsonObject move_route)
{
    QJsonObject obj;
    obj.insert("@always_on_top", always_on_top);
    obj.insert("@condition", condition);
    obj.insert("@direction_fix", direction_fix);
    obj.insert("@step_anime", step_anime);
    obj.insert("@through", through);
    obj.insert("@walk_anime", walk_anime);

    obj.insert("@graphic", graphic);

    obj.insert("@list", list);

    obj.insert("@move_route", move_route);

    obj.insert("@move_type", move_type);
    obj.insert("@move_speed", move_speed);
    obj.insert("@move_frequency", move_freq);
    obj.insert("@trigger", trigger);
    obj.insert("RXClass", "RPG::Event::Page");
    return obj;
}

QJsonObject Factory::create_page_condition(bool self_switch_valid, bool switch1_valid, bool switch2_valid, bool variable_valid,
                                           int self_switch_ch, int switch1_id, int switch2_id, int variable_id, int variable_value)
{
    QJsonObject obj;
    if (self_switch_ch == 0)
        obj.insert("@self_switch_ch","A");
    else if (self_switch_ch == 1)
        obj.insert("@self_switch_ch","B");
    else if (self_switch_ch == 2)
        obj.insert("@self_switch_ch","C");
    else if (self_switch_ch == 3)
        obj.insert("@self_switch_ch","D");

    obj.insert("@self_switch_valid",self_switch_valid);
    obj.insert("@switch1_id",switch1_id);
    obj.insert("@switch1_valid", switch1_valid);
    obj.insert("@switch2_id",switch2_id);
    obj.insert("@switch2_valid",switch2_valid);
    obj.insert("@variable_id",variable_id);
    obj.insert("@variable_valid",variable_valid);
    obj.insert("@variable_value", variable_value);
    obj.insert("RXClass", "RPG::Event::Page::Condition");
    return obj;
}

QJsonObject Factory::create_page_graphic(QString character_name, int character_hue, int blend_type, int direction, int opacity, int pattern, int tile_id)
{
    QJsonObject obj;
    obj.insert("@blend_type", blend_type);
    obj.insert("@character_hue", character_hue);
    obj.insert("@character_name", character_name);
    obj.insert("@direction", direction);
    obj.insert("@opacity", opacity);
    obj.insert("@pattern", pattern);
    obj.insert("@tile_id", tile_id);

    obj.insert("RXClass", "RPG::Event::Page::Graphic");
    return obj;
}

QJsonObject Factory::create_color_tone(int r, int g, int b, int gray, bool tone)
{
    QJsonObject obj;
    obj.insert("r", r);
    obj.insert("g", g);
    obj.insert("b", b);
    obj.insert("alpha_gray", gray);

    obj.insert("RXClass", tone ? "Tone" : "Color");
    return obj;
}


