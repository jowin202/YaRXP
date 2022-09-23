#ifndef RPGTROOPMEMBER_H
#define RPGTROOPMEMBER_H

#include <QObject>
#include <QDebug>

#include <QRect>

class RPGTroopMember : public QObject
{
    Q_OBJECT
public:
    explicit RPGTroopMember(QObject *parent = nullptr);
    //~RPGTroopMember() {qDebug() << "RPGTroopMember deleted"; }

    int x;
    int y;
    int enemy_id;
    bool immortal;
    bool hidden;


    //not included in file
    QRect bounding_rect;

    void copy_to(RPGTroopMember *member)
    {
        member->x = this->x;
        member->y = this->y;
        member->enemy_id = this->enemy_id;
        member->immortal = this->immortal;
        member->hidden = this->hidden;
    }
    QStringList param_order;
signals:

};

#endif // RPGTROOPMEMBER_H
