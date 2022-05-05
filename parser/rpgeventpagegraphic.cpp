#include "rpgeventpagegraphic.h"

RPGEventPageGraphic::RPGEventPageGraphic(QObject *parent) : QObject(parent)
{

}

void RPGEventPageGraphic::setParameter(QString param, int val)
{
    if (param == "@direction")
        this->direction = val;
    else if (param == "@blend_type")
        this->blend_type = val;
    else if (param == "@tile_id")
        this->tile_id = val;
    else if (param == "@pattern")
        this->pattern = val;
    else if (param == "@character_hue")
        this->character_hue = val;
    else if (param == "@opacity")
        this->opacity = val;
}

void RPGEventPageGraphic::setParameter(QString param, QString val)
{
    //if (param == "@character_name")
    this->character_name = val;
}
