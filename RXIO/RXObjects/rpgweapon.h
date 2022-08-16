#ifndef RPGWEAPON_H
#define RPGWEAPON_H

#include <QObject>
#include "rpgstring.h"

class RPGWeapon : public QObject
{
    Q_OBJECT
public:
    explicit RPGWeapon(QObject *parent = nullptr);

    RPGString name;
    RPGString icon_name;
    RPGString description;

    int id;

    int animation1_id;
    int animation2_id;

    int price;
    int atk;
    int pdef;
    int mdef;

    int str_plus;
    int dex_plus;
    int agi_plus;
    int int_plus;

    QList<int> element_set;
    QList<int> plus_state_set;
    QList<int> minus_state_set;

    QStringList param_order;
signals:

};

#endif // RPGWEAPON_H
