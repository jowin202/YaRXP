#include "rpgdb.h"
#include "rpgmapcontroller.h"
#include "rpgmapinfocontroller.h"
#include "rpgeventlistcontroller.h"

#include "events/eventlistitem.h"

RPGEventListController::RPGEventListController(RPGMapController *mc, QListWidget *listwidget)
    : QObject{}
{
    this->mc = mc;
    this->db = mc->getDB();
    this->mic = new RPGMapInfoController(db);
    this->listwidget = listwidget;


    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    listwidget->setFont(font);

    //listwidget->setDragDropMode(QAbstractItemView::InternalMove);

    connect(listwidget, &QListWidget::itemDoubleClicked, this, [=](QListWidgetItem *item) {
        if (dynamic_cast<EventListItem*>(item) != nullptr)
            ((EventListItem*)item)->edit_cell();
    });


    this->action_delete.setShortcut(Qt::Key_Delete);
    this->action_delete.setShortcutContext(Qt::WidgetShortcut);
    listwidget->addAction(&this->action_delete);
    connect(&this->action_delete, &QAction::triggered, [=]()
    {
        int row = listwidget->currentRow();
        if (row >= 0 && dynamic_cast<EventListItem*>(listwidget->item(row)) != nullptr)
        {
            int code = dynamic_cast<EventListItem*>(listwidget->item(row))->get_obj().value("@code").toInt();
            if (code == 0 || code == 509) return; //cant delete this

            if (code == 101 || code == 108 || code == 209 || code == 355)
            {
                delete listwidget->takeItem(row);
                while (dynamic_cast<EventListItem*>(listwidget->item(row)) != nullptr &&
                       dynamic_cast<EventListItem*>(listwidget->item(row))->get_obj().value("@code").toInt() == code+300)
                    delete listwidget->takeItem(row);

            }
            else //delete the rest
                delete listwidget->takeItem(row);
        }
    });
}

void RPGEventListController::fill_list(QJsonArray list)
{
    listwidget->clear();
    for (int i = 0; i < list.count(); i++)
    {
        EventListItem *item = new EventListItem(listwidget, this->mc, this->mic, list.at(i).toObject());
        listwidget->addItem(item);
    }
}

QJsonArray RPGEventListController::get_list()
{
    QJsonArray result;
    for (int i = 0; i < this->listwidget->count(); i++)
    {
        if (dynamic_cast<EventListItem*>(this->listwidget->item(i)) != nullptr)
            result.append( ((EventListItem*)this->listwidget->item(i))->get_obj() );
    }

    return result;
}




