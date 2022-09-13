#include "rpgenemyaction.h"
#include "rpgsystem.h"

RPGEnemyAction::RPGEnemyAction(QObject *parent) : QObject(parent)
{

}

QString RPGEnemyAction::get_condition_as_string()
{
    QString cond;

    if (condition_turn_a != 0 || condition_turn_b != 1) // deactivated if (a,b) = (0,1)
    {
        if (condition_turn_a == 0 && condition_turn_b == 0) cond = "Turn 0";
        else if (condition_turn_a == 0 && condition_turn_b > 1) cond = QString("Turn %1X").arg(condition_turn_b);
        else if (condition_turn_a > 0 && condition_turn_b == 0) cond = QString("Turn %1").arg(condition_turn_a);
        else cond = QString("Turn %1+%2X").arg(condition_turn_a).arg(condition_turn_b);
    }

    if (condition_hp != 100)
    {
        if (cond != "")
            cond.append(" & ");

        cond.append(QString("%1\% HP or below").arg(this->condition_hp));
    }

    if (condition_level != 1)
    {
        if (cond != "")
            cond.append(" & ");

        cond.append(QString("Level %1 or above").arg(this->condition_level));
    }

    if (condition_switch_id != 0)
    {
        if (cond != "")
            cond.append(" & ");

        cond.append(QString("Switch [%1] is ON").arg(this->condition_switch_id,4,10,QChar('0')));
    }


    if (cond == "")
        cond = "None";


    return cond;
}

QString RPGEnemyAction::get_action_as_string(RPGSystem *system)
{
    if (kind == 0)
        return (QStringList() << "Attack" << "Defend" << "Escape" << "Do Nothing").at(basic);
    else
        return system->datasource.get_obj_name_by_id(skill_id,RPGSystem::SKILLS,false,0,false);
}
