#ifndef TILE_H
#define TILE_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QDebug>

class TilesetEditWidget;



struct tile_options
{
    int mode;
    TilesetEditWidget *parent;
};

class Tile : public QGraphicsItem
{
public:
    Tile(tile_options *opt, int tile_num, int passage, int priority, int terrain);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    enum { PASSAGES, PASSAGES4, PRIORITY, BUSH, COUNTER, TERRAIN };

private:
    tile_options *opt;
    int tile_num;
    int passage;
    int priority;
    int terrain;

};

#endif // TILE_H
