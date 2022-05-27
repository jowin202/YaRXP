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

void RPGMap::fix_autotile_stuff(QPoint start, int layer)
{
    if (start.x() < 0 || start.x() >= width || start.y() < 0 || start.y() >= height)
        return;

    int offset_center = this->height * this->width * layer;
    offset_center += this->width * start.y() + start.x();

    if (data[offset_center] >= 0x180 || data[offset_center] == 0)
        return; // only for autotiles

    int autotiles_num = data[offset_center]/0x30 - 1;
    int default_autotile = 0x30 * (autotiles_num+1);



}
