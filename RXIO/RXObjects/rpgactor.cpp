#include "rpgactor.h"
#include "rpgsystem.h"

RPGActor::RPGActor(QObject *parent) : QObject(parent)
{

}

QImage RPGActor::get_character_graphic(RPGSystem *system)
{
    return QImage(system->characters_dir + character_name);
}

QImage RPGActor::get_battler_graphic(RPGSystem *system)
{
     return QImage(system->battlers_dir + battler_name);
}

