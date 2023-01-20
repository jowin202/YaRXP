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

    listwidget->setDragDropMode(QAbstractItemView::InternalMove);

    connect(listwidget, &QListWidget::itemDoubleClicked, this, [=](QListWidgetItem *item) {
        if (dynamic_cast<EventListItem*>(item) != nullptr)
            ((EventListItem*)item)->edit_cell();
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




