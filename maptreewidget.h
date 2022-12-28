#ifndef MAPTREEWIDGET_H
#define MAPTREEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QTreeWidget>
#include <QDir>
#include <QMessageBox>
#include <QErrorMessage>

#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>

#include "dialogs/mappropertiesdialog.h"

#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QHash>

class RPGDB;
class RPGMapInfoController;

class MapTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    MapTreeWidget(QWidget* parent = 0);
    void setDB(RPGDB *db);
    void restore_after_move();
    void restore_order_and_parent(QTreeWidgetItem *item, int *current);


public slots:
    void list_maps();
    void clicked_at_item(QTreeWidgetItem *current_item);
    void prepare_context_menu(const QPoint & pos );
    void show_map_properties_dialog();

    void itemExpanded(QTreeWidgetItem *item);
    void itemCollapsed(QTreeWidgetItem *item);

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

    RPGDB *db;
    RPGMapInfoController *mic;

};
#endif // MAPTREEWIDGET_H
