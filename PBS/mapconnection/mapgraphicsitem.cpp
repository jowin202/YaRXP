#include "mapgraphicsitem.h"

MapGraphicsItem::MapGraphicsItem(QImage img)
{
    this->w = img.width();
    this->h = img.height();
}

void MapGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

}

QRectF MapGraphicsItem::boundingRect() const
{
    return QRectF(0,0,w,h);
}

int MapGraphicsItem::width()
{
    return this->w;
}

int MapGraphicsItem::height()
{
    return this->h;
}
