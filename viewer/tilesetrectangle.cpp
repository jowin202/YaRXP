#include "tilesetrectangle.h"


TilesetRectangle::TilesetRectangle(int w, int h)
{
    this->w = w;
    this->h = h;
}

void TilesetRectangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QPen pen;
    pen.setWidth(4);
    //pen.setColor(QColor(150,150,150));
    painter->setPen(pen);

    painter->drawRect(0,0,32*w,32*h);
}

QRectF TilesetRectangle::boundingRect() const
{
    return QRectF(0,0,32*w,32*h);
}
