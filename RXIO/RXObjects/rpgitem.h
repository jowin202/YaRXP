#ifndef RPGITEM_H
#define RPGITEM_H

#include <QObject>
#include "rpgaudiofile.h"

class RPGItem : public QObject
{
    Q_OBJECT
public:
    explicit RPGItem(QObject *parent = nullptr);


    int id;

    int price;
    bool consumable;
    int parameter_type;
    int parameter_points;

    int recover_hp_rate;
    int recover_hp;
    int recover_sp_rate;
    int recover_sp;

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
signals:

};

#endif // RPGITEM_H
