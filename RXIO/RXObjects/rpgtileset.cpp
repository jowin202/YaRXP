#include "rpgtileset.h"

RPGTileset::RPGTileset(QObject *parent) : QObject(parent)
{
}

QImage RPGTileset::getTile(int tile)
{
    tile -= 0x0180; //remove offset
    int x = tile % 8;
    int y = tile / 8;

    if (x < 0 || y < 0 || this->tileset.isNull())
        return QImage();

    return this->tileset.copy(32*x, 32*y, 32, 32);
}

