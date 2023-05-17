#ifndef MAPGRAPHICSITEM_H
#define MAPGRAPHICSITEM_H

#include <QGraphicsItem>

class MapGraphicsItem : public QGraphicsItem
{
public:
    MapGraphicsItem(QImage img);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QRectF boundingRect() const override;

    int width();
    int height();

private:
    int h;
    int w;
};

#endif // MAPGRAPHICSITEM_H
