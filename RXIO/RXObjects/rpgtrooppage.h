#ifndef RPGTROOPPAGE_H
#define RPGTROOPPAGE_H

#include <QObject>
#include "rpgeventcommand.h"

class RPGTroopPage : public QObject
{
    Q_OBJECT
public:
    explicit RPGTroopPage(QObject *parent = nullptr);

    bool enemy_valid;
    bool switch_valid;
    bool turn_valid;
    bool actor_valid;

    int switch_id;
    int turn_a;
    int turn_b;
    int enemy_index;
    int enemy_hp;
    int actor_hp;
    int actor_id;

    int span;

    QList <RPGEventCommand*> list;




    QString get_condition_as_string(RPGSystem *system, QStringList enemy_list);



    QStringList condition_param_order;
    QStringList param_order;
signals:

};

#endif // RPGTROOPPAGE_H
