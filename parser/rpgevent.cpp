#include "rpgevent.h"

RPGEvent::RPGEvent(QObject *parent) : QObject(parent)
{

}

void RPGEvent::setParameter(QString param, int val)
{
    if (param == "@id")
        this->id = val;
    else if (param == "@x")
        this->x = val;
    else if (param == "@y")
        this->y = val;
}

void RPGEvent::setParameter(QString param, QString val)
{
    //if (param == "@name")
    this->name = val;
}
