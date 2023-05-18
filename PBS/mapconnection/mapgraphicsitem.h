#ifndef MAPGRAPHICSITEM_H
#define MAPGRAPHICSITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <QMultiMap>
#include <QRect>

/*
inline uint qHash(const QRect & r)
{
    return qHash(qHash(r.left()) + qHash(r.top()) + qHash(r.width()) + qHash(r.bottom()));
}
*/

struct link
{
    QRect rect;
    int dest;
};

class MapGraphicsItem : public QGraphicsObject
{
    Q_OBJECT
public:
    MapGraphicsItem();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QRectF boundingRect() const override;

    int width();
    int height();

    void setImage(QImage img);
    void addLink(QRect link_area, int dest);
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;


signals:
    void mouse_clicked(int);

private:
    int h;
    int w;
    QImage img;
    QList<link> link_list;
};

#endif // MAPGRAPHICSITEM_H
