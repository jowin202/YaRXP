#ifndef RPGENEMY_H
#define RPGENEMY_H

#include <QObject>
#include "rpgenemyaction.h"

class RPGSystem;
class RPGEnemy : public QObject
{
    Q_OBJECT
public:
    explicit RPGEnemy(QObject *parent = nullptr);

    QImage get_battler_graphics(RPGSystem* system);

    int id;

    QString name;
    QString battler_name;
    int battler_hue;

    int maxhp;
    int maxsp;

    int str;
    int dex;

    int agi;
    int int_var; //cant name a variable int

    int atk;
    int pdef;

    int mdef;
    int eva;

    int animation1_id; //Attacker
    int animation2_id; //Target

    int exp;
    int gold;

    int treasure_prob;
    int armor_id;
    int weapon_id;
    int item_id;


    QList<int> element_ranks;
    QList<int> state_ranks;

    QList<RPGEnemyAction*> action;

    QStringList param_order;
signals:

};

#endif // RPGENEMY_H
