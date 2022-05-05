#ifndef RPGMOVEROUTE_H
#define RPGMOVEROUTE_H

#include <QObject>
#include "rpgmovecommand.h"

class RPGMoveRoute : public QObject
{
    Q_OBJECT
public:
    explicit RPGMoveRoute(QObject *parent = nullptr);
    void setParameter(QString param, bool val);


    bool skippable;
    bool repeat;
    QList<RPGMoveCommand*> list;



signals:
private:
};

#endif // RPGMOVEROUTE_H
