#ifndef RPGARMOR_H
#define RPGARMOR_H

#include <QObject>
#include "rpgstring.h"

class RPGArmor : public QObject
{
    Q_OBJECT
public:
    explicit RPGArmor(QObject *parent = nullptr);

    RPGString name;
    RPGString icon_name;
    RPGString description;

    int id;

    int kind;
    int auto_state_id;

    int price;
    int pdef;
    int mdef;
    int eva;

    int str_plus;
    int dex_plus;
    int agi_plus;
    int int_plus;

    QList<int> guard_element_set;
    QList<int> guard_state_set;


    QStringList param_order;
signals:

};

#endif // RPGARMOR_H
