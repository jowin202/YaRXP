#ifndef RPGENEMYACTION_H
#define RPGENEMYACTION_H

#include <QObject>

class RPGEnemyAction : public QObject
{
    Q_OBJECT
public:
    explicit RPGEnemyAction(QObject *parent = nullptr);

    int condition_level;
    int condition_switch_id;
    int kind;
    int condition_turn_b;
    int skill_id;
    int rating;
    int condition_turn_a;
    int condition_hp;
    int basic;


    QStringList param_order;
signals:

};

#endif // RPGENEMYACTION_H
