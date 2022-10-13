#ifndef RPGSKILL_H
#define RPGSKILL_H

#include <QObject>
#include "rpgaudiofile.h"

class RPGSkill : public QObject
{
    Q_OBJECT
public:
    explicit RPGSkill(QObject *parent = nullptr);

    int id;

    int sp_cost;
    int power;
    int atk_f;
    int eva_f;

    int str_f;
    int dex_f;
    int agi_f;
    int int_f;

    int hit;
    int pdef_f;
    int mdef_f;
    int variance;

    int scope;
    int occasion;
    int animation1_id; // in gui as "user"
    int animation2_id; // in gui as "target"

    RPGAudioFile menu_se; // in gui as "menu use"
    int common_event_id;

    RPGString name;
    RPGString icon_name;
    RPGString description;

    QList<int> element_set;
    QList<int> plus_state_set;
    QList<int> minus_state_set;




    QStringList param_order;
    QByteArray to_json();
    QByteArray to_hash();
signals:

};

#endif // RPGSKILL_H
