#include "rpgeventpage.h"
#include "rpgsystem.h"

RPGEventPage::RPGEventPage(QObject *parent) : QObject(parent)
{


}

QImage RPGEventPage::get_page_graphic(RPGSystem *system)
{
    int direction = this->direction / 2 - 1;
    QImage img(system->characters_dir + this->character_name);

    int w = img.width()/4;
    int h = img.height()/4;

    return img.copy(this->pattern * w, direction*h,w,h);
}


