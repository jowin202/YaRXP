#include "rpgsystem.h"

RPGSystem::RPGSystem(QObject *parent) : QObject(parent)
{
    datasource.setSystem(this);
}
