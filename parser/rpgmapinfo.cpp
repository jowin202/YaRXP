#include "rpgmapinfo.h"

RPGMapInfo::RPGMapInfo(QObject *parent) : QObject(parent)
{

}

void RPGMapInfo::set_id(int id)
{
    this->id = id;
}

void RPGMapInfo::setParameter(QString param, int val)
{
    if (param == "@parent_id")
        this->parent_id = val;
    else if (param == "@scroll_x")
        this->scroll_x = val;
    else if (param == "@scroll_y")
        this->scroll_y = val;
    else if (param == "@order")
        this->order = val;
}

void RPGMapInfo::setParameter(QString param, bool val)
{
    if (param == "@expanded")
        this->expanded = val;
}

void RPGMapInfo::setParameter(QString param, QString val)
{
    if (param == "@name")
        this->name = val;
}

void RPGMapInfo::debug()
{
    qDebug() << id << " " << name << " " << parent_id << " " << scroll_x << " " << scroll_y << " " << expanded << " " << order;
}
