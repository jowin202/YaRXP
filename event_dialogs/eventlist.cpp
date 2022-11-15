#include "eventlist.h"
#include "commands/multilineeventcell.h"
#include "commands/simpleeventcell.h"

#include <QComboBox>

EventList::EventList(QWidget *parent) : QListWidget(parent)
{
    this->setSelectionMode(QAbstractItemView::ContiguousSelection);
    connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(fix_selections()));
}

void EventList::import_list()
{
    RPGEventCommand *command;

    if (this->list->length() > 0)
        this->param_order = this->list->first()->param_order;

    //Multiline Commands
    MultilineEventCell *last_head = 0;
    RPGMoveRoute *current_move_route = 0;
    RPGMoveCommand *current_move_command = 0;
    int current_mv_index = 0;


    for (int i = 0; i < list->length(); i++)
    {
        command = list->at(i);
        //command->debug();
        //qDebug() << i << command->code << command->get_command_text(system);


        //Multiline Commands
        if (command->code == 101 || command->code == 401 || //text
                command->code == 209 || command->code == 509 || //move route
                command->code == 108 || command->code == 408 || //comment
                command->code == 355 || command->code == 655 || //script
                command->code == 302 || command->code == 605 ) //shop processing
        {

            if (command->code == 209)
            {
                current_move_route = &command->move_route;
                current_mv_index = 0;
                current_move_command = 0;
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

            MultilineEventCell *cell = new MultilineEventCell(command, system, current_move_command);
            if (cell->is_head)
                last_head = cell;
            else
            {
                last_head->tail.append(cell);
                cell->head = last_head;
            }
            this->addItem(cell);
        }
        //Single Line Commands
        else
        {
            //breaks multi line move route
            current_mv_index = 0;
            current_move_route = 0;
            current_move_command = 0;

            this->addItem(new SimpleEventCell(command,system));
        }
    }
}

void EventList::export_list()
{
    this->list->clear();

    for (int i = 0; i < this->count(); i++)
    {
        SimpleEventCell *current_cell = (SimpleEventCell*)this->item(i);
        RPGEventCommand *command = new RPGEventCommand;
        command->code = current_cell->code;
        command->indent = current_cell->indent;
        command->parameters = current_cell->parameters;

    }
}

void EventList::fix_selections()
{
    for (int i = 0; i < this->selectedItems().length(); i++)
    {
        if ( ((SimpleEventCell*)this->selectedItems().at(i))->multiline )
        {
            if ( ((MultilineEventCell*)this->selectedItems().at(i))->is_head )
            {
                    //select everything
                    QList<MultilineEventCell*> *tail = &((MultilineEventCell*)this->selectedItems().at(i))->tail;
                    for (int j = 0; j < tail->length(); j++)
                    {
                        tail->at(j)->setSelected(true);
                    }
            }
            else // not head, but multiline
            {
                ((MultilineEventCell*)this->selectedItems().at(i))->head->setSelected(true);
            }
        }
    }
}
