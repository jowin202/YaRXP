#include "rpgmapinfo.h"

#include "RXIO/iomapfile.h"
#include "RXIO/RXObjects/rpgsystem.h"

RPGMapInfo::RPGMapInfo(QObject *parent) : QObject(parent)
{
}



void RPGMapInfo::set_id(int id)
{
    this->id = id;
}

void RPGMapInfo::load_map(RPGSystem *system)
{
    this->map = new RPGMap();
    IOMapFile iomap_file(system->data_dir +  "Map" + QString::number(this->id).rightJustified(3,'0') + ".rxdata", map);
    map->load_event_graphics(system);
}

void RPGMapInfo::save_map(RPGSystem *system)
{
    if (this->map != 0)
    {
        IOMapFile mapfile;
        mapfile.write_to_file(system->data_dir +  "Map" + QString::number(this->id).rightJustified(3,'0') + ".rxdata", this->map);
    }
}


void RPGMapInfo::debug()
{
    qDebug() << id << " " << name << " " << parent_id << " " << scroll_x << " " << scroll_y << " " << expanded << " " << order;
}
