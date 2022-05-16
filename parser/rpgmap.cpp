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
