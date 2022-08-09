#ifndef RPGMOVEROUTE_H
#define RPGMOVEROUTE_H

#include <QObject>
#include <QDebug>
#include "rpgmovecommand.h"

class RPGMoveRoute : public QObject
{
    Q_OBJECT
public:
    explicit RPGMoveRoute(QObject *parent = nullptr);
    void debug();

    bool skippable;
    bool repeat;
    QList<RPGMoveCommand*> list;



signals:
private:
};

#endif // RPGMOVEROUTE_H
