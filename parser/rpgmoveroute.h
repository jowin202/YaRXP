#ifndef RPGMOVEROUTE_H
#define RPGMOVEROUTE_H

#include <QObject>
#include "rpgmovecommand.h"

class RPGMoveRoute : public QObject
{
    Q_OBJECT
public:
    explicit RPGMoveRoute(QObject *parent = nullptr);

signals:
private:
    bool skippable;
    bool repeat;
    QList<RPGMoveCommand*> list;

};

#endif // RPGMOVEROUTE_H
