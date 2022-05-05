#include "rpgevent.h"

RPGEvent::RPGEvent(QObject *parent) : QObject(parent)
{

}

void RPGEvent::set_id(int id)
{
    this->id = id;
}

void RPGEvent::setParameter(QString param, int val)
{

}

void RPGEvent::setParameter(QString param, QString val)
{

}
