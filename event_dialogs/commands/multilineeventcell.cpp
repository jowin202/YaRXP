#include "multilineeventcell.h"
#include "RXIO/RXObjects/rpgmovecommand.h"
#include "RXIO/RXObjects/rpgsystem.h"


MultilineEventCell::MultilineEventCell(RPGEventCommand *command, RPGSystem *system, RPGMoveCommand *mvcmd) : SimpleEventCell(command, system)
{
    this->is_simple = false;

    if (code == 209)
    {
        QString actor;
        int e = parameters.at(0).toInt();
        if (e == -1) actor = "Player";
        else if (e==0) actor = "This event";
        else
        {
            QList<RPGEvent*> list = system->get_current_map_info()->map->events;
            for (int i = 0; i < list.length(); i++)
                if (e == list.at(i)->id)
                    actor = list.at(i)->name;
        }
        this->setText(indent_as_string +"@>Set Move Route : " + actor);
    }
    else if (code == 509)
    {
        this->setText(indent_as_string + "                 : " + QString(mvcmd->get_command_text()));
    }
    else
        this->setText(indent_as_string + command->get_command_text(system));



}
