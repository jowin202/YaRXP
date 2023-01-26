#ifndef MAPLOCATIONAREA_H
#define MAPLOCATIONAREA_H

#include <QGraphicsView>
#include <QWidget>
#include <QMouseEvent>
#include <QGraphicsItem>

class Rectangle : public QGraphicsItem
{
public:
    Rectangle()
    {
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override {
        QPen pen;
        pen.setColor(QColor(200,200,200));
        pen.setWidth(2);
        painter->setPen(pen);
        painter->drawRect(1,1,31,31);
        pen.setColor(QColor(150,150,150));
        pen.setWidth(2);
        painter->setPen(pen);
        painter->drawRect(2,2,29,29);
        Q_UNUSED(option);
        Q_UNUSED(widget);
    };
    QRectF boundingRect() const override { return QRect(0,0,32,32); };
};

class MapLocationArea : public QGraphicsView
{
    Q_OBJECT
public:
    MapLocationArea(QWidget *parent);

    void mousePressEvent(QMouseEvent *e);

    Rectangle *rectangle = 0;
    void define_rectangle(int x, int y) { this->rectangle = new Rectangle;
                                          this->rectangle->setPos(32*x,32*y);
                                          this->scene()->addItem(rectangle);
                                          this->centerOn(rectangle); }
signals:
    void pressed(int,int);
};

#endif // MAPLOCATIONAREA_H
