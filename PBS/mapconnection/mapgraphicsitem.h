#ifndef MAPGRAPHICSITEM_H
#define MAPGRAPHICSITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <QMultiMap>
#include <QRect>

#include "link.h"


class MapGraphicsItem : public QGraphicsObject
{
    Q_OBJECT
public:
    MapGraphicsItem();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QRectF boundingRect() const override;

    int width();
    int height();

    void setImage(QImage img, int self_id);
    void addLink(QRect link_area, int dest);
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;


signals:
    void mouse_clicked(int);

private:
    int h;
    int w;
    QImage img;
    QList<link> link_list;
    int self_id;
};

#endif // MAPGRAPHICSITEM_H
