#include "rpgenemy.h"
#include "rpgsystem.h"

RPGEnemy::RPGEnemy(QObject *parent) : QObject(parent)
{

}

QImage RPGEnemy::get_battler_graphics(RPGSystem* system)
{
    return QImage(system->battlers_dir + battler_name);
}
