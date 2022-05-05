#include "rpgmoveroute.h"

RPGMoveRoute::RPGMoveRoute(QObject *parent) : QObject(parent)
{

}

void RPGMoveRoute::setParameter(QString param, bool val)
{
    if (param == "@skippable")
        this->skippable = val;
    else if (param == "@repeat")
        this->repeat = val;
}
