#include "maptile.h"
#include "RXIO/RXObjects/rpgmap.h"
#include "mapview.h"

MapTile::MapTile(RPGMap *map, RPGTileset *tileset, QPoint pos, tile_options *options)
{
    this->map = map;
    this->pos = pos;
    this->tileset = tileset;
    this->opt = options;
}

void MapTile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    int id1 = map->get_tile_id_from_pos(this->pos, 0);
    int id2 = map->get_tile_id_from_pos(this->pos, 1);
    int id3 = map->get_tile_id_from_pos(this->pos, 2);

    QImage img1 = this->tileset->getTile(id1);
    QImage img2 = this->tileset->getTile(id2);
    QImage img3 = this->tileset->getTile(id3);

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
        RPGEvent *event = map->event_on_pos(pos);
        if (event != 0)
        {
            painter->setOpacity(0.5);
            painter->setPen(Qt::white);
            painter->drawRect(0,0,32,32);
            painter->fillRect(0,0,32,32, Qt::white);
            painter->setOpacity(1);
            painter->drawImage(QPoint(0,0),event->get_event_pic());
        }
    }

    //for debugging
    //painter->drawRect(0,0,32,32);
}

QRectF MapTile::boundingRect() const
{
    return QRectF(0,0,32,32);
}

