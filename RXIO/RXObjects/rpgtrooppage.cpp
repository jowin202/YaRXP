#include "rpgtrooppage.h"
#include "rpgsystem.h"

RPGTroopPage::RPGTroopPage(QObject *parent) : QObject(parent)
{

}

QString RPGTroopPage::get_condition_as_string(RPGSystem *system, QStringList enemy_list)
{
    QString condition;

    if (this->turn_valid)
    {
        condition += "Turn ";
        if (turn_a == 0 && turn_b == 0)
            condition += "0";
        else if (turn_a == 0 && turn_b > 0)
            condition += QString("%1X").arg(turn_b);
        else if (turn_a > 0 && turn_b == 0)
            condition += QString("%1").arg(turn_a);
        else if (turn_a > 0 && turn_b > 0)
            condition += QString("%1+%2X").arg(turn_a).arg(turn_b);
    }

    if (enemy_valid)
    {
        if (condition != "") condition += " & ";
        condition += QString("Enemy [%1. %2]'s HP %3\% or below")
                .arg(enemy_index+1).arg(enemy_list.at(enemy_index)).arg(enemy_hp);

    }

    if (actor_valid)
    {
        if (condition != "") condition += " & ";
        condition += QString("Actor [%1]'s HP %2\% or below")
                .arg(system->datasource.get_obj_name_by_id(actor_id, RPGSystem::ACTORS, false, -1, false))
                .arg(actor_hp);

    }

    if (switch_valid)
    {
        if (condition != "") condition += " & ";
        condition += QString("Switch %1 is ON").arg(system->datasource.get_switch_name(switch_id, switch_id, true, 4,true));
    }

    return condition;
}
