#include "rpgmap.h"

RPGMap::RPGMap(QObject *parent) : QObject(parent)
{

}


void RPGMap::debug()
{
    if (this->bgs != 0)
        this->bgs->debug();
    if (this->bgm != 0)
        this->bgm->debug();
    qDebug () << this->tileset_id;
}

RPGEvent* RPGMap::event_on_pos(QPoint pos)
{
    for (int i = 0; i < events.size(); i++)
    {
        if (events.at(i)->x == pos.x() && events.at(i)->y == pos.y())
            return events.at(i);
    }
    return 0;
}
