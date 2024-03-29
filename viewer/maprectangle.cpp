#include "maprectangle.h"

MapRectangle::MapRectangle(int w, int h)
{
    this->w = w;
    this->h = h;
}

void MapRectangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QPen pen;
    pen.setWidth(4);
    //pen.setColor(QColor(150,150,150));
    painter->setPen(pen);

    painter->drawRect(2,2,32*w-4,32*h-4);
}

QRectF MapRectangle::boundingRect() const
{
    return QRect(0,0,32*w,32*h);
}
