#include "maptile.h"
#include "RXIO2/rpgmapcontroller.h"
#include "mapview.h"

MapTile::MapTile(RPGMapController *mc, QPoint pos, tile_options *options)
{
    this->mc = mc;
    this->pos = pos;
    this->opt = options;
}

void MapTile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    QImage img1 = mc->get_tile_from_pos(this->pos, 0);
    QImage img2 = mc->get_tile_from_pos(this->pos, 1);
    QImage img3 = mc->get_tile_from_pos(this->pos, 2);

    painter->fillRect(0,0,32,32, purple);

    painter->setOpacity(1.0);
    painter->drawImage(0,0, img1);


    if (!opt->current_and_below || opt->layer >= 1)
    {
        if (opt->layer < 1)
            painter->setOpacity(0.5);
        painter->drawImage(0,0, img2);

        if (!opt->current_and_below || opt->layer == 2)
        {
            if (opt->layer < 2)
                painter->setOpacity(0.5);

            painter->drawImage(0,0, img3);
        }
    }

    if (opt->mode == MapView::EVENT)
    {
        painter->drawRect(0,0,32,32);
        QJsonObject event = mc->event_on_pos(pos);
        if (event.contains("RXClass"))
        {
            painter->setOpacity(0.5);
            painter->fillRect(0,0,32,32, Qt::white);
            painter->setOpacity(0.8);
            //painter->drawImage(QPoint(0,0),event->get_event_pic());
            painter->setOpacity(1);

            QPen pen;
            pen.setColor(Qt::white);
            pen.setWidth(3);
            painter->setPen(pen);
            painter->drawRect(2,2,28,28);
        }
        if (opt->marked_tile.x() == this->pos.x() && opt->marked_tile.y() == this->pos.y())
        {
            painter->setOpacity(1);
            QPen pen;
            pen.setColor(Qt::black);
            pen.setWidth(3);
            painter->setPen(pen);
            painter->drawRect(2,2,28,28);
        }
    }

    //for debugging
    //painter->drawRect(0,0,32,32);
}

QRectF MapTile::boundingRect() const
{
    return QRectF(0,0,32,32);
}

