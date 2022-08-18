#ifndef RPGTROOPMEMBER_H
#define RPGTROOPMEMBER_H

#include <QObject>

class RPGTroopMember : public QObject
{
    Q_OBJECT
public:
    explicit RPGTroopMember(QObject *parent = nullptr);

    int x;
    int y;
    int enemy_id;
    bool immortal;
    bool hidden;

    QStringList param_order;
signals:

};

#endif // RPGTROOPMEMBER_H
