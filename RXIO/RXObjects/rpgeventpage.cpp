#include "rpgeventpage.h"
#include "rpgsystem.h"

RPGEventPage::RPGEventPage(QObject *parent) : QObject(parent)
{


}

QImage RPGEventPage::get_page_graphic(RPGSystem *system)
{
    if (this->tile_id != 0)
    {
        if (system->get_current_map_info() != 0 && system->get_current_map_info()->map != 0)
        {
            if (system->tileset_hash.contains(system->get_current_map_info()->map->tileset_id))
            {
                RPGTileset *tileset = system->tileset_hash.value(system->get_current_map_info()->map->tileset_id);
                return tileset->getTile(this->tile_id);
            }
        }
    }

    int direction = this->direction / 2 - 1;
    QImage img(system->characters_dir + this->character_name);

    int w = img.width()/4;
    int h = img.height()/4;

    return img.copy(this->pattern * w, direction*h,w,h);
}


