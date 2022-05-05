#include "rpgeventpage.h"

RPGEventPage::RPGEventPage(QObject *parent) : QObject(parent)
{

}

void RPGEventPage::setParameter(QString param, int val)
{
    if (param == "@move_speed")
        this->move_speed = val;
    else if (param == "@move_frequency")
        this->move_frequency = val;
    else if (param == "@move_type")
        this->move_type = val;
    else if (param == "@trigger")
        this->trigger = val;
}

void RPGEventPage::setParameter(QString param, bool val)
{
    if (param == "@walk_anime")
        this->walk_anime = val;
    else if (param == "@step_anime")
        this->step_anime = val;
    else if (param == "@through")
        this->through = val;
    else if (param == "@direction_fix")
        this->direction_fix = val;
    else if (param == "@always_on_top")
        this->always_on_top = val;
}

void RPGEventPage::setParameter(QString param, RPGEventPageCondition* val)
{
    //if (param == "@condition")
    this->condition = val;
}

