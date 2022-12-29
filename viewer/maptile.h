#ifndef MAPTILE_H
#define MAPTILE_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>


class RPGMap;
class RPGTileset;
struct tile_options;

class MapTile : public QGraphicsItem
{
public:
    MapTile(RPGMap *map, RPGTileset *tileset, QPoint pos, tile_options *options);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QRectF boundingRect() const override;


private:
    RPGMap *map;
    RPGTileset *tileset;
    QPoint pos;
    QColor purple = QColor(0x8a,0x16,0xa0);
    tile_options *opt;

};

#endif // MAPTILE_H
