#ifndef RPGEVENTPAGE_H
#define RPGEVENTPAGE_H

#include <QObject>
#include "rpgeventpagecondition.h"
#include "rpgeventpagegraphic.h"
#include "rpgeventcommand.h"
#include "rpgmoveroute.h"

class RPGEventPage : public QObject
{
    Q_OBJECT
public:
    explicit RPGEventPage(QObject *parent = nullptr);

signals:

private:
    int move_speed;
    int move_frequency;
    bool walk_anime;
    bool step_anime;
    RPGMoveRoute route;

    bool through;
    int trigger;
    RPGEventPageCondition condition;
    RPGEventPageGraphic graphic;

    bool direction_fix;
    bool always_on_top;
    int move_type;
    QList<RPGEventCommand*> list;

};

#endif // RPGEVENTPAGE_H
