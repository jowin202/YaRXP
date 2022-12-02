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
