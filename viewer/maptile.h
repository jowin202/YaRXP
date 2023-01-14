#ifndef MAPTILE_H
#define MAPTILE_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

#include <QJsonObject>


class RPGMapController;
struct tile_options;

class MapTile : public QGraphicsItem
{
public:
    MapTile(RPGMapController *mc, QPoint pos, tile_options *options);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QRectF boundingRect() const override;



private:
    RPGMapController *mc;
    QPoint pos;
    tile_options *opt;

};

#endif // MAPTILE_H
