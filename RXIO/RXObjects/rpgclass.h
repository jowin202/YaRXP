#ifndef RPGCLASS_H
#define RPGCLASS_H

#include <QObject>
#include "rpgstring.h"

class RPGClass : public QObject
{
    Q_OBJECT
public:
    explicit RPGClass(QObject *parent = nullptr);

    int id;
    int position;
    RPGString name;

    QList<int> element_ranks;
    QList<int> state_ranks;

    QList<int> weapon_set;
    QList<int> armor_set;


    QList<int> learnings_skill_id;
    QList<int> learnings_level;
    bool learning_order;

    QStringList param_order;



    QByteArray to_json();
    QByteArray to_hash();

signals:

};

#endif // RPGCLASS_H
