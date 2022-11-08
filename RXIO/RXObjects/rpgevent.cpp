#include "rpgevent.h"

RPGEvent::RPGEvent(QObject *parent) : QObject(parent)
{

}

RPGEvent::~RPGEvent()
{
}

QImage RPGEvent::get_event_pic()
{
    if (this->pages.length() <= 0) return QImage(); //no pic if no page
    if (this->pages.at(0)->graphics.isNull()) return QImage(); //no pic if pic on page 1 is invalid

    int dir_offset = (this->pages.at(0)->direction / 2) - 1;
    int pattern_offset = this->pages.at(0)->pattern; //0,1,2,3

    //direction: 2 down, 4 left, 6 right, 8 up

    int pic_height = this->pages.at(0)->graphics.size().height()/4; //persons have 48 pixels, pokeballs 32
    int pic_width = this->pages.at(0)->graphics.size().width()/4;

    return this->pages.at(0)->graphics.copy(pic_width*pattern_offset,pic_height*dir_offset,32,32);
}
