#ifndef RPGTROOP_H
#define RPGTROOP_H

#include <QObject>
#include "rpgstring.h"
#include "rpgtroopmember.h"
#include "rpgtrooppage.h"

class RPGTroop : public QObject
{
    Q_OBJECT
public:
    explicit RPGTroop(QObject *parent = nullptr);

    RPGString name;
    int id;


    QList<RPGTroopPage*> pages;
    QList<RPGTroopMember*> members;

    QStringList param_order;
signals:

};

#endif // RPGTROOP_H
