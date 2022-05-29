#include "eventlist.h"


#include <QComboBox>

EventList::EventList(QWidget *parent) : QListWidget(parent)
{

}

void EventList::fill_list()
{
    RPGEventCommand *command;
    RPGEventCommand *prev_command = 0;

    for (int i = 0; i < list->length(); i++)
    {
        command = list->at(i);

        if (command->code == 101)
        {
            this->addItem(new ShowText(command->parameters.first().toString(), command->indent, command->code));
        }
        if (command->code == 401)
        {
            this->addItem(new ShowText(command->parameters.first().toString(), command->indent, command->code));
        }

        prev_command = command;
    }
}
