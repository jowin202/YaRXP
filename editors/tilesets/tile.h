#ifndef TILE_H
#define TILE_H

#include <QGraphicsItem>
#include <QPainter>
#include <QDebug>


struct tile_options
{
    int mode;
};

class Tile : public QGraphicsItem
{
public:
    Tile(tile_options *opt, int tile_n, QList<int> *passages, QList<int> *priorities, QList<int> *terrain);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    enum { PASSAGES, PASSAGES4, PRIORITY, BUSH, COUNTER, TERRAIN };

private:
    tile_options *opt;
    QList<int> *passages;
    QList<int> *priorities;
    QList<int> *terrain;
    int tile_num;

};

#endif // TILE_H
