#ifndef RPGACTOR_H
#define RPGACTOR_H

#include <QObject>
#include "rpgstring.h"

class RPGActor : public QObject
{
    Q_OBJECT
public:
    explicit RPGActor(QObject *parent = nullptr);


    RPGString battler_name;
    RPGString name;
    RPGString character_name;

    int id;
    int initial_level;
    int final_level;
    int character_hue;
    int battler_hue;
    int class_id;
    int exp_basis;
    int exp_inflation;
    int weapon_id;
    int armor1_id;
    int armor2_id;
    int armor3_id;
    int armor4_id;
    bool armor1_fix;
    bool armor2_fix;
    bool armor3_fix;
    bool armor4_fix;
    bool weapon_fix;



    QList<int> maxhp;
    QList<int> maxsp;
    QList<int> str;
    QList<int> dex;
    QList<int> agi;
    QList<int> int_var;

    QStringList param_order;

signals:

};

#endif // RPGACTOR_H
