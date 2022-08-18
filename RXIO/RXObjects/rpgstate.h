#ifndef RPGSTATE_H
#define RPGSTATE_H

#include <QObject>
#include "rpgstring.h"

class RPGState : public QObject
{
    Q_OBJECT
public:
    explicit RPGState(QObject *parent = nullptr);

    int id;

    RPGString name;
    int animation_id;
    int restriction;
    bool nonresistance;
    bool zero_hp;
    bool cant_get_exp;
    bool cant_evade;
    bool slip_damage;

    int rating;
    int hit_rate;
    int maxhp_rate;
    int maxsp_rate;
    int str_rate;
    int dex_rate;
    int agi_rate;
    int int_rate;
    int atk_rate;
    int pdef_rate;
    int mdef_rate;
    int eva;

    bool battle_only;
    int hold_turn;
    int auto_release_prob;
    int shock_release_prob;

    QList<int> guard_element_set;
    QList<int> plus_state_set;
    QList<int> minus_state_set;


    QStringList param_order;
signals:

};

#endif // RPGSTATE_H
