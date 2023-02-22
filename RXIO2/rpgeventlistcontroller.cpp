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
    listwidget->setSelectionMode(QListWidget::ContiguousSelection);

    connect(listwidget, &QListWidget::itemSelectionChanged, [=](){
        int min = listwidget->count();
        int max = 0;
        for (int i = 0; i < listwidget->selectedItems().count(); i++)
        {
            max = qMax(max,listwidget->row(listwidget->selectedItems().at(i)));
            min = qMin(min,listwidget->row(listwidget->selectedItems().at(i)));
        }

    /*
        if (min != max)
        {
            const QSignalBlocker blocker(listwidget);
            EventListItem *first = dynamic_cast<EventListItem*>(listwidget->item(min));
            EventListItem *last = dynamic_cast<EventListItem*>(listwidget->item(max));
            if (first == nullptr || last == nullptr) return; // avoid null pointer
            int code_first = first->get_obj().value("@code").toInt();
            int code_last = last->get_obj().value("@code").toInt();

            int multiline_codes[] = { 401, 408, 509, 605, 655 };

            for (int c = 0; c < 5; c++)
            {
                if (code_first == multiline_codes[c])
                {
                    int i = min;
                    do
                    {
                        listwidget->item(i)->setSelected(false);
                    } while(i >= 0 && ((EventListItem*)listwidget->item(i--))->get_obj().value("@code").toInt() == multiline_codes[c]);
                }
                if (code_last == multiline_codes[c])
                {
                    int i = max;
                    do
                    {
                        listwidget->item(i)->setSelected(false);
                    } while(++i < listwidget->count() && ((EventListItem*)listwidget->item(i))->get_obj().value("@code").toInt() == multiline_codes[c]);
                }
            }
        }
        */
    });

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
            if (code == 0 || code == 402 || code == 403 || code == 411 || code == 412 || code == 413
                    || code == 509 || code == 601 || code == 602 || code == 603) return; //cant delete this

            //multiline codes with indent lines between
            //show choices, conditional branch, loop
            //battle processing (301) could be a one-liner. check if next code is 601
            if (code == 102 || code == 111 || code == 112 ||
                    (code == 301 && dynamic_cast<EventListItem*>(listwidget->item(row+1)) != nullptr
                     && dynamic_cast<EventListItem*>(listwidget->item(row+1))->get_obj().value("@code").toInt() == 601))
            {
                int last_code = 0;
                if (code == 102) last_code = 404; //choices
                else if (code == 111) last_code = 412; //conditional branch
                else if (code == 112) last_code = 413; //loop
                else if (code == 301) last_code = 604; //battle processing

                int depth = 0;
                delete listwidget->takeItem(row); //remove first
                while (dynamic_cast<EventListItem*>(listwidget->item(row)) != nullptr &&
                       (depth > 0 || dynamic_cast<EventListItem*>(listwidget->item(row))->get_obj().value("@code").toInt() != last_code))
                {
                    if ( dynamic_cast<EventListItem*>(listwidget->item(row))->get_obj().value("@code").toInt() == code)
                        depth++;
                    else if (depth > 0 && dynamic_cast<EventListItem*>(listwidget->item(row))->get_obj().value("@code").toInt() == last_code)
                        depth--;

                    delete listwidget->takeItem(row);
                }
                delete listwidget->takeItem(row); //remove last
            }
            //multiline commands
            else if (code == 101 || code == 108 || code == 209 || code == 355 || code == 302)
            {
                delete listwidget->takeItem(row);
                while (dynamic_cast<EventListItem*>(listwidget->item(row)) != nullptr &&
                       dynamic_cast<EventListItem*>(listwidget->item(row))->get_obj().value("@code").toInt() == (code==302 ? 605 : code+300))
                    delete listwidget->takeItem(row);
            }
            else //delete the rest
                delete listwidget->takeItem(row);
        }
    });



    this->action_copy.setShortcut(QKeySequence(tr("Ctrl+C")));
    this->action_copy.setShortcutContext(Qt::WidgetShortcut);
    listwidget->addAction(&this->action_copy);
    connect(&this->action_copy, &QAction::triggered, [=]()
    {
        int min = listwidget->count();
        int max = 0;
        for (int i = 0; i < listwidget->selectedItems().count(); i++)
        {
            max = qMax(max,listwidget->row(listwidget->selectedItems().at(i)));
            min = qMin(min,listwidget->row(listwidget->selectedItems().at(i)));
        }
        QJsonArray copied_event_commands;
        for (int i = min; i <= max; i++)
        {
            EventListItem *current = dynamic_cast<EventListItem*>(listwidget->item(i));
            if (current == nullptr) continue; // avoid null pointer
            if (i == max && max == this->listwidget->count()-1) continue; //cant copy laste zero code

            copied_event_commands.append(current->get_obj());
        }
        QSettings settings;
        settings.setValue("copied_event_commands", QJsonDocument(copied_event_commands).toJson(QJsonDocument::Compact));
    });



    this->action_paste.setShortcut(QKeySequence(tr("Ctrl+V")));
    this->action_paste.setShortcutContext(Qt::WidgetShortcut);
    listwidget->addAction(&this->action_paste);
    connect(&this->action_paste, &QAction::triggered, [=]()
    {
        QSettings settings;
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(settings.value("copied_event_commands").toByteArray(), &err);
        if (err.error != QJsonParseError::NoError)
            return;

        QJsonArray list = doc.array();
        for (int i = 0; i < list.count(); i++)
        {
            listwidget->insertItem(listwidget->currentRow(),new EventListItem(listwidget,this->mc,this->mic,list.at(i).toObject()));
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




