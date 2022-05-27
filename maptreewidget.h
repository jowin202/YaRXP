#ifndef MAPTREEWIDGET_H
#define MAPTREEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QTreeWidget>
#include <QDir>
#include <QMessageBox>
#include "parser/rxdataparser.h"
#include "parser/rpgmapinfo.h"
#include "parser/rpgtileset.h"

#include "mappropertiesdialog.h"
#include "settings.h"

#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QHash>

class MapTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    MapTreeWidget(QWidget* parent = 0);
    void setSettings(Settings *settings)
    { this->settings = settings; }

public slots:
    void list_maps();
    void clicked_at_item(QTreeWidgetItem *current_item, QTreeWidgetItem *previous);
    void prepare_context_menu(const QPoint & pos );
    void show_map_properties_dialog();

signals:
    void on_map_selected(RPGMap*);
    void on_tileset_changed(int);

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

    Settings *settings;

};
#endif // MAPTREEWIDGET_H
