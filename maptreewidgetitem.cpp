#include "maptreewidgetitem.h"

MapTreeWidgetItem::MapTreeWidgetItem()
{

}

void MapTreeWidgetItem::set_id(int id)
{
    this->id = id;
}

void MapTreeWidgetItem::setParameter(QString param, int val)
{
    if (param == "@parent_id")
        this->parent_id = val;
    else if (param == "@scroll_x")
        this->scroll_x = val;
    else if (param == "@scroll_y")
        this->scroll_y = val;
    else if (param == "@order")
        this->order = val;
}

void MapTreeWidgetItem::setName(QString name)
{
    this->name = name;
}

void MapTreeWidgetItem::setExpanded(bool expanded)
{
    this->expanded = expanded;
}

void MapTreeWidgetItem::debug()
{
    qDebug() << "id: " << this->id <<  "\nname: " << this->name  << "\nscroll_x: " << this->scroll_x  << "\nscroll_y: " << this->scroll_y << "\nparent_id: " << this->parent_id<< "\norder: " << this->order;

}
