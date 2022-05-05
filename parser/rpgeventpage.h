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
    void setParameter(QString param, int val);
    void setParameter(QString param, bool val);
    void setParameter(QString param, RPGEventPageCondition *val);


    int move_speed;
    int move_frequency;
    int move_type;
    int trigger;

    bool walk_anime;
    bool step_anime;
    bool through;
    bool direction_fix;
    bool always_on_top;

    RPGMoveRoute *route;

    RPGEventPageCondition *condition;
    RPGEventPageGraphic *graphic;

    QList<RPGEventCommand*> list;

signals:

private:


};

#endif // RPGEVENTPAGE_H
