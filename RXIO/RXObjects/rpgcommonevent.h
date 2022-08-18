#ifndef RPGCOMMONEVENT_H
#define RPGCOMMONEVENT_H

#include <QObject>
#include "rpgstring.h"
#include "rpgeventcommand.h"

class RPGCommonEvent : public QObject
{
    Q_OBJECT
public:
    explicit RPGCommonEvent(QObject *parent = nullptr);

    int id;
    int switch_id;
    int trigger;

    QList<RPGEventCommand*> list;

    RPGString name;


    QStringList param_order;
signals:

};

#endif // RPGCOMMONEVENT_H
