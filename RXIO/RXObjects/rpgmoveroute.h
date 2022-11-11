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

    void copy_to(RPGMoveRoute *mvr)
    {
        mvr->skippable = this->skippable;
        mvr->repeat = this->repeat;
        mvr->list.clear();
        for (int i = 0; i < this->list.length(); i++)
            mvr->list.append(this->list.at(i)->copy());
        mvr->param_order = this->param_order;
    }

    QStringList param_order;
signals:
private:
};

#endif // RPGMOVEROUTE_H
