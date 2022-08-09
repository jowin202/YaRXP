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
#include "RXIO/RXObjects/rpgsettings.h"

#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QHash>

class MapTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    MapTreeWidget(QWidget* parent = 0);
    void setSettings(RPGSettings *settings)
    { this->settings = settings; }

    void handleParserException(ParserException *ex);

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
    QAction action8;

    RPGSettings *settings;


};
#endif // MAPTREEWIDGET_H
