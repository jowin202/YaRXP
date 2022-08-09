#include "rpgmoveroute.h"

RPGMoveRoute::RPGMoveRoute(QObject *parent) : QObject(parent)
{

}


void RPGMoveRoute::debug()
{
    qDebug() << skippable << " " << repeat << " ";

}
