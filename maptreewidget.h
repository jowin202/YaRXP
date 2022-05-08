#ifndef MAPTREEWIDGET_H
#define MAPTREEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QTreeWidget>
#include <QDir>
#include <QMessageBox>
#include "parser/rxdataparser.h"
#include "parser/rpgmapinfo.h"

#include <QDebug>
#include <QMenu>
#include <QAction>

class MapTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    MapTreeWidget(QWidget* parent = 0);

public slots:
    void list_maps(QString project_dir);
    void clicked_at_item(QTreeWidgetItem *item, int column);
    void prepare_context_menu(const QPoint & pos );

signals:
    void on_map_selected(RPGMap*);

private:
    QString project_dir;
    QString map_info_file;
    QList<RPGMapInfo*> map_list;
    QHash<int,QTreeWidgetItem*> id_map;

    QMenu menu;
    QAction action1;
    QAction action2;
    QAction action3;
    QAction action4;
    QAction action5;
    QAction action6;

};
#endif // MAPTREEWIDGET_H
