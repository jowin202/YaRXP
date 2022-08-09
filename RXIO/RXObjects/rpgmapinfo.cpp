#include "rpgmapinfo.h"

RPGMapInfo::RPGMapInfo(QObject *parent) : QObject(parent)
{

}



void RPGMapInfo::set_id(int id)
{
    this->id = id;
}


void RPGMapInfo::debug()
{
    qDebug() << id << " " << name << " " << parent_id << " " << scroll_x << " " << scroll_y << " " << expanded << " " << order;
}
