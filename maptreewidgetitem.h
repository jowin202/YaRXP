#ifndef MAPTREEWIDGETITEM_H
#define MAPTREEWIDGETITEM_H

#include <QTreeWidgetItem>
#include <iostream>
#include <QDebug>
#include <QObject>
#include <QWidget>

class MapTreeWidgetItem : public QObject
{
    Q_OBJECT
public:
    MapTreeWidgetItem();

    void set_id(int id);
    void setParameter(QString param, int val);
    void setName(QString name);
    void setExpanded(bool expanded);
    void debug();
//everything public
    int id;
    int parent_id;
    bool expanded;
    int scroll_x;
    int scroll_y;
    int order;
    QString name;
};

#endif // MAPTREEWIDGETITEM_H
