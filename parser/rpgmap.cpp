#include "rpgmap.h"

RPGMap::RPGMap(QObject *parent) : QObject(parent)
{

}

void RPGMap::setParameter(QString param, int val)
{
    if (param == "@width")
        this->width = val;
    else if (param == "@height")
        this->height = val;
    else if (param == "@tileset_id")
        this->tileset_id = val;
    else if (param == "@encounter_step")
        this->encounter_step = val;
}

void RPGMap::setParameter(QString param, bool val)
{
    if (param == "@autoplay_bgs")
        this->autoplay_bgs = val;
    else if (param == "@autoplay_bgm")
        this->autoplay_bgm = val;
}

void RPGMap::setParameter(QString param, RPGAudioFile *val)
{
    if (param == "@bgs")
        this->bgs = val;
    else if (param == "@bgm")
        this->bgm = val;
}

void RPGMap::debug()
{
    if (this->bgs != 0)
        this->bgs->debug();
    if (this->bgm != 0)
        this->bgm->debug();
    qDebug () << this->tileset_id;
}
