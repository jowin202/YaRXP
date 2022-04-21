#ifndef MAPTREEWIDGET_H
#define MAPTREEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QTreeWidget>
#include <QDir>
#include <QMessageBox>
#include "parser/rxdataparser.h"
#include "maptreewidgetitem.h"

#include <QDebug>

class MapTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    MapTreeWidget(QWidget* parent = 0);

public slots:
    void list_maps(QString project_dir);
    void clicked_at_item(QTreeWidgetItem *item, int column);


private:
    QString project_dir;
    QString map_info_file;
    QList<MapTreeWidgetItem*> map_list;
    QHash<int,QTreeWidgetItem*> id_map;
};

#endif // MAPTREEWIDGET_H