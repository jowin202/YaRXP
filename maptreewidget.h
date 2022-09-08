#ifndef MAPTREEWIDGET_H
#define MAPTREEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QTreeWidget>
#include <QDir>
#include <QMessageBox>
#include <QErrorMessage>
#include "RXIO/RXObjects/rpgmapinfo.h"
#include "RXIO/RXObjects/rpgtileset.h"
#include "RXIO/RXObjects/parserexception.h"

#include "RXIO/iomapinfofile.h"
#include "RXIO/iotilesetfile.h"
#include "RXIO/iomapfile.h"

#include "mappropertiesdialog.h"
#include "RXIO/RXObjects/rpgsystem.h"

#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QHash>

class MapTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    MapTreeWidget(QWidget* parent = 0);
    void setSystem(RPGSystem *system)
    { this->system = system; }

    void handleParserException(ParserException *ex);


    void restore_after_move()
    {
        int order = 1;
        for (int i = 0; i < this->topLevelItemCount(); i++)
        {
            this->restore_order_and_parent(this->topLevelItem(i), &order);
            int toplevel_item_list_id = this->topLevelItem(i)->text(2).toInt();

            system->map_info_list.at(toplevel_item_list_id)->parent_id = 0; //toplevel item has parent 0
        }
    }

    void restore_order_and_parent(QTreeWidgetItem *item, int *current)
    {
        int list_id = item->text(2).toInt();
        int id = system->map_info_list.at(list_id)->id;

        system->map_info_list.at(list_id)->order = (*current);


        item->setText(1,QString("%1").arg((*current)++,3,10,QChar('0')));
        for (int i = 0; i < item->childCount(); i++)
        {
            int child_list_id = item->child(i)->text(2).toInt();
            system->map_info_list.at(child_list_id)->parent_id = id;
            this->restore_order_and_parent(item->child(i), current);
        }
    }


public slots:
    void list_maps();
    void clicked_at_item(QTreeWidgetItem *current_item);
    void prepare_context_menu(const QPoint & pos );
    void show_map_properties_dialog();
    void do_save();

    void itemExpanded(QTreeWidgetItem *item)
    {
        if (item->text(2).toInt() >= 0 && item->text(2).toInt() < system->map_info_list.length())
            system->map_info_list.at(item->text(2).toInt())->expanded = true;
    }
    void itemCollapsed(QTreeWidgetItem *item)
    {
        if (item->text(2).toInt() >= 0 && item->text(2).toInt() < system->map_info_list.length())
            system->map_info_list.at(item->text(2).toInt())->expanded = false;
    }

    void dropEvent(QDropEvent *event);

signals:
    void on_map_selected(int);

private:
    QHash<int,QTreeWidgetItem*> id_map;

    QMenu menu;
    QAction action1;
    QAction action2;
    QAction action3;
    QAction action4;
    QAction action5;
    QAction action6;
    QAction action7;
    QAction action8;

    RPGSystem *system;

};
#endif // MAPTREEWIDGET_H
