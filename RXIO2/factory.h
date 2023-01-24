#ifndef FACTORY_H
#define FACTORY_H

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
class Factory
{
public:
    Factory();

    QJsonObject create_learnings();
    QJsonObject create_learnings(int level, int skill_id);

    QJsonObject create_audiofile();
    QJsonObject create_audiofile(QString name, int volume, int pitch);

    QJsonObject create_enemy_action();
    QJsonObject create_enemy_action(int turn_a, int turn_b, int hp, int level, int switch_id, int kind, int skill, int rating, int basic);

    QJsonObject create_troop_member();
    QJsonObject create_troop_member(int enemy_id, bool hidden, bool immortal, int x, int y);

    QJsonObject create_empty_event();
    QJsonObject create_event(int code, int indent, QJsonArray parameters);

    QJsonObject create_troop_page_condition(int actor_hp, int actor_id, bool actor_valid, int enemy_hp, int enemy_index, bool enemy_valid, int switch_id, bool switch_valid, int turn_a, int turn_b, bool turn_valid);
    QJsonObject create_troop_page_condition();

    QJsonObject create_troop_page();

    QJsonObject create_animation_timing();
    QJsonObject create_animation_timing(int frame, QJsonObject audiofile, int condition, int flash_scope, int r, int g, int b, int strength, int duration);



    //New Objects
    QJsonObject create_new_actor(int id);
    QJsonObject create_new_class(int id, int element_num, int state_num);
    QJsonObject create_new_skill(int id);
    QJsonObject create_new_item(int id);
    QJsonObject create_new_weapon(int id);
    QJsonObject create_new_armor(int id);
    QJsonObject create_new_enemy(int id, int element_num, int state_num);
    QJsonObject create_new_troop(int id);
    QJsonObject create_new_state(int id);
    QJsonObject create_new_animation(int id);
    QJsonObject create_new_tileset(int id);
    QJsonObject create_new_commonevent(int id);

    QJsonDocument *create_new_map();
    QJsonObject create_map_info(bool expanded, QString name, int order, int parent, int scroll_x, int scroll_y);


    QJsonObject create_event(int id, int x, int y);
    QJsonObject create_event_page(bool always_on_top, bool direction_fix, bool step_anime, bool through, bool walk_anime, int move_type,
                                  int move_speed, int move_freq, int trigger, QJsonObject condition, QJsonArray list, QJsonObject graphic, QJsonObject move_route);
    QJsonObject create_page_condition(bool self_switch_valid, bool switch1_valid, bool switch2_valid, bool variable_valid,
                                      int self_switch_ch, int switch1_id, int switch2_id, int variable_id, int variable_value);
    QJsonObject create_page_graphic(QString character_name, int character_hue, int blend_type, int direction, int opacity, int pattern, int tile_id);


    QJsonObject create_color_tone(int r, int g, int b, int gray, bool tone);
};

#endif // FACTORY_H
