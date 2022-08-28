#include "eventlist.h"
#include "commands/multilineeventcell.h"
#include "commands/simpleeventcell.h"

#include <QComboBox>

EventList::EventList(QWidget *parent) : QListWidget(parent)
{

}

void EventList::fill_list()
{
    RPGEventCommand *command;

    //Multiline Commands
    RPGMoveRoute *current_move_route = 0;
    RPGMoveCommand *current_move_command = 0;
    int current_mv_index = 0;




    for (int i = 0; i < list->length(); i++)
    {
        command = list->at(i);
        //qDebug() << command->code;

        //Multiline Commands
        if (command->code == 101 || command->code == 401 ||
                command->code == 209 || command->code == 509 ||
                command->code == 108 || command->code == 408 ||
                command->code == 355 || command->code == 655)
        {

            if (command->code == 209)
            {
                current_move_route = &command->move_route;
            }
            else if (command->code == 509)
            {
                current_move_command = current_move_route->list.at(current_mv_index++);
            }
            else
            {
                current_mv_index = 0;
                current_move_route = 0;
                current_move_command = 0;
            }
            this->addItem(new MultilineEventCell(command, system, current_move_command));
        }
        //Single Line Commands
        else
            this->addItem(new SimpleEventCell(command,system));


    }
}
