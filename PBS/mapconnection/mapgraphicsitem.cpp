#include "mapgraphicsitem.h"

MapGraphicsItem::MapGraphicsItem()
{
}

void MapGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (!img.isNull())
        painter->drawImage(0,0,img);
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

void MapGraphicsItem::setImage(QImage img)
{
    this->w = img.width();
    this->h = img.height();
    this->img = img;
}

void MapGraphicsItem::addLink(QRect link_area, int dest)
{
    link current_link;
    current_link.rect = link_area;
    current_link.dest = dest;
    this->link_list.append(current_link);
}

void MapGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPoint click_dest = QPoint(event->pos().x(), event->pos().y());
    for (int i = 0; i < link_list.count(); i++)
    {
        if (link_list.at(i).rect.contains(click_dest))
        {
            emit mouse_clicked(link_list.at(i).dest);
            return;
        }
    }
}
