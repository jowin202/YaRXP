#include "rpgtileset.h"
#include "rpgsystem.h"

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

QImage RPGTileset::getAutoTile(int autotileset, int tile)
{
    if (this->autotiles.length() > autotileset)
        return this->autotiles[autotileset].getTile(tile);
    else return QImage();
}

void RPGTileset::load_tileset_graphic(RPGSystem *system)
{
    //Load tileset image (only once)
    if (this->tileset.isNull() && this->tileset_name != "") //if tileset_name == "", no tileset specified
    {
        this->tileset = QImage(system->tileset_dir + this->tileset_name); //extension automatically recognized
        if (this->tileset.isNull())
            this->tileset = QImage(system->tileset_dir + this->tileset_name + ".PNG");
    }


    if (this->autotiles.length() < 7)
    {
        for (int i = 0; i < this->autotile_names.length(); i++)
        {
            QImage autotiles = QImage(system->autotiles_dir + this->autotile_names.at(i)); //extension automatically recognized
            this->autotiles.append(Autotileset(autotiles));
        }
    }

}

