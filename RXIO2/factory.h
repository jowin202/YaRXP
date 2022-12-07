#ifndef FACTORY_H
#define FACTORY_H

#include <QJsonObject>

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
};

#endif // FACTORY_H
